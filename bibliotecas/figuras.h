#ifndef _FIGURAS_H_
#define _FIGURAS_H_


#include "punto_direccion.h"
#include <iostream>
#include <math.h>
#include "textures.h"
#include "camara.h"
#include "materialProperties.h"

using namespace std;

class figura{
protected:
    int R, G, B;
    texture_enum text;
    texture *texturizador;
    string im = "";
    materialProperties *mp;
public:
    figura(materialProperties *_mp){
        this -> mp = _mp;
        this -> R = mp->getR();
        this -> G = mp->getG();
        this -> B = mp->getB();
        this->text = NO_TEXTURE;
        texturizador = new texture();

    }

    figura(texture_enum t, materialProperties *_mp){
        this -> mp = _mp;
        this -> R = mp->getR();
        this -> G = mp->getG();
        this -> B = mp->getB();
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
    }

    figura(texture_enum t, string im, dir d, materialProperties *_mp){
        this -> mp = _mp;
        this -> R = mp->getR();
        this -> G = mp->getG();
        this -> B = mp->getB();
        this->text = t;
        if(t == IMAGE){
            texturizador = new texture3(im, d);
        }
        else {
            texturizador = new texture();
        }
    }

    virtual bool implicit(point p) {
        return false;
    };
    int getR(){ return this->R;}
    int getG(){ return this->G;}
    int getB(){ return this->B;}

    virtual int getR(point p){
        if(this->text == WOOD|| this->text == PERLIN_NOISE){
            return texturizador->getR(p,this->R);
        }
        return this->R;
    }
    virtual int getG(point p){
        if(this->text == WOOD|| this->text == PERLIN_NOISE){
            return texturizador->getG(p,this->G);
        }
        return this->G;
    }
    virtual int getB(point p){
        if(this->text == WOOD|| this->text == PERLIN_NOISE){
            return texturizador->getB(p,this->B);
        }
        return this->B;
    }

    virtual bool intersection(dir rd, point ro, double &t, double &dist){
        return false;
    }
};



class esfera : public figura {
private:
    point c;
    double r;
public:
    esfera(point _c, double _r,  materialProperties *_mp): figura(_mp){
        this -> c = _c;
        this -> r = _r;
    }

	esfera(point _c, double _r, texture_enum t,  materialProperties *_mp): figura(t,_mp){
        this -> c = _c;
        this -> r = _r;
    }


    point getCenter(){ return this->c;}
    double getRadius(){ return this->r;}


    bool implicit(point p) override {
        dir aux = p - this -> c;
        return ( dot(aux, aux) - this -> r*this -> r ) <= 0;
    }


    /*
     * Los puntos interseccion se calcularan usando la siguiente formula:
     *  p1 = ro + rd*(t-dist)
     *  p2 = ro + rd*(t+dist)
     */
    bool intersection(dir rd, point ro, double &t, double &dist) override {
        point s = this->c;
        double r = this->r;
        t = dot(s - ro, rd);
        point p = ro + rd*t;
        double y = mod(s - p);
        if ( y < r) {
            dist = sqrt(r * r - y * y);
            return true;
        } else {
            return false;
        }
    }
	
	int getR(point pp) override {
		return figura::getR(pp);
	}
    int getG(point pp) override {
		return figura::getG(pp);
	}
    int getB(point pp) override {
		return figura::getB(pp);
	}
	
};

class plano : public figura {
private:
    point p;
    dir n;
public:
    plano( point _p, dir _n, materialProperties *_mp): figura(_mp){
        this -> p = _p;
        this -> n = _n;
    }
	
	plano( point _p, dir _n,texture_enum t, materialProperties *_mp): figura( t,_mp){
        this -> p = _p;
        this -> n = _n;
    }

	plano( point _p, dir _n, texture_enum t, string _im,  materialProperties *_mp): figura(t, _im, _n, _mp){
        this -> p = _p;
        this -> n = _n;
    }
	
    bool implicit(point p) override  {
        dir d = p - this -> p;
        return dot(d, this -> n) <= 0;
    }

    point getPoint(){ return this->p;}
    dir getNormal(){ return this->n;}

    bool intersection(dir rd, point ro, double &t, double &dist) override {
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
                return false;
            }
            dist = 0;
            return true;
        }

    }
	
	int getR(point pp) override {
		if(this->text == IMAGE){
			return texturizador->getR(this->p, pp);
		}
		return figura::getR(pp);
	}
    int getG(point pp) override {
        if(this->text == IMAGE){
			return texturizador->getG(this->p, pp);
		}
		return figura::getG(pp);
	}
    int getB(point pp) override {
        if(this->text == IMAGE){
            return texturizador->getB(this->p, pp);
		}
		return figura::getB(pp);
	}
	
};

class triangulo : public figura {
private:
    point v0, v1, v2;
    dir normal;
public:
    triangulo(point _v0, point _v1, point _v2,  materialProperties *_mp): figura(_mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        dir arista1 = this->v1 - this->v0;
        dir arista2 = this->v2 - this->v0;
        this -> normal =  cross(arista1, arista2);
    }
    triangulo( point _v0, point _v1, point _v2, dir _normal,  materialProperties *_mp): figura(_mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        this -> normal =  _normal;
    }

	triangulo(point _v0, point _v1, point _v2, texture_enum t,  materialProperties *_mp): figura(t,_mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        dir arista1 = this->v1 - this->v0;
        dir arista2 = this->v2 - this->v0;
        this -> normal =  cross(arista1, arista2);
    }

    triangulo(point _v0, point _v1, point _v2, dir _normal, texture_enum t, materialProperties *_mp): figura(t, _mp){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
        this -> normal =  _normal;
    }
   /* bool implicit(point p) override  {
        dir d = p - this -> p;
        return dot(d, this -> n) <= 0;
    }*/

    dir getNormal(){
        return this -> normal;
    }
    point getVertice0(){ return this->v0;}
    point getVertice1(){ return this->v1;}
    point getVertice2(){ return this->v2;}

    //Implementacion del algoritmo de Möller-Trumbore
    bool intersection(dir rd, point ro, double &t, double &dist) override {
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
        dist = 0;
        t = f*dot(arista2, q);
        //Hay interseccion
        if (t > EPSILON && t < 1/EPSILON) {
            return true;
        }
        // Intersecta la linea pero no el plano
        else {
            return false;
        }

    }
};

#endif
