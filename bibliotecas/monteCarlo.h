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
    pathTracer pT;

    double cFunc(const double v, const double min, const double max){
        return (max-min)*v + min;
    }

public:
    monteCarlo(camara c, int h, int w, int r):cam(c),height(h),wide(w),rays(r){}
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
        nearest->getRGB(REFLEXION,R,G,B);
    }


    void rtx(const list<shared_ptr<figura>> &e, const int x, const int y, int& R, int& G, int& B, bool basic){
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
				double dist = 0.0;
                pT.getRGB(cam.o, e, rayo, r, g, b, dist);
				r = pow(r, 0.05);//pow(dist,2.0);
				g = pow(g, 0.05);//pow(dist,2.0);
				b = pow(b, 0.05);//pow(dist,2.0);
            } else {
                this->getRGB(cam.o, e, rayo, r, g, b);
            }
            Rt += r;
            Gt += g;
            Bt += b;
        }
        R = Rt * CR / rays;
        G = Gt * CR / rays;
        B = Bt * CR / rays;
    }
};

#endif // MONTECARLO_H
