#include "bibliotecas/punto_direccion.h"
#include "bibliotecas/camara.h"
#include "bibliotecas/figuras.h"
#include "bibliotecas/matrix.h"
#include "bibliotecas/monteCarlo.h"
#include "bibliotecas/ply_reader.h"
#include "bibliotecas/globals.h"
#include "bibliotecas/photonMapper.h"

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

list<shared_ptr<figura>> set_room(){

    list<shared_ptr<figura>> elementos;
	
	double phong[] =        {0.90,0.0,0.0};
    event_enum eventos[] =  {PHONG, REFRACTION, REFLEXION };

    materialProperties floor = materialProperties(false, eventos, phong);
    floor.setKdPhong(216,216,216);
    floor.setKsPhong(216,216,216);
	
    materialProperties left = materialProperties(false, eventos, phong);
    left.setKdPhong(216,21,21);
    left.setKsPhong(216,21,21);

    materialProperties right = materialProperties(false, eventos, phong);
    right.setKdPhong(21,216,21);
    right.setKsPhong(21,216,21);

    materialProperties luz = materialProperties(true, eventos, phong);
    luz.setKs(255,255,255);
    luz.setAlfa(0);
	
    shared_ptr<figura> floor_ = make_shared<plano>(plano(newPoint(0,0,1), newDir(0,0,-1),floor));
    shared_ptr<figura> ceil_ = make_shared<plano>(plano(newPoint(0,0.5,0), newDir(0,-1,0),floor));
    shared_ptr<figura> back_ = make_shared<plano>(plano(newPoint(0,-0.5,0), newDir(0,1,0),floor));
    shared_ptr<figura> left_ = make_shared<plano>(plano(newPoint(-0.5,0,0), newDir(1,0,0),left));
    shared_ptr<figura> right_ = make_shared<plano>(plano(newPoint(0.5,0,0), newDir(-1,0,0),right));

    shared_ptr<figura> puntoLuz_1 = make_shared<punto>(punto(newPoint(0,0.49,0.5), luz));
    elementos.push_back(puntoLuz_1);
    elementos.push_back(floor_);
    elementos.push_back(ceil_);
    elementos.push_back(back_);
    elementos.push_back(left_);
    elementos.push_back(right_);
	
	double brdfValues2[] = {0.9,0.0,0.0};
    materialProperties limit = materialProperties(false, eventos, brdfValues2,0);
	limit.setKdPhong(0,0,0);
	limit.setKsPhong(0,0,0);
    shared_ptr<figura> limite = make_shared<plano>(plano(newPoint(0,0,0), newDir(0,0,1), limit));
	elementos.push_back(limite);

    return elementos;
}

// Devuelve una escena medianamente compleja
list<shared_ptr<figura>> setUpSceneCool(){
	// Definición de materiales
    double reflection[] = {0,0.0,0.9};
    double refraction[] = {0.0,0.9,0.0};
    double wall[] = {0.90,0.0,0.0};
	double l[] = {1.0};
    event_enum eventos[] = {PHONG, REFRACTION, REFLEXION };
	event_enum eventos2[] = {EMISSION};
    materialProperties mp = materialProperties(false, eventos, wall);
	materialProperties PR = materialProperties(false, eventos,wall);
	mp.setAlfa(0);
    materialProperties light = materialProperties(true, eventos2, l);
	light.setKs(255,255,255);
    materialProperties reflexion = materialProperties(false, eventos, reflection);
	//reflexion.setKd(0,0,0);
	reflexion.setKs(255,255,255);
	reflexion.setKdPhong(255,255,255);
	reflexion.setKsPhong(255,255,255);
	reflexion.setAlfa(10);
    materialProperties refraccion = materialProperties(false, eventos, refraction,1.3);
	refraccion.setKd(0,255,255);
	refraccion.setKs(0,255,255);
	refraccion.setKdPhong(0,255,255);
	refraccion.setKsPhong(0,255,255);

	// Definición de geometrías
    list<shared_ptr<figura>> elementos;
	shared_ptr<figura> puntoLuz = make_shared<punto>(punto(newPoint(0,0.49,0.5), light));
	//mp.setKd(120, 120, 120);
	//mp.setKs(120, 120, 120);
	mp.setKdPhong(170, 170, 170);
	mp.setKsPhong(120, 120, 120);
	shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,1), newDir(0,0,-1), mp));
	shared_ptr<figura> suelo = make_shared<plano>(plano(newPoint(0,-0.5,0), newDir(0,1,0),mp));
    shared_ptr<figura> techo = make_shared<plano>(plano(newPoint(0,0.5,0), newDir(0,-1,0),mp));
    //mp.setKd(255,0,0);
	//mp.setKs(255,0,0);
	mp.setKdPhong(255,0,0);
	mp.setKsPhong(120,0,0);
    shared_ptr<figura> izquierda = make_shared<plano>(plano(newPoint(-0.5,0,0), newDir(1,0,0), mp));
    //mp.setKd(0,255,0);
	//mp.setKs(0,255,0);
	mp.setKdPhong(0,255,0);
	mp.setKsPhong(0,120,0);
    shared_ptr<figura> derecha = make_shared<plano>(plano(newPoint(0.5,0,0), newDir(-1,0,0),mp));
    shared_ptr<figura> ESFERAreflexion = make_shared<esfera>(newPoint(0.3,-0.3,0.9), 0.2, reflexion);
	shared_ptr<figura> ESFERAreflexion2 = make_shared<esfera>(newPoint(0.3,-0.1,0.9), 0.2, reflexion);
	shared_ptr<figura> ESFERAreflexion3 = make_shared<esfera>(newPoint(-0.3,-0.15,0.7), 0.2, reflexion);
	shared_ptr<figura> ESFERAreflexion4 = make_shared<esfera>(newPoint(-0.3,-0.3,0.7), 0.2, reflexion);
	PR.setKd(0,255,255);
	PR.setKs(0,255,255);
	PR.setKdPhong(0,255,255);
	PR.setKsPhong(0,255,255);
	PR.setAlfa(0);
	shared_ptr<figura> ESFERArefraccion = make_shared<esfera>(newPoint(0,-0.1,0.7), 0.2, refraccion);
	mp.setKdPhong(0,0,255);
	mp.setKsPhong(0,0,120);
	mp.setAlfa(4);
	shared_ptr<figura> ESFERAphong2 = make_shared<esfera>(newPoint(0.3,0.2,0.9), 0.1, mp);
	shared_ptr<figura> ESFERAphong3 = make_shared<esfera>(newPoint(-0.3,0.15,0.7), 0.1, mp);
	shared_ptr<figura> ESFERAphong4 = make_shared<esfera>(newPoint(0,0.35,0.8), 0.1, mp);
	shared_ptr<figura> ESFERAphong5 = make_shared<esfera>(newPoint(-0.15,0.275,0.75), 0.1, mp);
	shared_ptr<figura> ESFERAphong6 = make_shared<esfera>(newPoint(0.15,0.275,0.85), 0.1, mp);

    elementos.push_back(puntoLuz);
	elementos.push_back(fondo);
    elementos.push_back(suelo);
    elementos.push_back(techo);
    elementos.push_back(izquierda);
    elementos.push_back(derecha);
    elementos.push_back(ESFERArefraccion);
	elementos.push_back(ESFERAreflexion);
	elementos.push_back(ESFERAreflexion2);
	elementos.push_back(ESFERAreflexion3);
	elementos.push_back(ESFERAreflexion4);
	elementos.push_back(ESFERArefraccion);
	elementos.push_back(ESFERAphong2);
	elementos.push_back(ESFERAphong3);
	elementos.push_back(ESFERAphong4);
	elementos.push_back(ESFERAphong5);
	elementos.push_back(ESFERAphong6);

    double brdfValues2[] = {0.9,0.0,0.0};
    materialProperties limit = materialProperties(false, eventos, brdfValues2,0);
	limit.setKdPhong(0,0,0);
	limit.setKsPhong(0,0,0);
	mp.setKdPhong(0,0,0);
	mp.setKsPhong(0,0,0);
    shared_ptr<figura> limite = make_shared<plano>(plano(newPoint(0,0,0), newDir(0,0,1), mp));
	elementos.push_back(limite);
	
    return elementos;
}

list<shared_ptr<figura>> scene_1(){

    double reflection[] =   {0,0.0,0.90};
    double refraction[] =   {0,0.90,0.0};
    double phong[] =        {0.90,0.0,0.0};
    event_enum eventos[] =  {PHONG, REFRACTION, REFLEXION };


    list<shared_ptr<figura>> elementos = set_room();

    materialProperties mirror = materialProperties(false, eventos, reflection);
    materialProperties glass = materialProperties(false, eventos, refraction, 1.5);
    glass.setKd(255,255,255);
    mirror.setKs(255,255,255);

    shared_ptr<figura> sphere1 = make_shared<esfera>(newPoint(0.25,-0.15, 0.75), 0.15, glass);
    shared_ptr<figura> sphere2 = make_shared<esfera>(newPoint(-0.25,-0.25,0.75), 0.15, mirror);

    elementos.push_back(sphere1);
    elementos.push_back(sphere2);

    return elementos;
}

list<shared_ptr<figura>> scene_2(){

    double reflection[] =   {0,0.0,0.90};
    double refraction[] =   {0,0.90,0.0};
    double phong[] =        {0.90,0.0,0.0};
    event_enum eventos[] =  {PHONG, REFRACTION, REFLEXION };

    materialProperties red = materialProperties(false, eventos, phong);
    red.setKdPhong(216,21,21);
    red.setKsPhong(216,21,21);
	
	materialProperties white = materialProperties(false, eventos, phong);
    white.setKdPhong(216,216,216);
    white.setKsPhong(216,216,216);

    list<shared_ptr<figura>> elementos = set_room();

    shared_ptr<figura> sphere1 = make_shared<esfera>(newPoint(0,-0.35, 0.85), 0.15, white);
    shared_ptr<figura> sphere2 = make_shared<esfera>(newPoint(-0.25,-0.35, 0.5), 0.15, red);
	shared_ptr<figura> sphere3 = make_shared<esfera>(newPoint(0.25,-0.35, 0.7), 0.15, white);

    elementos.push_back(sphere1);
    elementos.push_back(sphere2);
	elementos.push_back(sphere3);

    return elementos;
}

list<shared_ptr<figura>> scene_4(){

    double reflection[] =   {0,0.0,0.90};
    double refraction[] =   {0,0.90,0.0};
    double phong[] =        {0.90,0.0,0.0};
    event_enum eventos[] =  {PHONG, REFRACTION, REFLEXION };

    materialProperties orange = materialProperties(false, eventos, phong);
    orange.setKdPhong(216,153,5);
    orange.setKsPhong(216,153,5);

    list<shared_ptr<figura>> elementos = set_room();

    materialProperties glass = materialProperties(false, eventos, refraction, 1.5);
    glass.setKd(216,216,216);

    shared_ptr<figura> sphere1 = make_shared<esfera>(newPoint(0,-0.2, 0.7), 0.3, glass);
    shared_ptr<figura> sphere2 = make_shared<esfera>(newPoint(0,-0.2, 0.7), 0.15, orange);

    elementos.push_back(sphere1);
    elementos.push_back(sphere2);

    return elementos;
}

// Devuelve diversas escenas según id
list<shared_ptr<figura>> setUpScene(int id){
	if(id  == 1){
		return scene_1();
	}
	else if(id == 2){
		return scene_2();
	}
	else if(id == 4){
		return scene_4();
	}
	else{
		return setUpSceneCool();
	}
}

int completadas = 0;

// Genera un fragmento de la imagen de la escena
void generateScene( monteCarlo mc, const list<shared_ptr<figura>> &e,
					const string &fOut, const int &hMin, const int &hMax,
					const int &w, const int &h, const int &luzPuntual){
						
	int R,G,B;
	fstream flujoOut;
	flujoOut.open((fOut).c_str(), ios::out);
	for (int i = hMin; i <= hMax; ++i){
        for (int j = 0; j < w; j++){
            mc.rtx(e,i,j,R,G,B, luzPuntual);
            flujoOut << R << " " << G << " " << B;
            flujoOut << "	";
        }
        completadas++;
        double prog = (double)completadas / (double)h;
        cout << "\r" << "progreso: " << fixed << setprecision(2) << setw(6) << prog*100 << "%" << flush;
    }
	flujoOut.close();
}

int main(int argc, char *argv[]){
	// Comrpobación del número de argumentos
	if(argc != 5){
		cout << "Parametros: rayos por pixel, anchura imnagen, altura imagen, nombre imagen" << endl;
		return 0;
	}
	// Si se usa el algoritmo de photon mapping o path tracer
	bool photOn = true;
	// Identificador de la escena, de 1 a 5
	int nScene = 5;
	
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
	
	// Parámetros path tracer
	// Solo se soporta 1x1 de aspect ratio
    double rpp = atoi(argv[1]), w = max(atoi(argv[2]), atoi(argv[3])), h = w;
	int maxRays, maxPhotonsGlobal, maxPhotonsCaustics, photonsPerRay;

	// Si la escena contiene luces puntuales
	bool luzPuntual = true;
	
	// Photon maps para iluminación global y cáusticas
	photonMap pm, pmC;
	
	double gamma = 0.2;
	
	// Parámetros para el photon map
	maxRays = 1000000;
	maxPhotonsGlobal = 1000000;
	maxPhotonsCaustics = 1000000;
	photonsPerRay =  500;

    int threads = 4;
    if (threads > h || threads > w){
        cerr << "Numero de threads incompatible con la resolucion de la imagen" << endl;
        exit(5);
    }

    string ruta = argv[4];

	// Preparamos la escena
	list<shared_ptr<figura>> e = setUpScene(nScene);
	
	thread th[threads];
	int hMin = 0, hMax = - 1 + h/threads;
	
	monteCarlo mc(c,h,w,rpp, gamma);
	photonMapper aux(maxRays, maxPhotonsGlobal, maxPhotonsCaustics);
	// Si se usa photon mapping se genera el mapa de photones para iluminación global
	// y de cáusticas
	if(photOn){
		aux.generatePhotonMap(pm, pmC, e,mc.getLuces(e));
		cout << "Se ha generado el photon map"<< endl;
		cout << "Photones de iluminacion global: " << pm.size() << endl;
		cout << "Photones de cáusticas: " << pmC.size() << endl;
	}
	// Dividimos el trabajo
	for(int i = 0; i < threads;++i){
		camara c2 = c;
		// Si es path tracing
		mc = monteCarlo(c2,h,w,rpp, gamma);
		// Si es photon mapping
		if(photOn){
			mc = monteCarlo(c2,h,w,rpp, gamma, photonMap(pm.generateTreeAux(),pm.size()), 
					photonMap(pmC.generateTreeAux(),pmC.size()), photonsPerRay);
		}
		if(i == threads-1){hMax = h - 1;}
		th[i] = thread(&generateScene, mc, e, ruta+to_string(i), hMin, hMax, w, h, luzPuntual);
		hMin += h/threads;
		hMax += h/threads;
		cout << "Se ha creado el thread " << i+1 << endl;
	}
	// Vaciamos los photon maps auxiliares
	pm.clear();
	pmC.clear();
	for(int i = 0; i < threads;++i){th[i].join();}
	
	fstream flujoOut;
    string fOutAux = ruta;
    flujoOut.open((fOutAux).c_str(), ios::out);
    flujoOut    <<      "P3"     << endl
                <<   "#MAX="  << MAX << endl
                <<  "# " << fOutAux << endl
                << w << " " << h << endl
                <<      CR      << endl;
	
	// Juntamos los fragmentos de la imagen
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

