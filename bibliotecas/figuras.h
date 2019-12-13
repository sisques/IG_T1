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
#include "phong.h"
#include <list>

using namespace std;

class figura{
protected:
    texture_enum text;
    texture *texturizador;
    materialProperties mp;
    list<point> lightPoints;

public:
    figura(materialProperties _mp){
        this -> mp = _mp;
        this->text = NO_TEXTURE;
        texturizador = nullptr;//new texture();
        srand(0);
    }

    figura(texture_enum t, materialProperties _mp){
        this -> mp = _mp;
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
        srand (time(NULL));
    }

    figura(texture_enum t, string im, dir d, materialProperties _mp){
        this -> mp = _mp;
        this->text = t;
        if(t == IMAGE){
            texturizador = new texture3(im, d);
        }
        else {
            texturizador = new texture();
        }
        srand (time(NULL));
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

    void getRGB(event_enum e, double &r, double &g, double &b){
        if ( e == REFLEXION || e == EMISSION) {
            r = mp.getKsR();
            g = mp.getKsG();
            b = mp.getKsB();
        } else if (e == REFRACTION) {
            r = mp.getKdR();
            g = mp.getKdG();
            b = mp.getKdB();
        }
        if (r < 0) { r = 0; }
        else if (r > 1) { r = 1; }
        if (g < 0) { g = 0; }
        else if (g > 1) { g = 1; }
        if (b < 0) { b = 0; }
        else if (b > 1) { b = 1; }
    }

    void phongColor(const dir indir, const dir outdir, point p, double &r, double &g, double &b) {
        dir refindir = reflection(indir, getNormal(p),p);
        double cos = dot(refindir, outdir);
        double aux = pow(cos, mp.getAlfa());
        r = (mp.getKdPhongR()/M_PI) + (mp.getKsPhongR()*(mp.getAlfa()+2)/(2*M_PI))*aux;
        g = (mp.getKdPhongG()/M_PI) + (mp.getKsPhongG()*(mp.getAlfa()+2)/(2*M_PI))*aux;
        b = (mp.getKdPhongB()/M_PI) + (mp.getKsPhongB()*(mp.getAlfa()+2)/(2*M_PI))*aux;
    }

    /*virtual double getR(point p){
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
    }*/

    virtual bool intersection(dir rd, point ro, double &t){
        return false;
    }


    dir reflection(dir _in, dir _n, point o){

        dir inputRay = normalize(_in);
        dir normal = normalize(_n);
        dir output = 2.0*dot(normal, inputRay)*normal -inputRay;
        return -normalize(output);
    }

      double reflectance0(double n1, double n2) {
        double sqrt_R0 = (n1 - n2) / (n1 + n2);
        return sqrt_R0 * sqrt_R0;
    }

      double schlickReflectance(double n1, double n2, double c) {
        double R0 = reflectance0(n1, n2);
        return R0 + (1 - R0) * c * c * c * c * c;
    }

// basado en https://github.com/matt77hias/java-smallpt/blob/master/src/core/Specular.java
    virtual dir refraction(dir d, dir n, point o) {
        dir d_Re = reflection(d,n,o);
        bool fuer_a_dent = dot(n,d);
        dir nl = fuer_a_dent ? n : n;
        double n_out = mp.getIndiceRefraccionMedio();
        double n_in = mp.getIndiceRefraccionObjeto();
        double r = fuer_a_dent ? n_out / n_in : n_in / n_out;
        double cos_theta = dot(d,nl);
        double cos2_phi = 1.0 - r * r * (1.0 - cos_theta * cos_theta);

        //reflexion interna
        if (cos2_phi < 0) {
            return d_Re;
        }

        dir d_Tr = normalize(r*d - nl*(r*cos_theta + sqrt(cos2_phi)));
        double c = 1.0 - (fuer_a_dent ? -cos_theta : dot(d_Tr,n));
        double Re = schlickReflectance(n_out, n_in, c);
        double p_Re = 0.25 + 0.5 * Re;


        double rnd = (double)rand() / RAND_MAX;
        if (rnd < p_Re) {
            return d_Re;
        }
        else  {
            return d_Tr;
        }


    }

    dir phongDir(dir outdir, dir n, double specexp) {
        Matrix mat;
        dir ldir = normalize(outdir);

        dir ref = reflection(ldir, n, newPoint(0,0,0));

        double ndotl = dot(ldir, n);

        if(1.0 - ndotl > EPSILON) {
            dir ivec, kvec, jvec;

            // build orthonormal basis
            if(fabs(ndotl) < EPSILON) {
                kvec = -normalize(ldir);
                jvec = n;
                ivec = cross(jvec, kvec);
            } else {
                ivec = normalize(cross(ldir, ref));
                jvec = ref;
                kvec = cross(ref, ivec);
            }

            mat = originalBase(ivec, jvec, kvec, newPoint(0,0,0));
        }

        double rnd1 = (double)rand() / RAND_MAX;
        double rnd2 = (double)rand() / RAND_MAX;

        double phi = acos(pow(rnd1, 1.0 / (specexp + 1.0)));
        double theta = 2.0 * M_PI * rnd2;

        dir v;
        v.x = cos(theta) * sin(phi);
        v.y = cos(phi);
        v.z = sin(theta) * sin(phi);
        v = mat*v;

        return v;
    }

    virtual dir getNormal() {return newDir(0,0,0);}
    virtual dir getNormal(point p) {return newDir(0,0,0);}

    dir nextRay(event_enum evento, dir inputRay, point inputPoint) {
        dir normal = this -> getNormal(inputPoint);
        if ( evento == REFLEXION) {
            return reflection(inputRay, normal, inputPoint);
        } else if (evento == REFRACTION) {
            return refraction(inputRay, normal, inputPoint);
        }
        else if (evento == PHONG){
            return phongDir(inputRay, normal, mp.getAlfa());
        }
        else{
            return normal;
        }
    }
};

class punto : public figura {
private:
    point c;
public:
    punto(point _c, materialProperties _mp): figura(_mp){
        c = _c;
        if(mp.isLightSource()){
            this->lightPoints.push_back(newPoint(c.x, c.y, c.z));
        }
    }

    bool intersection(dir rd, point ro, double &t) override {
        dir d = normalize(c - ro);
        rd = normalize(rd);
        t = mod(d)/mod(rd);
        return (d.x+EPSILON > rd.x &&  d.x-EPSILON < rd.x)
               && (d.y+EPSILON > rd.y &&  d.y-EPSILON < rd.y)
               && (d.z+EPSILON > rd.z &&  d.z-EPSILON < rd.z);
    }
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

    /*double getR(point pp) override {
        return figura::getR(pp);
    }
    double getG(point pp) override {
        return figura::getG(pp);
    }
    double getB(point pp) override {
        return figura::getB(pp);
    }*/


    dir getNormal() override {
        return newDir(0,0,0);
    }
    dir getNormal(point p) override {
        return normalize(p - this -> getCenter());
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
    dir getNormal() override { return normalize(this->n);}
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

    /*double getR(point pp) override {
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
    }*/



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
        return normalize(this -> normal);
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

};

#endif
