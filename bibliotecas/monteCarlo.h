#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <iostream>
#include <math.h>
#include "camara.h"
#include <list>
#include <memory>
#include <limits>

using namespace std;

class monteCarlo
{
private:
	camara cam;
	int height, wide, rays;
	
	void getRGB(camara c, list<shared_ptr<figura>> e,  dir rayo, int& R, int& G, int& B, int x, int y){
		double t = 0;
		double delta = 0;
		double distMin = numeric_limits<double>::max();
		double distActual = 0;
		bool colision;
		shared_ptr<figura> nearest;
		point colP;
		for( auto it = e.begin(); it != e.end(); ++it){
			shared_ptr<figura> f = *it;
			colision = f->intersection(rayo, c.o, t, delta);
			if (colision) {
				point p = c.o + rayo * (t - delta);
				distActual = mod(c.o - p);
				if (distActual < distMin) {
					nearest = f;
					distMin = distActual;
					colP = p;
				}
			}
		}
		R = nearest->getR(colP);
		G = nearest->getG(colP);
		B = nearest->getB(colP);
	}
	
	double cFunc(const double v, const double min, const double max){
		return (max-min)*v + min;
	}
	
public:
	monteCarlo(camara c, int h, int w, int r):cam(c),height(h),wide(w),rays(r){}
	~monteCarlo(){};
	
	void rtx(const list<shared_ptr<figura>> e, const int x, const int y, int& R, int& G, int& B){
		int cX = y - (height/2);
		int cY = (wide/2) - x;
		double minX = (1.0/(wide/2)) * cX*1.0;
		double maxX = (1.0/(wide/2)) * (cX*1.0+1.0);
		double maxY = (1.0/(height/2)) * cY*1.0;
		double minY = (1.0/(height/2)) * (cY*1.0-1.0);
		
		double a3[rays], b3[rays];
		for(int i = 0; i < rays;++i){
			a3[i] = (double)rand() / (RAND_MAX+1);
			b3[i] = (double)rand() / (RAND_MAX+1);
		}
		
		double Xs[rays], Ys[rays];
		for(int i = 0; i < rays; ++i){
			Xs[i] = cFunc(a3[i], minX, maxX);
			Ys[i] = cFunc(b3[i], minY, maxY);
		}
		int r, g, b;
		double Rt = 0.0, Gt = 0.0, Bt = 0.0;
		for(int i = 0; i < rays; ++i){
			dir rayo = newDir(Xs[i],Ys[i],1);
			getRGB(cam, e, rayo, r, g, b, x, y);
			Rt += r; // /p(a3[i])*p(b3[i])
			Gt += g; // /p(a3[i])*p(b3[i]) 
			Bt += b; // /p(a3[i])*p(b3[i])
		}
		
		R = Rt / rays;
		G = Gt / rays;
		B = Bt / rays;
	}
};

#endif // MONTECARLO_H
