#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <iostream>
#include <math.h>
#include "camara.h"
#include <list>
#include <memory>
#include <limits>
#include "path_tracer.h"

using namespace std;

class monteCarlo
{
private:
    camara cam;
    int height, wide, rays;
	double gamma;
    pathTracer pT;

    double cFunc(const double v, const double min, const double max){
        return (max-min)*v + min;
    }

public:
    monteCarlo(camara c, int h, int w, int r, double g):cam(c),height(h),wide(w),rays(r),gamma(g){}
    ~monteCarlo(){};


    void getRGB(point c, const list<shared_ptr<figura>> &e,  dir rayo, double& R, double& G, double& B){
        double t = 0;
        double distMin = numeric_limits<double>::max();
        double distActual = 0;
        bool colision;
        shared_ptr<figura> nearest;
        point colP;
        for( auto it = e.begin(); it != e.end(); ++it){
            shared_ptr<figura> f = *it;
            colision = f->intersection(rayo, c, t);
            if (colision) {
                point p = c + rayo * t;
                distActual = mod(c - p);
                if (distActual < distMin ) {
                    nearest = f;
                    distMin = distActual;
                    colP = p;
                }
            }

        }
        nearest->getRGB(REFLEXION,colP,R,G,B);
    }

	list<shared_ptr<figura>> getLuces(const list<shared_ptr<figura>> &e){
        list<shared_ptr<figura>> luces;
        for(shared_ptr<figura> i:e){
            if(i->isLight()){
                luces.push_back(i);
            }
        }
        return luces;
    }

    void rtx(const list<shared_ptr<figura>> &e, const int &x, const int &y, int& R, int& G, int& B,
			const bool &luzPuntual, const bool &basic){
		list<shared_ptr<figura>> luces = getLuces(e);
        int cX = y - (height/2);
        int cY = (wide/2) - x;
        double minX = (1.0/(wide/2)) * cX*1.0;
        double maxX = (1.0/(wide/2)) * (cX*1.0+1.0);
        double maxY = (1.0/(height/2)) * cY*1.0;
        double minY = (1.0/(height/2)) * (cY*1.0-1.0);
        double indirectL = 0;

        double a3[rays], b3[rays];
        for(int i = 0; i < rays;++i){
            a3[i] = ((double)rand()-1) / (RAND_MAX);
            b3[i] = ((double)rand()-1) / (RAND_MAX);
        }

        double Xs[rays], Ys[rays];
        for(int i = 0; i < rays; ++i){
            Xs[i] = cFunc(a3[i], minX, maxX);
            Ys[i] = cFunc(b3[i], minY, maxY);
        }
        double r, g, b;
        double Rt = 0.0, Gt = 0.0, Bt = 0.0;
        double aux = 0.0;
        for(int i = 0; i < rays; ++i){
            dir rayo = normalize(newDir(Xs[i],Ys[i],1));

            if (!basic) {
				pT.getRGB(cam.o, e, luces, rayo, r, g, b, luzPuntual);
            } else {
                this->getRGB(cam.o, e, rayo, r, g, b);
            }
            Rt += r;
            Gt += g;
            Bt += b;
        }
		
		Rt = pow(Rt / rays,gamma);
        Gt = pow(Gt / rays,gamma);
        Bt = pow(Bt / rays,gamma);
		
        R = Rt * CR;
        G = Gt * CR;
        B = Bt * CR;
		
		if (R < 0) { R = 0; }
        else if (R > MAX) { R = MAX; }
        if (G < 0) { G = 0; }
        else if (G > MAX) { G = MAX; }
        if (B < 0) { B = 0; }
        else if (B > MAX) { B = MAX; }
    }
};

#endif // MONTECARLO_H
