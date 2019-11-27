#ifndef _REFLECTANCE_H_
#define _REFLECTANCE_H_


#include <iostream>
#include <math.h>
#include <list>
#include "punto_direccion.h"
#include "camara.h"
#include "russian_roulette.h"

using namespace std;

class materialProperties{
protected:
    bool light_source;
	bool phong;
    int R, G, B;
    russianRoulette rR;
    //normalizar a 0.9 o el max de las 3
public:
	materialProperties(){}
    materialProperties(bool is_light_source, bool _phong, event_enum events[], double probs[]){
        this -> light_source = is_light_source;
        this -> rR = russianRoulette(events, probs);
		phong = _phong;
    }

    materialProperties(bool is_light_source, int _R, int _G, int _B, bool _phong, event_enum events[], double probs[]){
        this -> light_source = is_light_source;
        this -> R = _R;
        this -> G = _G;
        this -> B = _B;
		this -> rR = russianRoulette(events, probs);
		phong = _phong;
    }

    void setRGB(int _R,int _G,int _B){
        this -> R = _R;
        this -> G = _G;
        this -> B = _B;
    }

    int getR(){ return this->R;}
    int getG(){ return this->G;}
    int getB(){ return this->B;}

    bool isLightSource(){
        return this->light_source;
    }
	
	bool isPhong(){
		return this->phong;
	}

	event_enum evento(){
		return rR.event();
	}

};




#endif
