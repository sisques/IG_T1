#ifndef _FIGURAS_H_
#define _FIGURAS_H_


#include "punto_direccion.h"
#include <iostream>
#include <math.h>
#include "textures.h"
#include "camara.h"

using namespace std;

class figura{
protected:
    int R, G, B, text;
	camara cam;
	texture *texturizador;
	string im = "";
public:
    figura(camara c, int _R, int _G, int _B){
        this -> R = _R;
        this -> G = _G;
        this -> B = _B;
		this->text = 0;
		this -> cam = c;
		texturizador = new texture();
    }
	
	figura(camara c, int _R, int _G, int _B, int t){
		this -> R = _R;
        this -> G = _G;
        this -> B = _B;
		this->text = t;
		this->cam = c;
		if(t == 1){
			texturizador = new texture1();
		}
		else if(t == 2){
			texturizador = new texture2();
		}
		else if(t == 3){
			texturizador = new texture3();
		}
		else if(t == 4){
			texturizador = new texture4();
		}
		else if(t == 5){
			texturizador = new texture5();
		}
		else if(t == 6){
			texturizador = new texture6();
		}
		else if(t == 7){
			texturizador = new texture7();
		}
		else{
			texturizador = new texture();
		}
    }
	
	figura(camara c, int _R, int _G, int _B, int t, string im, dir d){
		this -> R = _R;
        this -> G = _G;
        this -> B = _B;
		this->text = t;
		this->cam = c;
		if(t == 8){
			texturizador = new texture8(im, d);
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
		if(this->text == 1 || this->text == 4 || this->text == 5){
			return texturizador->getR(p,this->R);
		}
		else if(this->text == 3){
			return texturizador->getR(this->cam.o, p,this->R);
		}
		return this->R;
	}
    virtual int getG(point p){
		if(this->text == 1 || this->text == 4 || this->text == 5){
			return texturizador->getG(p,this->G);
		}
		else if(this->text == 3 ){
			return texturizador->getG(this->cam.o, p,this->G);
		}
		return this->G;
	}
    virtual int getB(point p){
		if(this->text == 1 || this->text == 4 || this->text == 5){
			return texturizador->getB(p,this->B);
		}
		else if(this->text == 3){
			return texturizador->getB(this->cam.o, p,this->B);
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
    esfera(camara c, point _c, double _r, int _R, int _G, int _B): figura(c, _R, _G, _B){
        this -> c = _c;
        this -> r = _r;
    }
	
	esfera(camara c, point _c, double _r, int _R, int _G, int _B, int t): figura(c, _R, _G, _B, t){
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
		if(this->text == 2){
			return texturizador->getR(this->c, pp,this->R);
		}
		return figura::getR(pp);
	}
    int getG(point pp) override {
		if(this->text == 2){
			return texturizador->getG(this->c, pp,this->G);
		}
		return figura::getG(pp);
	}
    int getB(point pp) override {
		if(this->text == 2){
			return texturizador->getB(this->c, pp,this->B);
		}
		return figura::getB(pp);
	}
	
};


class plano : public figura {
private:
    point p;
    dir n;
public:
    plano(camara c, point _p, dir _n, int _R, int _G, int _B): figura(c, _R, _G, _B){
        this -> p = _p;
        this -> n = _n;
    }
	
	plano(camara c, point _p, dir _n, int _R, int _G, int _B, int t): figura(c, _R, _G, _B, t){
        this -> p = _p;
        this -> n = _n;
    }
	
	plano(camara c, point _p, dir _n, int _R, int _G, int _B, int t, string _im): figura(c, _R, _G, _B, t, _im, _n){
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
		if(this->text == 2 || this->text == 6 || this->text == 7){
			return texturizador->getR(this->p, pp,this->R);
		}
		else if(this->text == 8){
			return texturizador->getR(this->p, pp);
		}
		return figura::getR(pp);
	}
    int getG(point pp) override {
		if(this->text == 2 || this->text == 6 || this->text == 7){
			return texturizador->getG(this->p, pp,this->G);
		}
		else if(this->text == 8){
			return texturizador->getG(this->p, pp);
		}
		return figura::getG(pp);
	}
    int getB(point pp) override {
		if(this->text == 2 || this->text == 6 || this->text == 7){
			return texturizador->getB(this->p, pp,this->B);
		}
		else if(this->text == 8){
			return texturizador->getB(this->p, pp);
		}
		return figura::getB(pp);
	}
	
};

class triangulo : public figura {
private:
    point v0, v1, v2;
public:
    triangulo(camara c, point _v0, point _v1, point _v2, int _R, int _G, int _B): figura(c, _R, _G, _B){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
    }
	
	triangulo(camara c, point _v0, point _v1, point _v2, int _R, int _G, int _B, int t): figura(c, _R, _G, _B,t){
        this -> v0 = _v0;
        this -> v1 = _v1;
        this -> v2 = _v2;
    }
   /* bool implicit(point p) override  {
        dir d = p - this -> p;
        return dot(d, this -> n) <= 0;
    }*/

    dir getNormal(){
       dir arista1 = this->v1 - this->v0;
       dir arista2 = this->v2 - this->v0;
       return cross(arista1, arista2);
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
