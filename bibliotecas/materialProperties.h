#ifndef _MATERIAL_PROPERTIES_H_
#define _MATERIAL_PROPERTIES_H_


#include <iostream>
#include <cmath>
#include <list>
#include "globals.h"
#include "punto_direccion.h"
#include "camara.h"
#include "russian_roulette.h"

using namespace std;

class materialProperties{
protected:
    bool light_source;
    double kdR, kdG, kdB;
    double ksR, ksG, ksB;
    double PkdR, PkdG, PkdB;
    double PksR, PksG, PksB;
    double indiceRefraccionObjeto, indiceRefraccionMedio;
    double alfa;
    russianRoulette rR;

public:
    ~materialProperties(){}
    materialProperties(){}
    materialProperties(bool is_light_source, event_enum events[], double probs[], double r_obj = GLASS, double r_mdo = VACUUM ){
        this -> light_source = is_light_source;
        this -> setKd(0,0,0);
        this -> setKs(0,0,0);
        this -> setKdPhong(0,0,0);
        this -> setKsPhong(0,0,0);
        this -> setIndiceRefraccionObjeto(r_obj);
        this -> setIndiceRefraccionMedio(r_mdo);

        this -> rR = russianRoulette(events, probs);
    }

    void setKd(int _R,int _G,int _B){
        double d = (double)MAX;
        this -> kdR = _R/d;
        this -> kdG = _G/d;
        this -> kdB = _B/d;
    }

    void setKs(int _R,int _G,int _B){
        double d = (double)MAX;
        this -> ksR = _R/d;
        this -> ksG = _G/d;
        this -> ksB = _B/d;
    }

    void setKdPhong(int _R,int _G,int _B){
        double d = (double)MAX;
        this -> PkdR = _R/d;
        this -> PkdG = _G/d;
        this -> PkdB = _B/d;
    }

    void setKsPhong(int _R,int _G,int _B){
        double d = (double)MAX;
        this -> PksR = _R/d;
        this -> PksG = _G/d;
        this -> PksB = _B/d;
    }

    double getKdR(){ return this->kdR;}
    double getKdG(){ return this->kdG;}
    double getKdB(){ return this->kdB;}

    double getKsR(){ return this->ksR;}
    double getKsG(){ return this->ksG;}
    double getKsB(){ return this->ksB;}

    double getKdPhongR(){ return this->PkdR;}
    double getKdPhongG(){ return this->PkdG;}
    double getKdPhongB(){ return this->PkdB;}

    double getKsPhongR(){ return this->PksR;}
    double getKsPhongG(){ return this->PksG;}
    double getKsPhongB(){ return this->PksB;}

    void setAlfa(double a){this->alfa = a;}

    double getAlfa(){return this->alfa;}

    void setIndiceRefraccionObjeto(double r){this->indiceRefraccionObjeto = r;}

    double getIndiceRefraccionObjeto(){return this->indiceRefraccionObjeto;}

    void setIndiceRefraccionMedio(double r){this->indiceRefraccionMedio = r;}

    double getIndiceRefraccionMedio(){return this->indiceRefraccionMedio;}

    bool isLightSource(){
        return this->light_source;
    }

    event_enum evento(){
        return rR.event();
    }

    double probEvent(event_enum e){
        return rR.probEvent(e);
    }

};

#endif