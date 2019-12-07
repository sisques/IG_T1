#include "bibliotecas/punto_direccion.h"
#include "bibliotecas/camara.h"
#include "bibliotecas/figuras.h"
#include "bibliotecas/matrix.h"
#include "bibliotecas/monteCarlo.h"
#include "bibliotecas/ply_reader.h"
#include "bibliotecas/globals.h"
#include "bibliotecas/phong.h"

#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iomanip>

using namespace std;

camara c;

list<shared_ptr<figura>> setUpScene(){

    double reflection[] = {0,0.0,0.9};
    double refraction[] = {0.0,0.9,0.0};
    double wall[] = {0.90,0.0,0.0};
    event_enum eventos[] = {PHONG, REFRACTION, REFLEXION };


    materialProperties mp = materialProperties(false, eventos, wall);
	mp.setAlfa(0);
    materialProperties light = materialProperties(true, eventos, wall,1);
	light.setKs(255,255,255);
    materialProperties reflexion = materialProperties(false, eventos, reflection,1.33);
	reflexion.setKd(0,0,0);
	reflexion.setKs(255,255,255);
	reflexion.setKdPhong(255,255,255);
	reflexion.setKsPhong(255,255,255);
    materialProperties refraccion = materialProperties(false, eventos, refraction,1.33);
	refraccion.setKd(255,255,255);
	refraccion.setKs(0,0,0);
	refraccion.setKdPhong(0,0,0);
	refraccion.setKsPhong(0,0,0);


    list<shared_ptr<figura>> elementos;
	mp.setKd(120, 120, 120);
	mp.setKs(120, 120, 120);
	mp.setKdPhong(120, 120, 120);
	mp.setKsPhong(120, 120, 120);
    shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,1), newDir(0,0,-1), mp));
    shared_ptr<figura> suelo = make_shared<plano>(plano(newPoint(0,-0.5,0), newDir(0,1,0),mp));
    shared_ptr<figura> techo = make_shared<plano>(plano(newPoint(0,0.5,0), newDir(0,-1,0),light));
    shared_ptr<figura> espalda = make_shared<plano>(plano(newPoint(0,0,-0.1), newDir(0,0,1),mp));
    mp.setKd(255,0,0);
	mp.setKs(255,0,0);
	mp.setKdPhong(255,0,0);
	mp.setKsPhong(255,0,0);
    shared_ptr<figura> izquierda = make_shared<plano>(plano(newPoint(-0.5,0,0), newDir(1,0,0),mp));
    mp.setKd(0,255,0);
	mp.setKs(0,255,0);
	mp.setKdPhong(0,255,0);
	mp.setKsPhong(0,255,0);
    shared_ptr<figura> derecha = make_shared<plano>(plano(newPoint(0.5,0,0), newDir(-1,0,0),mp));
    shared_ptr<figura> ESFERArefraccion = make_shared<esfera>(newPoint(0.0,0.0,0.3), 0.15, refraccion);
    shared_ptr<figura> ESFERAreflexion = make_shared<esfera>(newPoint(0,0,0.7), 0.2, reflexion);


    //elementos.push_back(espalda);
    elementos.push_back(fondo);
    elementos.push_back(suelo);
    elementos.push_back(techo);
    elementos.push_back(izquierda);
    elementos.push_back(derecha);
    elementos.push_back(ESFERArefraccion);
    elementos.push_back(ESFERAreflexion);



    double brdfValues2[] = {0.9,0.0,0.0};
    materialProperties limit = materialProperties(false, eventos, brdfValues2,0);
	limit.setKd(0,0,0);
	limit.setKs(0,0,0);
	limit.setKdPhong(0,0,0);
	limit.setKsPhong(0,0,0);
    shared_ptr<figura> limite = make_shared<plano>(plano(newPoint(0,0,0), newDir(0,0,-1), limit));

	elementos.push_back(limite);
    return elementos;
}

int completadas = 0;

void generateScene( monteCarlo mc, const list<shared_ptr<figura>> &e,
					const string &fOut, const int hMin, const int hMax, const int w, const int h){
	int R,G,B;
	fstream flujoOut;
	flujoOut.open((fOut).c_str(), ios::out);
	for (int i = hMin; i <= hMax; ++i){
        for (int j = 0; j < w; j++){
            mc.rtx(e,i,j,R,G,B, false);
            flujoOut << R << " " << G << " " << B;
            flujoOut << "	";
        }
        completadas++;
        double prog = (double)completadas / (double)h;
        cout << "\r" << "progreso: " << fixed << setprecision(2) << setw(6) << prog*100 << "%" << flush;
    }
	flujoOut.close();
}

int main(){
    /*
     * camara       x   y   z
     *      o   =   0   0   0
     *      l   =   1   0   0
     *      u   =   0   1   0
     *      f   =   0   0   1
     *
     *      aspect ratio        1:1
     */
	point origenCamara = newPoint(0,0,0);
    dir anchuraPlano = newDir(1,0,0);
    dir alturaPlano = newDir(0,1,0);
    dir distanciaPlano = newDir(0,0,1);


	c =  newCamara(origenCamara, alturaPlano, anchuraPlano, distanciaPlano);
    double rpp = 0, w = 0, h = 0;
    string fOut = "";

    /*cout << "Introduce la altura de la imagen deseada:" << endl;
     *
    cin >> h;

    cout << "Introduce la anchura de la imagen deseada:" << endl;
    cin >> w;

	cout << "Introduce el numero de rayos por pixel deseado:" << endl;
    cin >> rpp;

    cout << "Introduce el nombre del fichero de salida:" << endl;
    cin >> fOut;
    */
    h = 500;
    w = 500;
    rpp = 10;
    int threads = 10;
    if (threads > h || threads > w){
        cerr << "Numero de threads incompatible con la resolucion de la imagen" << endl;
        exit(5);
    }
    fOut = "test";
    string ruta = "C:\\Users\\BlueSac\\Desktop\\Tools\\WorspaceCodelite\\Practica3IG\\";

	list<shared_ptr<figura>> e = setUpScene();
	thread th[threads];
	int hMin = 0, hMax = - 1 + h/threads;
	for(int i = 0; i < threads;++i){
		monteCarlo mc(c,h,w,rpp);
		if(i == threads-1){hMax = h - 1;}
		th[i] = thread(&generateScene, mc, e, ruta+to_string(i), hMin, hMax, w, h);
		hMin += h/threads;
		hMax += h/threads;
	}
	for(int i = 0; i < threads;++i){th[i].join();}
	
	fOut = fOut + ".ppm";
    fstream flujoOut;
    string fOutAux = ruta + fOut;
    flujoOut.open((fOutAux).c_str(), ios::out);
    flujoOut    <<      "P3"     << endl
                <<   "#MAX="  << MAX << endl
                <<  "# " << fOut << endl
                << w << " " << h << endl
                <<      CR      << endl;
				
	for(int i = 0; i < threads;++i){
		string file = ruta+to_string(i);
		fstream flujoAux;
		flujoAux.open((file).c_str(), ios::in);
		char c;
		while(!flujoAux.eof()){
			flujoAux >> std::noskipws >> c;
			flujoOut << c;
		}
		flujoAux.close();
		remove((file).c_str());
	}
	flujoOut.close();
}

