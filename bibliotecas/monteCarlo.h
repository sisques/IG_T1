#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <iostream>
#include <math.h>
#include "camara.h"
#include <list>
#include <memory>
#include <limits>
#include "path_tracer.h"
#include "photonMap.h"
#include "photonMapper.h"

using namespace std;

// Clase para integrar el RGB correspondiente a un pixel de la imagen a generar
class monteCarlo
{
private:
    // La cámara de la escena
    camara cam;
    // Altura y anchura de la imagen, y número de rayos por pixel
    int height, wide, rays;
    // Corrección gamma a aplicar (tone map)
    double gamma;
    // Path tracer con el que se puede obtener el RGB de cada rayo
    pathTracer pT;
    // Photon map global y de cáusticas de la escena
    photonMap photonM, photonMC;
    // Si es photon mapping o path tracing
    bool photon = false;
    // Photon mapping con el que se puede obtener el RGB de cada rayo
    photonMapper pM;
    // Número máximo de rayos lanzados en photon mapping, máximo de fotones
    // de iluminación global y máximo de fotones de cáusticas

    // Devuelve un valor entre máximo y mínimo en base a v
    double cFunc(const double v, const double min, const double max){
        return (max-min)*v + min;
    }

public:

    // Constructor que pide la cámara, altura y anchura, el número de rayos por pixel y el gamma
    monteCarlo(camara c, int h, int w, int r, double g):cam(c),height(h),wide(w),rays(r),gamma(g){}
    // Constructor que pide la cámara, altura y anchura, el número de rayos por pixel, el gamma
    // y los photon maps global y de cáusticas
    monteCarlo(camara c, int h, int w, int r, double g, const photonMap &p, const photonMap &pc, int ppr)
            :cam(c),height(h),wide(w),rays(r),gamma(g),photonM(p), photonMC(pc),photon(true){
        photonM.setPhotonsPerRay(ppr);
        photonMC.setPhotonsPerRay(ppr);
    }

    // Destructor
    ~monteCarlo(){};

    // Halla las luces de la escena formada por las figuras en e
    list<shared_ptr<figura>> getLuces(const list<shared_ptr<figura>> &e){
        list<shared_ptr<figura>> luces;
        for(shared_ptr<figura> i:e){
            if(i->isLight()){
                luces.push_back(i);
            }
        }
        return luces;
    }

    // Devuelve el RGB resultante a la integración mediante el algoritmo montecarlo de los
    // caminos trazados para un pixel concreto
    void rtx(const list<shared_ptr<figura>> &e, const int &x, const int &y, int& R, int& G, int& B,
             const bool &luzPuntual){
        list<shared_ptr<figura>> luces = getLuces(e);
        int cX = y - (height/2);
        int cY = (wide/2) - x;
        // Haya los límites del pixel
        double minX = (1.0/(wide/2)) * cX*1.0;
        double maxX = (1.0/(wide/2)) * (cX*1.0+1.0);
        double maxY = (1.0/(height/2)) * cY*1.0;
        double minY = (1.0/(height/2)) * (cY*1.0-1.0);

        // Genera tantas coordenadas aleatorias dentro del pixel como número de rayos
        double a3[rays], b3[rays];
        for(int i = 0; i < rays;++i){
            a3[i] = ((double)rand()-1) / (RAND_MAX);
            b3[i] = ((double)rand()-1) / (RAND_MAX);
        }
        // Genera los rayos que van de la cámara a los puntos calculados
        double Xs[rays], Ys[rays];
        for(int i = 0; i < rays; ++i){
            Xs[i] = cFunc(a3[i], minX, maxX);
            Ys[i] = cFunc(b3[i], minY, maxY);
        }
        double r, g, b;
        double Rt = 0.0, Gt = 0.0, Bt = 0.0;
        double aux = 0.0;
        // Tantas veces como rayos
        for(int i = 0; i < rays; ++i){
            dir rayo = normalize(newDir(Xs[i],Ys[i],1));
            // Si es photon mapping se usa el photon mapper
            if (!photon) {
                pT.getRGB(cam.o, e, luces, rayo, r, g, b, luzPuntual);
            }
                // Sino se usa el path tracer
            else {
                pM.getRGB(cam.o, e, luces, photonM, photonMC, rayo, r, g, b, luzPuntual);
            }
            // Se va acumulando los valores obtenidos
            Rt += r;
            Gt += g;
            Bt += b;
        }
        // Se calcula la media y se aplica corrección gamma
        Rt = pow(Rt / rays,gamma);
        Gt = pow(Gt / rays,gamma);
        Bt = pow(Bt / rays,gamma);

        // Se multiplica por la resolución de color
        R = Rt * CR;
        G = Gt * CR;
        B = Bt * CR;

        // Si se pasa algún valor del máximo, se capa
        if (R < 0) { R = 0; }
        else if (R > MAX) { R = MAX; }
        if (G < 0) { G = 0; }
        else if (G > MAX) { G = MAX; }
        if (B < 0) { B = 0; }
        else if (B > MAX) { B = MAX; }
    }
};

#endif // MONTECARLO_H