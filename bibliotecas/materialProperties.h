#ifndef _REFLECTANCE_H_
#define _REFLECTANCE_H_


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
	bool phong;
    double R, G, B;
    russianRoulette rR;
	double irradiance;
    //normalizar a 0.9 o el max de las 3
public:
	materialProperties(){}
    materialProperties(bool is_light_source, bool _phong, event_enum events[], double probs[], double ir){
        this -> light_source = is_light_source;
        this -> setRGB(0,0,0);
        this -> rR = russianRoulette(events, probs);
		phong = _phong;
		irradiance = ir;
    }

    void setRGB(int _R,int _G,int _B){
        this -> R = _R/255.0;
        this -> G = _G/255.0;
        this -> B = _B/255.0;
    }

    materialProperties(bool is_light_source, bool _phong, event_enum events[], double probs[], double _R, double _G, double _B, double ir){
        this -> light_source = is_light_source;
        this -> setRGB(_R,_G,_B);
		this -> rR = russianRoulette(events, probs);
		phong = _phong;
		irradiance = ir;
    }

	double getIrradiance(){ return this->irradiance;}

		double getR(){ return this->R;}
    double getG(){ return this->G;}
    double getB(){ return this->B;}

    bool isLightSource(){
        return this->light_source;
    }
	
	bool isPhong(){
		return this->phong;
	}

	event_enum evento(){
		return rR.event();
	}
	
	double probEvent(event_enum e){
		return rR.probEvent(e);
	}

};




#endif
