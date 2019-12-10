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

    bool colision( const point &c, const list<shared_ptr<figura>> &e, const dir &rayo, shared_ptr<figura> &fig, point &col) const {
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

    point getRGB( const point &inputPoint,const list<shared_ptr<figura>> &scene,const dir &inputRay ) const {

        shared_ptr<figura> actualFig = nullptr;
        point puntoColision;
        bool colisiona = false;
        event_enum event;
        double R_actual = 0, G_actual = 0, B_actual = 0;
        double R_acumulado = 0, G_acumulado = 0, B_acumulado = 0, dist = 0;

        colisiona = colision(inputPoint,scene,inputRay,actualFig,puntoColision);
        dir dirNewRay = inputRay;
        point nextPoint = inputPoint;
        dir rayoAnterior = dirNewRay;


        while ( colisiona && event != DEATH) {

            dist += mod(nextPoint-puntoColision);

            event = actualFig->evento();

            if ( colisiona && actualFig -> isLight() ) {
                actualFig -> getRGB(EMISSION, R_actual, G_actual, B_actual);
            } else if ( event == DEATH ) {
                R_actual = 0; G_actual = 0; B_actual = 0;
            } else if(event == REFRACTION || event == REFLEXION || event == PHONG){

                rayoAnterior = dirNewRay;

                dirNewRay = actualFig->nextRay(event, rayoAnterior, puntoColision,nextPoint);

                if ( event == REFRACTION) {
                    puntoColision = nextPoint;
                }


                if (event == PHONG) {
                    actualFig -> phongColor(rayoAnterior, dirNewRay, puntoColision, R_actual, G_actual, B_actual );
                } else {
                    actualFig -> getRGB(event, R_actual, G_actual, B_actual);
                }

                R_acumulado = R_actual*R_acumulado*abs(dot(-rayoAnterior,dirNewRay));
                G_acumulado = G_actual*G_acumulado*abs(dot(-rayoAnterior,dirNewRay));
                B_acumulado = B_actual*B_acumulado*abs(dot(-rayoAnterior,dirNewRay));
            } else {
                //ESTO PQ SAUL PQ E DIME PQ
                actualFig->getRGB(REFLEXION, R_actual, G_actual, B_actual);
            }

            colisiona = colision(nextPoint,scene,dirNewRay,actualFig,puntoColision);
        }


        return newPoint(R_acumulado,G_acumulado,B_acumulado,dist);
    }

    double cFunc(const double &v, const double &min, const double &max){
        return (max-min)*v + min;
    }
};

#endif // PATHTRACER_H
