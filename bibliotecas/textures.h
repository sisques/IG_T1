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

enum texture_enum { NO_TEXTURE, WOOD, PERLIN_NOISE, IMAGE};

class texture{
protected:
	double cr = 256.0;
	double scale = 25;
public:
    texture(){}
	texture(double _cr){cr = _cr;}

	void setScale(double p){
		scale = p;
	}

	virtual double getR(point p, double r){ return r;}
    virtual double getG(point p, double g){ return g;}
    virtual double getB(point p, double b){ return b;}

	virtual double getR(point c, point p, double r){ return r;}
    virtual double getG(point c, point p, double g){ return g;}
    virtual double getB(point c, point p, double b){ return b;}

	virtual double getR(point c, point p){ return cr-1;}
    virtual double getG(point c, point p){ return cr-1;}
    virtual double getB(point c, point p){ return cr-1;}
};

//Madera
class texture1 : public texture {
private:
	perlinNoise pn;
public:
	texture1():texture(){
		pn = perlinNoise();
	}
	texture1(double _cr):texture(_cr){
		pn = perlinNoise();
	}

	double getR(point p, double r) override {
		double n = 20.0 * pn.noise(p.x*scale, p.y*scale, p.z*scale);
		n = n - floor(n);
		return r * n;
	}
    double getG(point p, double g) override {
		double n = 20.0 * pn.noise(p.x*scale, p.y*scale, p.z*scale);
		n = n - floor(n);
		return g * n;
	}
    double getB(point p, double b) override {
		double n = 20.0 * pn.noise(p.x*scale, p.y*scale, p.z*scale);
		n = n - floor(n);
		return b * n;
	}
};

//Perlin_noise
class texture2 : public texture {
private:
	perlinNoise pn;
	
public:
	texture2():texture(){
		pn = perlinNoise();
	}
	texture2(double _cr):texture(_cr){
		pn = perlinNoise();
	}

	double getR(point p, double r) override {
		double n = pn.noise(p.x*scale, p.y*scale, p.z*scale);
		return r * n;
	}
    double getG(point p, double g) override {
		double n = pn.noise(p.x*scale, p.y*scale, p.z*scale);
		return g * n;
	}
    double getB(point p, double b) override {
		double n = pn.noise(p.x*scale, p.y*scale, p.z*scale);
		return b * n;
	}
};

//Imagen
class texture3 : public texture {
    int **Rs;
    int **Gs;
    int **Bs;
    int width, height;
    dir n;
public:
    texture3(string im, dir _n):texture(){
        n = _n;
        list<int*> pixels = ppmReader(im);
        width = pixels.front()[0];
        height = pixels.front()[1];
        pixels.pop_front();

        Rs = new int*[height];
        for (int i = 0; i < height; ++i){Rs[i] = new int[width];}
        Gs = new int*[height];
        for (int i = 0; i < height; ++i){Gs[i] = new int[width];}
        Bs = new int*[height];
        for (int i = 0; i < height; ++i){Bs[i] = new int[width];}

        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                Rs[i][j] = pixels.front()[0];
                Gs[i][j] = pixels.front()[1];
                Bs[i][j] = pixels.front()[2];
                pixels.pop_front();
            }
        }

    }
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
    double getR(point c, point p) override {
        dir aux = c - p;
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
    double getG(point c, point p) override {
        dir aux = c - p;
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
    double getB(point c, point p) override {
        dir aux = c - p;
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
