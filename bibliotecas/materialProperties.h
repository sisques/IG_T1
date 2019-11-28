#ifndef _REFLECTANCE_H_
#define _REFLECTANCE_H_


#include <iostream>
#include <math.h>
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
    //normalizar a 0.9 o el max de las 3
public:
	materialProperties(){}
    materialProperties(bool is_light_source, bool _phong, event_enum events[], double probs[]){
        this -> light_source = is_light_source;
        this -> rR = russianRoulette(events, probs);
		phong = _phong;
    }

    void setRGB(int _R,int _G,int _B){
        this -> R = _R/255.0;
        this -> G = _G/255.0;
        this -> B = _B/255.0;
    }

    materialProperties(bool is_light_source, double _R, double _G, double _B, bool _phong, event_enum events[], double probs[]){
        this -> light_source = is_light_source;
        this -> setRGB(_R,_G,_B);
		this -> rR = russianRoulette(events, probs);
		phong = _phong;
    }



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
