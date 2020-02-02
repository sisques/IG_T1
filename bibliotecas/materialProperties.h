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

// Clase que representa un material y sus propiedades
class materialProperties{
protected:
    // Si es fuente de luz
    bool light_source;
    // RGB refracción
    double kdR, kdG, kdB;
    // RGB reflexión
    double ksR, ksG, ksB;
    // RGB difusión
    double PkdR, PkdG, PkdB;
    // RGB reflexión difusa
    double PksR, PksG, PksB;
    // Índice de refracción del objeto y del medio
    double indiceRefraccionObjeto, indiceRefraccionMedio;
    // índice de reflectacia
    double alfa;
    // Ruleta rusa correspondiente al material
    russianRoulette rR;

public:
    // Constructor
    materialProperties(){}
    // Destructor
    ~materialProperties(){}

    // Constructor que pide si es fuente de luz, los eventos que se pueden dar y sus probabilidades, y
    // el índice de refracción como opcional
    // Asigna como RGBs por defecto negro
    materialProperties(bool is_light_source, event_enum events[], double probs[], double r_obj = GLASS, double r_mdo = VACUUM){
        this -> light_source = is_light_source;
        this -> setKd(0,0,0);
        this -> setKs(0,0,0);
        this -> setKdPhong(0,0,0);
        this -> setKsPhong(0,0,0);
        this -> setIndiceRefraccionObjeto(r_obj);
        this -> setIndiceRefraccionMedio(r_mdo);
        this -> rR = russianRoulette(events, probs);
    }

    // Asigna el RGB de la refracción
    void setKd(int _R,int _G,int _B){
        double d = (double)MAX;
        this -> kdR = _R/d;
        this -> kdG = _G/d;
        this -> kdB = _B/d;
    }

    // Asigna el RGB de la reflexión
    void setKs(int _R,int _G,int _B){
        double d = (double)MAX;
        this -> ksR = _R/d;
        this -> ksG = _G/d;
        this -> ksB = _B/d;
    }

    // Asigna el RGB de la difusión
    void setKdPhong(int _R,int _G,int _B){
        double d = (double)MAX;
        this -> PkdR = _R/d;
        this -> PkdG = _G/d;
        this -> PkdB = _B/d;
    }

    // Asigna el RGB de la reflexión difusa
    void setKsPhong(int _R,int _G,int _B){
        double d = (double)MAX;
        this -> PksR = _R/d;
        this -> PksG = _G/d;
        this -> PksB = _B/d;
    }

    // Devuelve el RGB de la refracción
    double getKdR(){ return this->kdR;}
    double getKdG(){ return this->kdG;}
    double getKdB(){ return this->kdB;}

    // Devuelve el RGB de la reflexión
    double getKsR(){ return this->ksR;}
    double getKsG(){ return this->ksG;}
    double getKsB(){ return this->ksB;}

    // Devuelve el RGB de la difusión
    double getKdPhongR(){ return this->PkdR;}
    double getKdPhongG(){ return this->PkdG;}
    double getKdPhongB(){ return this->PkdB;}

    // Devuelve el RGB de la reflexión difusa
    double getKsPhongR(){ return this->PksR;}
    double getKsPhongG(){ return this->PksG;}
    double getKsPhongB(){ return this->PksB;}

    // Asigna el valor de la reflectancia
    void setAlfa(double a){this->alfa = a;}

    // Devuelve el valor de la reflectancia
    double getAlfa(){return this->alfa;}

    // Asigna el valor del índice de refracción del objeto
    void setIndiceRefraccionObjeto(double r){this->indiceRefraccionObjeto = r;}

    // Devuelve el valor del índice de refracción del objeto
    double getIndiceRefraccionObjeto(){return this->indiceRefraccionObjeto;}

    // Asigna el valor del índice de refracción del medio
    void setIndiceRefraccionMedio(double r){this->indiceRefraccionMedio = r;}

    // Devuelve el valor del índice de refracción del medio
    double getIndiceRefraccionMedio(){return this->indiceRefraccionMedio;}

    // Devuelve true si es una fuente de luz
    bool isLightSource(){
        return this->light_source;
    }

    // Devuelve un evento basado en la ruleta rusa
    event_enum evento(){
        return rR.event();
    }

    // Devuelve la probabilidad de un evento en concreto
    double probEvent(event_enum e){
        return rR.probEvent(e);
    }

};

#endif