#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <iostream>
#include <math.h>
#include "camara.h"
#include "russian_roulette.h"
#include <list>
#include <memory>
#include <limits>
#include "globals.h"
#include "phong.h"


using namespace std;

class pathTracer
{
private:

    list<shared_ptr<figura>> getLuces(list<shared_ptr<figura>> &e){
        list<shared_ptr<figura>> luces;
        for(shared_ptr<figura> i:e){
            if(i->isLight()){
                luces.push_back(i);
            }
        }
        return luces;
    }

    bool colision(point c, const list<shared_ptr<figura>> &e, dir rayo, shared_ptr<figura> &fig, point &col){
        double t = 0;
        double distMin = numeric_limits<double>::max();
        double distActual = 0;
        bool colision;
        bool yes = false;
        shared_ptr<figura> nearest;
        for( auto it = e.begin(); it != e.end(); ++it){
            shared_ptr<figura> f = *it;
            colision = f->intersection(rayo, c, t);
            if (colision) {
                point p = c + rayo * t;
                distActual = mod(c - p);
                if (distActual < distMin) {
                    nearest = f;
                    distMin = distActual;
                    col = p;
                    yes = true;
                }
            }

        }
        fig = nearest;
        return yes;
    }

    double luzDirecta(list<shared_ptr<figura>> &e, point p){
        list<shared_ptr<figura>> luces = getLuces(e);
        list<point> puntosLuces;
        for(shared_ptr<figura> i:luces){
            for(point x:i->getLightPoints()){
                puntosLuces.push_back(x);
            }
        }
        list<dir> rayos;
        for(point d:puntosLuces){
            rayos.push_back(d-p);
        }
        for(dir i:rayos){
            shared_ptr<figura> fig;
            point trash;
            bool colisiona = colision(p, e, i, fig, trash);
            if(colisiona && fig->isLight()){
                return 1.0;
            }
        }
        return 0.0;
    }

public:
    pathTracer(){}
    ~pathTracer(){};

    void getRGB(point c, const list<shared_ptr<figura>> &e,  dir rayo, double& R, double& G, double& B, double &dist){
        shared_ptr<figura> actualFig = nullptr;
        point colP;
        bool colisiona = colision(c,e,rayo,actualFig,colP);
        dist += mod(c-colP);
        event_enum event;
        if(!colisiona){
            event = DEATH;
        }
        else{
            event = actualFig->evento();
        }
        double R_siguiente, G_siguiente, B_siguiente;

        if(colisiona && actualFig->isLight()){
            actualFig->getRGB(EMISSION, R,G,B);
        }
        else if(event == DEATH){
            R = 0;
            G = 0;
            B = 0;
        }
        else if(event == REFRACTION || event == REFLEXION || event == PHONG){
            dir dirNewRay;
            point nextPoint;
            dirNewRay = actualFig->nextRay(event, rayo, colP,nextPoint);
            if ( event == REFRACTION) {
                colP = nextPoint;
            }
            getRGB(colP,  e,  dirNewRay, R_siguiente, G_siguiente, B_siguiente, dist);
            if(event == PHONG){
                actualFig->phongColor(rayo,dirNewRay,colP,R,G,B);
            }
            else{
                actualFig->getRGB(event,R,G,B);
            }
            R = R*R_siguiente*abs(dot(-rayo,dirNewRay));
            G = G*G_siguiente*abs(dot(-rayo,dirNewRay));
            B = B*B_siguiente*abs(dot(-rayo,dirNewRay));
        }
        else{
            actualFig->getRGB(REFLEXION,R,G,B);
        }
    }

    double cFunc(const double v, const double min, const double max){
        return (max-min)*v + min;
    }
};

#endif // PATHTRACER_H
