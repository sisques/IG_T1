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
    double indiceRefraccionObjeto;
    double alfa;
    russianRoulette rR;

public:
    ~materialProperties(){}
    materialProperties(){}
    materialProperties(bool is_light_source, event_enum events[], double probs[], double r_obj = GLASS){
        this -> light_source = is_light_source;
        this -> rR = russianRoulette(events, probs);
        this -> setKd(0,0,0);
        this -> setKs(0,0,0);
        this -> setKdPhong(0,0,0);
        this -> setKsPhong(0,0,0);
        this -> setIndiceRefraccionObjeto(r_obj);

    }
    void setKd(int _R,int _G,int _B){
        this -> kdR = (double)_R/CR;
        this -> kdG = (double)_G/CR;
        this -> kdB = (double)_B/CR;
    }

    void setKs(int _R,int _G,int _B){
        this -> ksR = (double)_R/CR;
        this -> ksG = (double)_G/CR;
        this -> ksB = (double)_B/CR;
    }

    void setKdPhong(int _R,int _G,int _B){
        this -> PkdR = (double)_R/CR;
        this -> PkdG = (double)_G/CR;
        this -> PkdB = (double)_B/CR;
    }

    void setKsPhong(int _R,int _G,int _B){
        this -> PksR = (double)_R/CR;
        this -> PksG = (double)_G/CR;
        this -> PksB = (double)_B/CR;
    }


    void setKd(double _R,double _G,double _B){
        this -> kdR = _R;
        this -> kdG = _G;
        this -> kdB = _B;
    }

    void setKs(double _R,double _G,double _B){
        this -> ksR = _R;
        this -> ksG = _G;
        this -> ksB = _B;
    }

    void setKdPhong(double _R,double _G,double _B){
        this -> PkdR = _R;
        this -> PkdG = _G;
        this -> PkdB = _B;
    }

    void setKsPhong(double _R,double _G,double _B){
        this -> PksR = _R;
        this -> PksG = _G;
        this -> PksB = _B;
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
