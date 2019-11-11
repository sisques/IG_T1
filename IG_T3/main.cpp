#include "bibliotecas/punto_direccion.h"
#include "bibliotecas/camara.h"
#include "bibliotecas/figuras.h"
#include "bibliotecas/tranformations.h"
#include "bibliotecas/monteCarlo.h"
#include "bibliotecas/ply_reader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <memory>
#include <limits>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

camara c;

list<shared_ptr<figura>> setUpScene(){
    string file = "C:\\Users\\BlueSac\\Desktop\\Tools\\WorspaceCodelite\\Practica3IG\\test.ply";
    list<shared_ptr<figura>> elementos = plyReader(file,c, 1);


    shared_ptr<figura> p1 = make_shared<esfera>(esfera(c, newPoint(-50,0,100), 25, 255,0, 0,1));
    shared_ptr<figura> p2 = make_shared<esfera>(esfera(c, newPoint(50,0,100), 25, 0,255, 0,1));
    shared_ptr<figura> p3 = make_shared<esfera>(esfera(c, newPoint(0,50,100), 25, 0,0, 255,3));
    shared_ptr<figura> p4 = make_shared<esfera>(esfera(c, newPoint(0,-50,100), 25, 255,0, 255,3));


    shared_ptr<figura> fondo = make_shared<plano>(plano(c, newPoint(0,0,500), newDir(0,0,-1), 0,0,0, 3));
    elementos.push_back(p1);
    elementos.push_back(p2);
    elementos.push_back(p3);
    elementos.push_back(p4);
    elementos.push_back(fondo);



    return elementos;
}

void generateScene( monteCarlo mc, const list<shared_ptr<figura>> e,
					const string fOut, const int hMin, const int hMax, const int w){
	int R,G,B;
	fstream flujoOut;
	flujoOut.open((fOut).c_str(), ios::out);
	for (int i = hMin; i <= hMax; ++i){
        for (int j = 0; j < w; j++){
            mc.rtx(e,i,j,R,G,B);
            flujoOut << R << " " << G << " " << B;
            flujoOut << "	";
        }
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
    cin >> h;

    cout << "Introduce la anchura de la imagen deseada:" << endl;
    cin >> w;

	cout << "Introduce el numero de rayos por pixel deseado:" << endl;
    cin >> rpp;

    cout << "Introduce el nombre del fichero de salida:" << endl;
    cin >> fOut;
    */
    h = 1024;
    w = 1024;
    rpp = 1;
    fOut = "test";
    string ruta = "C:\\Users\\BlueSac\\Desktop\\Tools\\WorspaceCodelite\\Practica3IG\\";
   
	list<shared_ptr<figura>> e = setUpScene();
	thread th[10];
	int hMin = 0, hMax = - 1 + h/10;
	for(int i = 0; i < 10;++i){
		monteCarlo mc(c,h,w,10);
		if(i == 9){hMax = h - 1;}
		th[i] = thread(&generateScene, mc, e, ruta+to_string(i), hMin, hMax, w);
		hMin += h/10;
		hMax += h/10;
	}
	for(int i = 0; i < 10;++i){th[i].join();}
	
	fOut = fOut + ".ppm";
    fstream flujoOut;
    string fOutAux = ruta + fOut;
    flujoOut.open((fOutAux).c_str(), ios::out);
    flujoOut    <<      "P3"     << endl
                <<   "#MAX=255"  << endl
                <<  "# " << fOut << endl
                << w << " " << h << endl
                <<      "255"    << endl;
				
	for(int i = 0; i < 10;++i){
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

