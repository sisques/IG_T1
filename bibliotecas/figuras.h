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

class figura{
protected:
    double R, G, B;
    texture_enum text;
    texture *texturizador;
    materialProperties mp;
	list<point> lightPoints;
	camara c =  newCamara(newPoint(0,0,0), newDir(0,1,0), newDir(1,0,0), newDir(0,0,1));
	
public:
    figura(materialProperties _mp){
        this -> mp = _mp;
        this -> R = mp.getR();
        this -> G = mp.getG();
        this -> B = mp.getB();
        this->text = NO_TEXTURE;
        texturizador = new texture();
		srand(0);
    }

    figura(texture_enum t, materialProperties _mp){
        this -> mp = _mp;
        this -> R = mp.getR();
        this -> G = mp.getG();
        this -> B = mp.getB();
        this->text = t;
        if(t == WOOD){
            texturizador = new texture1();
        }
        else if(t == PERLIN_NOISE){
            texturizador = new texture2();
        }

        else{//textures.flatColor
            texturizador = new texture();
        }
		srand(0);
    }

    figura(texture_enum t, string im, dir d, materialProperties _mp){
        this -> mp = _mp;
        this -> R = mp.getR();
        this -> G = mp.getG();
        this -> B = mp.getB();
        this->text = t;
        if(t == IMAGE){
            texturizador = new texture3(im, d);
        }
        else {
            texturizador = new texture();
        }
		srand(0);
    }

	event_enum evento(){
		return mp.evento();
	}
	
	bool isLight(){
		return mp.isLightSource();
	}
	
	double probEvent(event_enum e){
		return mp.probEvent(e);
	}
	
	virtual list<point> getLightPoints(){
		return this->lightPoints;
	}

    double getR(){ return this->R;}
    double getG(){ return this->G;}
    double getB(){ return this->B;}

    virtual double getR(point p){
        if(this->text == WOOD|| this->text == PERLIN_NOISE){
            return texturizador->getR(p,this->R);
        }
        return this->R;
    }
    virtual double getG(point p){
        if(this->text == WOOD|| this->text == PERLIN_NOISE){
            return texturizador->getG(p,this->G);
        }
        return this->G;
    }
    virtual double getB(point p){
        if(this->text == WOOD|| this->text == PERLIN_NOISE){
            return texturizador->getB(p,this->B);
        }
        return this->B;
    }

    virtual bool intersection(dir rd, point ro, double &t){
        return false;
    }
	
	virtual dir nextRay(event_enum evento, dir inputRay, point inputPoint){
    }
    
	dir reflexion(dir _in, dir _n, point o){
		dir in = normalize(_in);
        dir n = normalize(_n);
        dir y = n;
        dir x = in;
        dir z = cross(x,y);
        x = cross(y,z);
        Matrix new_base = newBase(x,y,z,o);
        Matrix original_Base = originalBase(x,y,z,o);
        dir inputRay = in*new_base;
        dir normal = n*new_base;
        dir output = 2*dot(normal, inputRay)*normal -inputRay;
        return -normalize(output*original_Base);
    }
    dir refraction(dir _in, dir _n, point o){
        dir in = normalize(_in);
        dir n = normalize(_n);
        dir y = n;
        dir x = in;
        dir z = cross(x,y);
        x = cross(y,z);
        double n1 = 1.00029;
        double n2 = 1.33;
        Matrix new_base = newBase(x,y,z,o);
        Matrix original_Base = originalBase(x,y,z,o);
        dir inputRay = in*new_base;
        dir normal = n*new_base;
        double r = n1 / n2;
        double c1 = dot(-normal,inputRay);
        double c2 = sqrt(1-r*r*(1-c1*c1));
        dir output = r*inputRay + (r*c1 - c2)*normal;
        return normalize(output*original_Base);
    }


    virtual dir getNormal() {}
    virtual dir getNormal(point p) {}
};



class esfera : public figura {
private:
    point c;
    double r;
public:
    esfera(point _c, double _r,  materialProperties _mp): figura(_mp){
        this -> c = _c;
        this -> r = _r;
		if(mp.isLightSource()){
			this->lightPoints.push_back(newPoint(c.x, c.y, c.z));
			this->lightPoints.push_back(newPoint(c.x+r, c.y, c.z));
			this->lightPoints.push_back(newPoint(c.x, c.y+r, c.z));
			this->lightPoints.push_back(newPoint(c.x, c.y, c.z+r));
			this->lightPoints.push_back(newPoint(c.x-r, c.y, c.z));
			this->lightPoints.push_back(newPoint(c.x, c.y-r, c.z));
			this->lightPoints.push_back(newPoint(c.x, c.y, c.z-r));
		}
    }

	esfera(point _c, double _r, texture_enum t,  materialProperties _mp): figura(t,_mp){
        this -> c = _c;
        this -> r = _r;
		if(mp.isLightSource()){
			this->lightPoints.push_back(newPoint(c.x, c.y, c.z));
			this->lightPoints.push_back(newPoint(c.x+r, c.y, c.z));
			this->lightPoints.push_back(newPoint(c.x, c.y+r, c.z));
			this->lightPoints.push_back(newPoint(c.x, c.y, c.z+r));
			this->lightPoints.push_back(newPoint(c.x-r, c.y, c.z));
			this->lightPoints.push_back(newPoint(c.x, c.y-r, c.z));
			this->lightPoints.push_back(newPoint(c.x, c.y, c.z-r));
		}
    }


    point getCenter(){ return this->c;}
    double getRadius(){ return this->r;}


    bool intersection(dir rd, point ro, double &t) override {
        double aux  = dot(this->c - ro, rd);
        if (aux < 0){
            //corta detras del punto de origen
            return false;
        }
        point p = ro + rd*aux;
        double y = mod(this->c - p);
        if ( y <= r) {
            t = aux - sqrt(this->r * this->r - y * y);
            return true;
        } else {
            return false;
        }
    }

    double getR(point pp) override {
        return figura::getR(pp);
    }
    double getG(point pp) override {
        return figura::getG(pp);
    }
    double getB(point pp) override {
        return figura::getB(pp);
    }


    dir getNormal() override {
        return newDir(0,0,0);
    }
    dir getNormal(point p) override {
        return p - this -> getCenter();
    }

    dir nextRay(event_enum evento, dir inputRay, point inputPoint) override {
        dir normal = inputPoint - this -> getCenter();
        if ( evento == REFLEXION) {
            return reflexion(inputRay, normal, inputPoint);
        } else if (evento == REFRACTION) {
            return refraction(inputRay, normal, inputPoint);
        }
		else if (evento == PHONG){
			double x = rand()/RAND_MAX, y = rand()/RAND_MAX, z = rand()/RAND_MAX;
			dir aux = ((normal*rotateX(x*M_PI_2))*rotateY(y*M_PI_2))*rotateZ(z*M_PI_2);
			return aux;
		}
		else{
			return normal;
		}
    }

	
};

class plano : public figura {
private:
    point p;
    dir n;
public:
    plano( point _p, dir _n, materialProperties _mp): figura(_mp){
        this -> p = _p;
        this -> n = _n;
		if(mp.isLightSource()){
			this->lightPoints.push_back(newPoint(p.x, p.y, p.z));
			this->lightPoints.push_back(newPoint(p.x+2, p.y, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y+2, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y, p.z+2));
			this->lightPoints.push_back(newPoint(p.x-2, p.y, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y-2, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y, p.z-2));
		}
    }
	
	plano( point _p, dir _n,texture_enum t, materialProperties _mp): figura( t,_mp){
        this -> p = _p;
        this -> n = _n;
		if(mp.isLightSource()){
			this->lightPoints.push_back(newPoint(p.x, p.y, p.z));
			this->lightPoints.push_back(newPoint(p.x+2, p.y, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y+2, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y, p.z+2));
			this->lightPoints.push_back(newPoint(p.x-2, p.y, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y-2, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y, p.z-2));
		}
    }

	plano( point _p, dir _n, texture_enum t, string _im,  materialProperties _mp): figura(t, _im, _n, _mp){
        this -> p = _p;
        this -> n = _n;
		if(mp.isLightSource()){
			this->lightPoints.push_back(newPoint(p.x, p.y, p.z));
			this->lightPoints.push_back(newPoint(p.x+2, p.y, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y+2, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y, p.z+2));
			this->lightPoints.push_back(newPoint(p.x-2, p.y, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y-2, p.z));
			this->lightPoints.push_back(newPoint(p.x, p.y, p.z-2));
		}
    }
	

    point getPoint(){ return this->p;}
    dir getNormal() override { return this->n;}
    dir getNormal(point p) override {return this->getNormal();}

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

    double getR(point pp) override {
		if(this->text == IMAGE){
			return texturizador->getR(this->p, pp);
		}
		return figura::getR(pp);
	}
    double getG(point pp) override {
        if(this->text == IMAGE){
			return texturizador->getG(this->p, pp);
		}
		return figura::getG(pp);
	}
    double getB(point pp) override {
        if(this->text == IMAGE){
            return texturizador->getB(this->p, pp);
		}
		return figura::getB(pp);
	}
	
	dir nextRay(event_enum evento, dir inputRay, point inputPoint) override {
        dir normal = this -> getNormal() ;
        if ( evento == REFLEXION) {
            return reflexion(inputRay, normal, inputPoint);
        } else if (evento == REFRACTION) {
            return refraction(inputRay, normal, inputPoint);
        }
		else if (evento == PHONG){
			double x = (double)rand()/RAND_MAX, y = (double)rand()/RAND_MAX, z = (double)rand()/RAND_MAX;
			dir aux = ((normal*rotateX(x*M_PI_2))*rotateY(y*M_PI_2))*rotateZ(z*M_PI_2);
			return aux;
		}
		else{
			return normal;
		}
    }
	
};

class triangulo : public figura {
private:
    point v0, v1, v2;
    dir normal;
public:
    triangulo(point _v0, point _v1, point _v2,  materialProperties _mp): figura(_mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        dir arista1 = this->v1 - this->v0;
        dir arista2 = this->v2 - this->v0;
        this -> normal =  cross(arista1, arista2);
    }
    triangulo( point _v0, point _v1, point _v2, dir _normal,  materialProperties _mp): figura(_mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        this -> normal =  _normal;
    }

	triangulo(point _v0, point _v1, point _v2, texture_enum t,  materialProperties _mp): figura(t,_mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        dir arista1 = this->v1 - this->v0;
        dir arista2 = this->v2 - this->v0;
        this -> normal =  cross(arista1, arista2);
    }

    triangulo(point _v0, point _v1, point _v2, dir _normal, texture_enum t, materialProperties _mp): figura(t, _mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        this -> normal =  _normal;
    }

    dir getNormal() override {
        return this -> normal;
    }
	
	
	list<point> getLightPoints() override{
		list<point> aux;
		aux.push_back(v0);
		aux.push_back(v1);
		aux.push_back(v2);
		return aux;
	}
	
    dir getNormal(point p) override {return this->getNormal();}
    point getVertice0(){ return this->v0;}
    point getVertice1(){ return this->v1;}
    point getVertice2(){ return this->v2;}

    //Implementacion del algoritmo de Möller-Trumbore
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
    dir nextRay(event_enum evento, dir inputRay, point inputPoint) override {
        dir normal = this -> getNormal();
        if ( evento == REFLEXION) {
            return reflexion(inputRay, normal, inputPoint);
        } else if (evento == REFRACTION) {
            return refraction(inputRay, normal, inputPoint);
        }
		else if (evento == PHONG){
			double x = rand()/RAND_MAX, y = rand()/RAND_MAX, z = rand()/RAND_MAX;
			dir aux = ((normal*rotateX(x*M_PI_2))*rotateY(y*M_PI_2))*rotateZ(z*M_PI_2);
			return aux;
		}
		else{
			return normal;
		}
    }
};

#endif
