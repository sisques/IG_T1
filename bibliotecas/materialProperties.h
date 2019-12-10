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
    double refValue;
    double alfa;
    double ref;
    russianRoulette rR;

public:
    ~materialProperties(){}
    materialProperties(){}
    materialProperties(bool is_light_source, event_enum events[], double probs[], double r = 1.00029){
        this -> light_source = is_light_source;
        this -> setKd(0,0,0);
        this -> setKs(0,0,0);
        this -> setKdPhong(0,0,0);
        this -> setKsPhong(0,0,0);
        this -> ref = r;
        this -> rR = russianRoulette(events, probs);
    }

    void setKd(int _R,int _G,int _B){
        this -> kdR = _R/255.0;
        this -> kdG = _G/255.0;
        this -> kdB = _B/255.0;
    }

    void setKs(int _R,int _G,int _B){
        this -> ksR = _R/255.0;
        this -> ksG = _G/255.0;
        this -> ksB = _B/255.0;
    }

    void setKdPhong(int _R,int _G,int _B){
        this -> PkdR = _R/255.0;
        this -> PkdG = _G/255.0;
        this -> PkdB = _B/255.0;
    }

    void setKsPhong(int _R,int _G,int _B){
        this -> PksR = _R/255.0;
        this -> PksG = _G/255.0;
        this -> PksB = _B/255.0;
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

    void setRefValue(double r){this->ref = r;}

    double getRefValue(){return this->ref;}

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
