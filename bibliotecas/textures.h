#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "punto_direccion.h"
#include <iostream>
#include <math.h>
#include "perlinNoise.h"
#include "ppm_reader.h"
#include <list>
#include "matrix.h"

using namespace std;

// Texturas aplicables
enum texture_enum { NO_TEXTURE, WOOD, PERLIN_NOISE, IMAGE};

// Clase básica para definir texturas, en las que se sobrescribirán
// todas las funciones. Se hace una implementación mínima de sus funciones
// para poder instanciar un puntero a ella, donde cabrá cualquier hija
class texture{
protected:
	// Resolución del color
	double cr = 256.0;
	// Escala de aplicación de la textura, según este valor las texturas
	// tendrán una mayor o menor proporción
	double scale = 25;
public:
	// Constructor por defecto
    texture(){}
	// Constructor que pide la resolución de color
	texture(double _cr){cr = _cr;}

	// Se actualiza la escala
	void setScale(double p){
		scale = p;
	}

	// Para obtener la modificación del color correspondiente
	// en base a la posición
	virtual double getR(point p, double r){ return r;}
    virtual double getG(point p, double g){ return g;}
    virtual double getB(point p, double b){ return b;}
	
	// Para obtener el color correspondiente según dos puntos
	// Se usa únicamente para las de texturas de imagen, pero
	// se incluye de nuevo para poder instanciar una texture
	// y poder acceder desde ella a cualquier hija
	virtual double getR(point c, point p){ return cr-1;}
    virtual double getG(point c, point p){ return cr-1;}
    virtual double getB(point c, point p){ return cr-1;}
};

// Efecto madera basado en Perlin Noise
class texture1 : public texture {
private:
	// Estructura encargada de calcular Perlin Noise
	perlinNoise pn;
public:

	// Contructor por defecto, genera perlinNoise
	texture1():texture(){
		pn = perlinNoise();
	}
	// Contructor que pide la resolución del color, genera perlinNoise
	texture1(double _cr):texture(_cr){
		pn = perlinNoise();
	}
	
	// Aplica perlinNoise al rojo según la posición p para obtener un
	// efecto tipo madera
	double getR(point p, double r) override {
		double n = 20.0 * pn.noise(p.x*scale, p.y*scale, p.z*scale);
		n = n - floor(n);
		return r * n;
	}
	// Aplica perlinNoise al verde según la posición p para obtener un
	// efecto tipo madera
    double getG(point p, double g) override {
		double n = 20.0 * pn.noise(p.x*scale, p.y*scale, p.z*scale);
		n = n - floor(n);
		return g * n;
	}
	// Aplica perlinNoise al azul según la posición p para obtener un
	// efecto tipo madera
    double getB(point p, double b) override {
		double n = 20.0 * pn.noise(p.x*scale, p.y*scale, p.z*scale);
		n = n - floor(n);
		return b * n;
	}
};

// Efecto de "difuminado" en zonas aplicando Perlin Noise
class texture2 : public texture {
private:
	// Estructura encargada de calcular Perlin Noise
	perlinNoise pn;
	
public:
	// Contructor por defecto, genera perlinNoise
	texture2():texture(){
		pn = perlinNoise();
	}
	// // Contructor que pide la resolución de color, genera perlinNoise
	texture2(double _cr):texture(_cr){
		pn = perlinNoise();
	}

	// Aplica perlinNoise al rojo según la posición p para obtener un
	// efecto tipo difuminado
	double getR(point p, double r) override {
		double n = pn.noise(p.x*scale, p.y*scale, p.z*scale);
		return r * n;
	}
	// Aplica perlinNoise al verde según la posición p para obtener un
	// efecto tipo difuminado
    double getG(point p, double g) override {
		double n = pn.noise(p.x*scale, p.y*scale, p.z*scale);
		return g * n;
	}
	// Aplica perlinNoise al azul según la posición p para obtener un
	// efecto tipo difuminado
    double getB(point p, double b) override {
		double n = pn.noise(p.x*scale, p.y*scale, p.z*scale);
		return b * n;
	}
};

// Textura pensada unicamente para planos que sean paredes de una habitación,
// es decir, que no estén inclinados
class texture3 : public texture {
	// Matrices para guardar la imagen
    int **Rs;
    int **Gs;
    int **Bs;
	// Altura y anchura de la imagen
    int width, height;
	// Normal del plano
    dir n;
public:
	// Contructor que pide la ruta de la imagen y la normal del plano
    texture3(string im, dir _n):texture(){
        n = _n;
		// Lee la imagen
        list<int*> pixels = ppmReader(im);
		// Asigna la altura y anchura de la imagen
        width = pixels.front()[0];
        height = pixels.front()[1];
        pixels.pop_front();
		// Reserva la memoria necesaria para la imagen
        Rs = new int*[height];
        for (int i = 0; i < height; ++i){Rs[i] = new int[width];}
        Gs = new int*[height];
        for (int i = 0; i < height; ++i){Gs[i] = new int[width];}
        Bs = new int*[height];
        for (int i = 0; i < height; ++i){Bs[i] = new int[width];}
		// Asigna a la matriz los colores de los pixeles
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                Rs[i][j] = pixels.front()[0];
                Gs[i][j] = pixels.front()[1];
                Bs[i][j] = pixels.front()[2];
                pixels.pop_front();
            }
        }

    }
	// Destructor que elimina las matrices de memoria
    ~texture3(){
        for (int i = 0; i < height; ++i){
            delete [] Rs[i];
        }
        delete [] Rs;
        for (int i = 0; i < height; ++i){
            delete [] Gs[i];
        }
        delete [] Gs;
        for (int i = 0; i < height; ++i){
            delete [] Bs[i];
        }
        delete [] Bs;
    }
	
	// Devuelve el rojo correspondiente en base a la distancia
	// entre dos puntos, que se asumen serán el centro del plano y
	// el punto deaseado
    double getR(point c, point p) override {
        dir aux = c - p;
		// Se obtienen las coordenadas del pixel deseado de la imagen
		// según la normal del plano
        int x = (int)(aux.x*scale)%width;
        int y = (int)(aux.y*scale)%height;
        if(n.x == 0 && n.z == 0){
            x = (int)(aux.x*scale)%width;
            y = (int)(aux.z*scale)%height;
        }
        else if(n.y == 0 && n.z == 0){
            x = (int)(aux.z*scale)%width;
            y = (int)(aux.y*scale)%height;
        }
        if(x < 0){x = width + x;}
        if(y < 0){y = height + y;}
        return Rs[y][x]/cr;
    }
	// Devuelve el verde correspondiente en base a la distancia
	// entre dos puntos, que se asumen serán el centro del plano y
	// el punto deaseado
    double getG(point c, point p) override {
        dir aux = c - p;
		// Se obtienen las coordenadas del pixel deseado de la imagen
		// según la normal del plano
        int x = (int)(aux.x*scale)%width;
        int y = (int)(aux.y*scale)%height;
        if(n.x == 0 && n.z == 0){
            x = (int)(aux.x*scale)%width;
            y = (int)(aux.z*scale)%height;
        }
        else if(n.y == 0 && n.z == 0){
            x = (int)(aux.z*scale)%width;
            y = (int)(aux.y*scale)%height;
        }
        if(x < 0){x = width + x;}
        if(y < 0){y = height + y;}
        return Gs[y][x]/cr;
    }
	// Devuelve el azul correspondiente en base a la distancia
	// entre dos puntos, que se asumen serán el centro del plano y
	// el punto deaseado
    double getB(point c, point p) override {
        dir aux = c - p;
		// Se obtienen las coordenadas del pixel deseado de la imagen
		// según la normal del plano
        int x = (int)(aux.x*scale)%width;
        int y = (int)(aux.y*scale)%height;
        if(n.x == 0 && n.z == 0){
            x = (int)(aux.x*scale)%width;
            y = (int)(aux.z*scale)%height;
        }
        else if(n.y == 0 && n.z == 0){
            x = (int)(aux.z*scale)%width;
            y = (int)(aux.y*scale)%height;
        }
        if(x < 0){x = width + x;}
        if(y < 0){y = height + y;}
        return Bs[y][x]/cr;
    }
};
#endif
