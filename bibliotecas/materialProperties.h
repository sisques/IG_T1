#ifndef _REFLECTANCE_H_
#define _REFLECTANCE_H_


#include <iostream>
#include <math.h>
#include <list>
#include "punto_direccion.h"
#include "camara.h"

using namespace std;

enum brdf_index {
        PHONG_BRDF,
        DELTA_BRDF,
        DELTA_BTDF
};

class materialProperties{
protected:
    bool light_source;
    int R, G, B;
    double *brdfValues;
    //normalizar a 0.9 o el max de las 3
public:
    materialProperties(bool is_light_source, double _brdfValues[]){
        this -> light_source = is_light_source;
        this -> brdfValues = _brdfValues;
    }

    materialProperties(bool is_light_source, int _R, int _G, int _B, double _brdfValues[]){
        this -> light_source = is_light_source;
        this -> R = _R;
        this -> G = _G;
        this -> B = _B;
        this -> brdfValues = _brdfValues;
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

};




#endif
