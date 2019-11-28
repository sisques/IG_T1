#include "../bibliotecas/punto_direccion.h"
#include "../bibliotecas/camara.h"
#include "../bibliotecas/figuras.h"
#include "../bibliotecas/matrix.h"
#include "../bibliotecas/monteCarlo.h"
#include "../bibliotecas/ply_reader.h"
#include "../bibliotecas/globals.h"

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

    double brdfValues[] = {0.4,0.4};
    event_enum eventos[] = {REFRACTION, REFLEXION};

    materialProperties mp = materialProperties(false, false, eventos, brdfValues);
    list<shared_ptr<figura>> elementos;
    mp.setRGB(120, 120, 120);
    shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,1), newDir(0,0,-1), mp));
    shared_ptr<figura> suelo = make_shared<plano>(plano(newPoint(0,-0.5,0), newDir(0,1,0),mp));
    shared_ptr<figura> techo = make_shared<plano>(plano(newPoint(0,0.5,0), newDir(0,-1,0),mp));
    mp.setRGB(255, 0, 0);
    shared_ptr<figura> izquierda = make_shared<plano>(plano(newPoint(-0.5,0,0), newDir(1,0,0),mp));
    mp.setRGB(0, 255, 0);
    shared_ptr<figura> derecha = make_shared<plano>(plano(newPoint(0.5,0,0), newDir(-1,0,0),mp));

    materialProperties phong = materialProperties(false,0,0,255, false, eventos, brdfValues);
    shared_ptr<figura> esferaPhong = make_shared<esfera>(newPoint(0.25,-0.25,0.75), 0.25, phong);


    materialProperties especular_refracion = materialProperties(false,0,255,255, false, eventos, brdfValues);
    shared_ptr<figura> esferaEspecularRefracion = make_shared<esfera>(newPoint(-0.4,-0.4,0.9), 0.1, especular_refracion);

    materialProperties light = materialProperties(true,255,255,255, false, eventos, brdfValues);
    point p1 = newPoint(-0.25,0.5,0.25);
    point p2 = newPoint(0.25,0.5,0.25);
    point p3 = newPoint(-0.25,0.5,0.75);
    point p4 = newPoint(0.25,0.5,0.75);
    shared_ptr<figura> lght_src_1 = make_shared<triangulo>(triangulo(p1,p2,p3,light));
    shared_ptr<figura> lght_src_2 = make_shared<triangulo>(triangulo(p2,p3,p4,light));

    elementos.push_back(lght_src_1);
    elementos.push_back(lght_src_2);

    elementos.push_back(fondo);
    elementos.push_back(suelo);
    elementos.push_back(techo);
    elementos.push_back(izquierda);
    elementos.push_back(derecha);
    elementos.push_back(esferaPhong);
    elementos.push_back(esferaEspecularRefracion);
    /*
    shared_ptr<figura> limite = make_shared<plano>(plano(newPoint(0,0,0), newDir(0,0,-1), light));

    elementos.push_back(limite);
    */return elementos;
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
    h = 200;
    w = 200;
    rpp = 10;
    int threads = 1;
    if (threads > h || threads > w){
        cerr << "Numero de threads incompatible con la resolucion de la imagen" << endl;
        exit(5);
    }
    fOut = "test";
    string ruta = "/home/victor/gitRepos/IG_T1/imagenes/";

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

