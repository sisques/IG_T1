#ifndef _FIGURAS_H_
#define _FIGURAS_H_


#include "punto_direccion.h"
#include <iostream>
#include <math.h>
#include "textures.h"
#include "camara.h"
#include "materialProperties.h"
#include "matrix.h"
#include "camara.h"
#include "globals.h"
#include <list>

using namespace std;

// Clase que representa una las geometría
class figura{
protected:
	// Identificador de textura
    texture_enum text;
	// Objeto con el que se aplica la textura
    texture *texturizador;
	// Propiedades del material de la figura
    materialProperties mp;
	// Lista de puntos de luz
    list<point> lightPoints;
	// True si es un punto, false si es cualquier otro tipo de figura
	bool isPoint = false;

public:

	// Constructor únicamente con propiedades de material
	// Se le asigna un texturizador nulo, y se indica que no tiene textura
    figura(materialProperties _mp){
        this -> mp = _mp;
        this->text = NO_TEXTURE;
        texturizador = nullptr;
        srand(static_cast<unsigned int>(clock()));
    }

	// Constructor con propiedade de material, y un identificador de textura básica
    figura(texture_enum t, materialProperties _mp){
        this -> mp = _mp;
        this->text = t;
		// Textura tipo madera basado en perlin noise
        if(t == WOOD){
            texturizador = new texture1();
        }
		// Textura con perlin noise básico
        else if(t == PERLIN_NOISE){
            texturizador = new texture2();
        }
		// Textura de color plano, como si no tuviese
        else{
            texturizador = new texture();
        }
		srand(static_cast<unsigned int>(clock()));
    }

	// Constructor pensado únicamente para planos
	// Pide una textura, la ruta de una imagen, la dirección en que mira la imagen
	// y las propiedades del material
    figura(texture_enum t, string im, dir d, materialProperties _mp){
        this -> mp = _mp;
        this->text = t;
		// Textura imagen
        if(t == IMAGE){
            texturizador = new texture3(im, d);
        }
		// Si no sin textura
        else {
            texturizador = new texture();
        }
		srand(static_cast<unsigned int>(clock()));
    }

	// Devuelve un evento en base a la ruleta rusa de las propiedades
	// del materiañ
    event_enum evento(){
        return mp.evento();
    }

	// Devuelve cierto si es una fuente de luz
    bool isLight(){
        return mp.isLightSource();
    }
	
	// Devuelve cierto si la geometría es un punto
	bool IsPoint(){
		return this->isPoint;
	}

	// Devuelve la probabilidad de ocurrencia de un evento
    double probEvent(event_enum e){
        return mp.probEvent(e);
    }
	
	// Devuelve la lista de puntos de luz
    virtual list<point> getLightPoints(){
        return this->lightPoints;
    }
	
	// Configura la escala de la textura
	void setScale(double p){texturizador->setScale(p);}

	// Devuelve el RGB de una figura según el punto, evento y las propiedades del material
    virtual void getRGB(event_enum e, point p, double &r, double &g, double &b){
		if ( e == REFLEXION || e == EMISSION) {
            r = mp.getKsR();
			g = mp.getKsG();
			b = mp.getKsB();
        } else if (e == REFRACTION) {
            r = mp.getKdR();
			g = mp.getKdG();
			b = mp.getKdB();
        }
		// Si tiene una textura tipo madera o perlin noise
		// aplica la textura al RGB
		if(this->text == WOOD|| this->text == PERLIN_NOISE){
            r = texturizador->getR(p,r);
			g = texturizador->getG(p,g);
			b = texturizador->getB(p,b);
        }
	}
	
	// Devuelve el RGB de la geometría aplicando la BRDF phong, en base al rayo de entrada,
	// de salida y el punto
	virtual void phongColor(const dir indir, const dir outdir, point p, double &r, double &g, double &b) {
		dir refindir = reflexion(indir, getNormal(p));
		double cos = dot(refindir, outdir);
		double aux = pow(cos, mp.getAlfa());
		r = (mp.getKdPhongR()/M_PI) + (mp.getKsPhongR()*(mp.getAlfa()+2)/(2*M_PI))*aux;
		g = (mp.getKdPhongG()/M_PI) + (mp.getKsPhongG()*(mp.getAlfa()+2)/(2*M_PI))*aux;
		b = (mp.getKdPhongB()/M_PI) + (mp.getKsPhongB()*(mp.getAlfa()+2)/(2*M_PI))*aux;
		// Si tiene una textura tipo madera o perlin noise
		// aplica la textura al RGB
		if(this->text == WOOD|| this->text == PERLIN_NOISE){
            r = texturizador->getR(p,r);
			g = texturizador->getG(p,g);
			b = texturizador->getB(p,b);
        }
	}

	// Devuelve true si el rayo representado por la dir rd y el punto ro
	// intersecciona con la geometría, en caso afirmativo devuelve en t el
	// escalar por el que se ha de multiplicar rd para hallar el punto de corte
    virtual bool intersection(dir rd, point ro, double &t){
        return false;
    }

	// Devuelve el rayo de reflexión en base a el rayo de entrada y la normal
    dir reflexion(dir _in, dir _n){
        dir inputRay = normalize(_in);
        dir normal = normalize(_n);
        dir output = 2.0*dot(normal, inputRay)*normal -inputRay;
        return -normalize(output);
    }
	
	 dir reflexion(dir _in, dir _n, point o){

        dir inputRay = normalize(_in);
        dir normal = normalize(_n);
        dir output = 2.0*dot(normal, inputRay)*normal -inputRay;
        return -normalize(output);
    }
	
    // basado en https://stackoverflow.com/questions/42218704/how-to-properly-handle-refraction-in-raytracing
	// Devuelve el rayo de refracción en base a el rayo de entrada y la normal
    /*virtual dir refraction(dir d, dir n, point o) {
        double r = mp.getIndiceRefraccionObjeto();
        double cosI = dot(d,-n);
        if (cosI < 0) {
           r  = 1.0 / r;
        }
        return (d * r - n * (-cosI + r * cosI));
    }*/
	
	double reflectance0(double n1, double n2) {
        double sqrt_R0 = (n1 - n2) / (n1 + n2);
        return sqrt_R0 * sqrt_R0;
    }

    double schlickReflectance(double n1, double n2, double c) {
        double R0 = reflectance0(n1, n2);
        return R0 + (1 - R0) * c * c * c * c * c;
    }

    // basado en https://github.com/matt77hias/java-smallpt/blob/master/src/core/Specular.java
    virtual dir refraction(dir d, dir n, point o, bool &reflexionInterna) {
        reflexionInterna = false;
        dir d_Re = -reflexion(d,n,o);
        bool out_to_in = dot(n,d) < 0;
        dir nl = out_to_in ? -n : n;
        double n_out = mp.getIndiceRefraccionMedio();
        double n_in = mp.getIndiceRefraccionObjeto();
        double r = out_to_in ? n_out / n_in : n_in / n_out;
        double cos_theta = dot(d,nl);
        double cos2_phi = 1.0 - r * r * (1.0 - cos_theta * cos_theta);
        //reflexion interna
        if (cos2_phi < 0) {
            reflexionInterna = true;
            return d_Re;
        }
        dir d_Tr = normalize(r*d - nl*(r*cos_theta + sqrt(cos2_phi)));
        double c = 1.0 - (out_to_in ? -cos_theta : dot(d_Tr,n));
        double Re = schlickReflectance(n_out, n_in, c);
        double p_Re = 0.25 + 0.5 * Re;
        double rnd = (double)rand() / RAND_MAX;
        if (rnd < p_Re) {
            return d_Re;
        }
        else  {
            return normalize(d_Tr);
        }
    }
	
	// Basado en https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF?url=3d-basic-rendering/phong-shader-BRDF
	// Devuelve el rayo de refracción en base a el rayo de entrada, la normal y la reflectancia
	dir phongDir(dir indir, dir n, double reflect) {
		Matrix mat;
		// Dirección de entrada del rayo
		dir ldir = normalize(indir);
		// Dirección del rayo de reflexión perfecta
		dir ref = reflexion(ldir, n);
		// Producto escalar del rayo de entrada y la normal
		double ndotl = dot(ldir, n);

		if(1.0 - ndotl > EPSILON) {
			dir ivec, kvec, jvec;
			// Halla la base de coordenadas local de la geometría
			if(fabs(ndotl) < EPSILON) {
				kvec = -normalize(ldir);
				jvec = n;
				ivec = cross(jvec, kvec);
			} else {
				ivec = normalize(cross(ldir, ref));
				jvec = ref;
				kvec = cross(ref, ivec);
			}
			// Crea la matriz de la base
			mat = originalBase(ivec, jvec, kvec, newPoint(0,0,0));
		}
		// Genera una dirección con cosine sampling
		double rnd1 = (double)rand() / RAND_MAX;
		double rnd2 = (double)rand() / RAND_MAX;
		double phi = acos(pow(rnd1, 1.0 / (reflect + 1.0)));
		double theta = 2.0 * M_PI * rnd2;
		dir v;
		v.x = cos(theta) * sin(phi);
		v.y = cos(phi);
		v.z = sin(theta) * sin(phi);
		// Pasa la dirección a coordenadas globales
		v = mat*v;
		return v;
	}

	// Devuelve la normal de la geometría
    virtual dir getNormal() {return newDir(0,0,0);}
	// Devuelve la normal de la geometría en un punto
    virtual dir getNormal(point p) {return newDir(0,0,0);}

	// Devuelve una dirección en base al tipo de evento, el rayo de entrada
	// y un punto
    dir nextRay(event_enum evento, dir inputRay, point inputPoint, bool &reflexionInterna) {
        dir normal = this -> getNormal(inputPoint);
        reflexionInterna = false;
        // Si el evento es reflexión
        if ( evento == REFLEXION) {
            return reflexion(inputRay, normal);
            // Si el evento es refracción
        } else if (evento == REFRACTION) {
            return refraction(inputRay, normal, inputPoint, reflexionInterna);
        }
            // Si el evento es phong
        else if (evento == PHONG){
            return phongDir(inputRay, normal, mp.getAlfa());
        }
            // Por defecto
        else{
            return normal;
        }
    }
	
	// Devuelve uen p un punto aleatorio de la geometría
	// y en d una dirección aleatoria
	// Se usa para photon mapping
	virtual void getLightRay(point &p, dir &d) {}
};

// Clase que representa un punto como geometría/figura
class punto : public figura {
private:
	// Posición del punto
    point c;
public:
	// Constructor que pide la posición y propiedades de material
	punto(point _c, materialProperties _mp): figura(_mp){
		this->isPoint = true;
		c = _c;
		if(mp.isLightSource()){
            this->lightPoints.push_back(newPoint(c.x, c.y, c.z));
        }
	}
	
	// Devuelve cierto si el rayo representado por rd y ro intersecciona
	// con el punto, y devuelve en t el escalar por el que multiplicar rd
	// para alcanzar el punto
	// Obviamente solo interseccionará si es un rayo directo como serían
	// los rayos de sombra
	bool intersection(dir rd, point ro, double &t) override {
        dir d = normalize(c - ro);
		rd = normalize(rd);
		t = mod(d)/mod(rd);
		return (d.x+EPSILON > rd.x &&  d.x-EPSILON < rd.x) 
				&& (d.y+EPSILON > rd.y &&  d.y-EPSILON < rd.y)
				&& (d.z+EPSILON > rd.z &&  d.z-EPSILON < rd.z);
    }
	
	// Devuelve en p su posición y en d una dirección aleatoria
	void getLightRay(point &p, dir &d) override{
		double rx,ry,rz;
		do{
			rx = 2.0f * ((double)rand() / (double)RAND_MAX) - 1.0f;
			ry = 2.0f * ((double)rand() / (double)RAND_MAX) - 1.0f;
			rz = 2.0f * ((double)rand() / (double)RAND_MAX) - 1.0f;
		}while(rx*rx + ry*ry+ rz*rz > 1.0);
		dir rayo = newDir(rx,ry,rz);
		rayo = normalize(rayo);
		d = rayo;
		p = c;
	}
};

// Clase que representa una esfera como geometría/figura
class esfera : public figura {
private:
	// Centro de la esfera
    point c;
	// Radio de la esfera
    double r;
public:
	// Constructor que pide el centro y radio de la esfera, y sus propiedades
    esfera(point _c, double _r,  materialProperties _mp): figura(_mp){
        this -> c = _c;
        this -> r = _r;
    }
	// Constructor que pide el centro y radio de la esfera, y su textura y propiedades
    esfera(point _c, double _r, texture_enum t,  materialProperties _mp): figura(t,_mp){
        this -> c = _c;
        this -> r = _r;
    }
	
	// Devuelve el centro de la esfera
    point getCenter(){ return this->c;}
	// Devuelve el radio de la esfera
    double getRadius(){ return this->r;}

	// Devuelve true si el rayo representado por la dir rd y el punto ro
	// intersecciona con la esfera, en caso afirmativo devuelve en t el
	// escalar por el que se ha de multiplicar rd para hallar el punto de corte
    bool intersection(dir rd, point ro, double &t) override {
        double aux  = dot(this->c - ro, rd);
        if (aux < 0){
            //corta detras del punto de origen
			t = aux;
            return false;
        }
        point p = ro + rd*aux;
        double y = mod(this->c - p);
        if ( y <= r) {
			t = aux - sqrt(this->r * this->r - y * y);
			if(t == 0){
				t = aux + sqrt(this->r * this->r - y * y);
			}
            return true;
        } else {
            return false;
        }
    }

	// Devuelve una normal inválida, ya que depende del punto de la suferficie
    dir getNormal() override {
        return newDir(0,0,0);
    }
	// Devuelve la normal de la esfera en el punto p
    dir getNormal(point p) override {
        return normalize(p - this -> getCenter());
    }
	
	// Devuelve en d un rayo aleatorio y en p el punto de partida del rayo
	// el cual es el punto de intersección del rayo partiendo del centro
	void getLightRay(point &p, dir &d) override{
		double rx,ry,rz;
		do{
			rx = 2.0f * ((double)rand() / (double)RAND_MAX) - 1.0f;
			ry = 2.0f * ((double)rand() / (double)RAND_MAX) - 1.0f;
			rz = 2.0f * ((double)rand() / (double)RAND_MAX) - 1.0f;
		}while(rx*rx + ry*ry+ rz*rz > 1.0);
		dir rayo = newDir(rx,ry,rz);
		d = rayo;
		double t;
		intersection(d, c, t);
		p = c + rayo * (t);
	}
};

// Clase que representa un plano como geometría/figura
class plano : public figura {
private:
	// Punto de referencia del plano
    point p;
	// Normal del plano
    dir n;
public:
	// Constructor que pide el punto de referencia, la normal y las propiedades del plano
    plano( point _p, dir _n, materialProperties _mp): figura(_mp){
        this -> p = _p;
        this -> n = _n;
    }

	// Constructor que pide el punto de referencia, la normal, la textura y las propiedades del plano
    plano( point _p, dir _n,texture_enum t, materialProperties _mp): figura( t,_mp){
        this -> p = _p;
        this -> n = _n;
    }

	// Constructor que pide el punto de referencia, la normal, la textura, ruta de la imagen y las propiedades del plano
    plano( point _p, dir _n, texture_enum t, string _im,  materialProperties _mp): figura(t, _im, _n, _mp){
        this -> p = _p;
        this -> n = _n;
	}

	// Devuelve el punto de referencia del plano
    point getPoint(){ return this->p;}
	
	// Devuelve la normal del plano
    dir getNormal() override { return normalize(this->n);}
	
	// Devuelve la normal del plano
    dir getNormal(point p) override {return this->getNormal();}
	
	// Devuelve el RGB del plano aplicando la BRDF phong, en base al rayo de entrada,
	// de salida y el punto
	// Si la textura es una imagen se hace un procesamiento especial, en caso contrario
	// se usa el phong color de la clase padre
	void phongColor(const dir indir, const dir outdir, point pp, double &r, double &g, double &b) override {
		if(this->text == IMAGE){
			dir refindir = reflexion(indir, getNormal(p));
			double cos = dot(refindir, outdir);
			double aux = pow(cos, mp.getAlfa());
            r = texturizador->getR(p,pp);
			g = texturizador->getG(p,pp);
			b = texturizador->getB(p,pp);
			r = (r/M_PI) + (r*(mp.getAlfa()+2)/(2*M_PI))*aux;
			g = (g/M_PI) + (g*(mp.getAlfa()+2)/(2*M_PI))*aux;
			b = (b/M_PI) + (b*(mp.getAlfa()+2)/(2*M_PI))*aux;
        }
		else{
			figura::phongColor(indir,outdir,pp,r,g,b);
		}
	}

	// Devuelve true si el rayo representado por la dir rd y el punto ro
	// intersecciona con el plano, en caso afirmativo devuelve en t el
	// escalar por el que se ha de multiplicar rd para hallar el punto de corte
    bool intersection(dir rd, point ro, double &t) override {
        dir diff = this->p - ro;
        double p1 = dot(diff, this->n);
        double p2 = dot(rd, this->n);
        if (p2 == 0){
            //lineas paralelas
            return false;
        } else if (p1 == 0) {
            //linea contenida en el plano
            return false;
        } else {
            //hay un puntpo de interseccion
            t = p1 / p2;
            if (t < 0) {
                //corta detras del punto de origenm
                return false;
            }
            return true;
        }

    }

	// Devuelve el RGB del plano según el punto, evento y las propiedades de este
	void getRGB(event_enum e, point pp, double &r, double &g, double &b) override{
		if(this->text == IMAGE){
			r = texturizador->getR(this->p, pp);
			g = texturizador->getG(this->p, pp);
			b = texturizador->getB(this->p, pp);
		}
		else{
			figura::getRGB(e,p,r,g,b);
		}
	}
	
	// Devuelve en d un rayo aleatorio y en p el punto de partida del rayo
	// el cual es un punto aleatorio del plano en un radio de 0.6 del punto
	// de referencia
	void getLightRay(point &pl, dir &d) override{
		// Hallamos un punto aleatorio sobre un plano
		pl.x = ((double)rand() / (double)RAND_MAX);
		pl.z = ((double)rand() / (double)RAND_MAX); 
		pl.y = 0;
		// Generamos una dirección aleatoria con cosine sampling
		double u = (double)rand() / (double)RAND_MAX;
		double v = 2*M_PI*(double)rand() / (double)RAND_MAX;
		d = newDir(2.0f*cos(v)*sqrt(u)-1,2.0f*sin(v)*sqrt(u)-1, 2.0f*sqrt(1-u)-1);
		// Nos aseguramos de que la dirección no vaya hacia abajo respecto el plano
		if(d.y < 0){d.y = -d.y;}
		// Hallamos una base para el plano
		Matrix mat;
		dir ldir = newDir(1,0,1);
		if(ldir.x ==  1 && ldir.y ==  1 && ldir.z ==  1){
			ldir = newDir(1,1,1);
		}
		dir ivec, kvec, jvec;
		kvec = -normalize(cross(n,ldir));
		jvec = n;
		ivec = cross(jvec, kvec);
		mat = originalBase(ivec, jvec, kvec, newPoint(p.x,p.y,p.z));
		// Trasladamos la dirección y el punto a la base del plano
		d = mat*d;
		pl = mat*pl;
		// Limitamos los valores
		if(pl.x-p.x > 0.6){pl.x -= 0.6;}
		if(pl.y-p.y > 0.6){pl.y -= 0.6;}
		if(pl.z-p.z > 0.6){pl.z -= 0.6;}
		d = normalize(d);
	}
};

// Clase que representa un triángulo como geometría
class triangulo : public figura {
private:
	// Los vértices del triángulo
    point v0, v1, v2;
	// La normal del triángulo
    dir normal;
public:
	// Constructor que pide los 3 vértives y las propiedades
    triangulo(point _v0, point _v1, point _v2,  materialProperties _mp): figura(_mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        dir arista1 = this->v1 - this->v0;
        dir arista2 = this->v2 - this->v0;
        this -> normal =  cross(arista1, arista2);
    }
	// Constructor que pide los 3 vértives, la normal y las propiedades
    triangulo( point _v0, point _v1, point _v2, dir _normal,  materialProperties _mp): figura(_mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        this -> normal =  _normal;
    }
	// Constructor que pide los 3 vértives, una textura y las propiedades
    triangulo(point _v0, point _v1, point _v2, texture_enum t,  materialProperties _mp): figura(t,_mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        dir arista1 = this->v1 - this->v0;
        dir arista2 = this->v2 - this->v0;
        this -> normal =  cross(arista1, arista2);
    }
	// Constructor que pide los 3 vértives, la normal, una textura y las propiedades
    triangulo(point _v0, point _v1, point _v2, dir _normal, texture_enum t, materialProperties _mp): figura(t, _mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        this -> normal =  _normal;
    }

	// Devuelve la normal del triángulo
    dir getNormal() override {
        return normalize(this -> normal);
    }

	// Devuelve la normal del triángulo
    dir getNormal(point p) override {return this->getNormal();}
	// Devuelven los vértices
    point getVertice0(){ return this->v0;}
    point getVertice1(){ return this->v1;}
    point getVertice2(){ return this->v2;}

    //Implementacion del algoritmo de Möller-Trumbore
	// Devuelve true si el rayo representado por la dir rd y el punto ro
	// intersecciona con el triángulo, en caso afirmativo devuelve en t el
	// escalar por el que se ha de multiplicar rd para hallar el punto de corte
    bool intersection(dir rd, point ro, double &t) override {
        const double EPSILON = 0.0000001;
        dir arista1, arista2, h, s, q;
        double a, f, u, v;
        arista1 = this->v1 - this->v0;
        arista2 = this->v2 - this->v0;
        h = cross(rd, arista2);
        a = dot(arista1, h);

        //El rayo es paralelo al triangulo
        if (a > -EPSILON && a < EPSILON){
            return false;
        }

        f = 1.0/a;
        s = ro - this -> v0;
        u = f*dot(s,h);
        if (u < 0.0 || u > 1.0) {
            return false;
        }

        q = cross(s, arista1);
        v = f*dot(rd, q);
        if (v < 0.0 || u + v > 1.0){
            return false;
        }

        //Ahora se pasa a calcular t para obtener el punto de interseccion
        t = f*dot(arista2, q);
        if (t < 0){
            //corta detras del punto de origen
            return false;
        }
        //Hay interseccion
        return t > EPSILON && t < 1 / EPSILON;
    }

};

#endif
