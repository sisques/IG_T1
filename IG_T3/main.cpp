#include "../bibliotecas/punto_direccion.h"
#include "../bibliotecas/camara.h"
#include "../bibliotecas/figuras.h"
#include "../bibliotecas/matrix.h"
#include "../bibliotecas/monteCarlo.h"
#include "../bibliotecas/ply_reader.h"
#include "../bibliotecas/perlinNoise.h"

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
#include <iomanip>

using namespace std;

camara c;


list<shared_ptr<figura>> setUpScene(){
    string file = "/home/victor/4o/IG/IG_T1/models/human.ply";
    int matrixSize = 3;
    double brdfValues[] = {0.0,0.0,0.0};
    Matrix m[matrixSize] ={rotateX(-M_PI/2), rotateY(M_PI/4 + M_PI/2), translate(0,-0.3,0.5)};
    materialProperties mp = materialProperties(false, brdfValues);
    list<shared_ptr<figura>> elementos; //= plyReader(file,&mp, m, matrixSize);


//cornell box
    //list<shared_ptr<figura>> elementos;


    //fondo, suelo y techo -> gris
    //pared izquierda -> roja
    //pared derecha -> verde
    //esfera 1 -> phong material
    //esfera 2 -> mezcla de especular perfecta y refracion perfecta
    mp.setRGB(255,0,0);
    string ruta = "/home/victor/4o/IG/IG_T1/textures/marto.ppm";
    shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,10000), newDir(0,-1,-1), IMAGE,ruta ,&mp));

    elementos.push_back(fondo);



    return elementos;
}

int lineasCompletadas = 0;

void generateScene( monteCarlo mc, const list<shared_ptr<figura>> e,
					const string fOut, const int hMin, const int hMax, const int w, const int h){
	int R,G,B;
	fstream flujoOut;
	flujoOut.open((fOut).c_str(), ios::out);
	for (int i = hMin; i <= hMax; ++i){
        for (int j = 0; j < w; j++){
            mc.rtx(e,i,j,R,G,B);
            flujoOut << R << " " << G << " " << B;
            flujoOut << "	";
        }
        lineasCompletadas++;
        double prog = (double)lineasCompletadas / (double)h;
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
    cin >> h;

    cout << "Introduce la anchura de la imagen deseada:" << endl;
    cin >> w;

	cout << "Introduce el numero de rayos por pixel deseado:" << endl;
    cin >> rpp;

    cout << "Introduce el nombre del fichero de salida:" << endl;
    cin >> fOut;
    */
    h = 100;
    w = 100;
    rpp = 10;
    int threads = 10;
    if (threads > h || threads > w){
        cerr << "Numero de threads incompatible con la resolucion de la imagen" << endl;
        exit(5);
    }
    fOut = "test";
    string ruta = "/home/victor/4o/IG/IG_T1/imagenes/";

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
                <<   "#MAX=255"  << endl
                <<  "# " << fOut << endl
                << w << " " << h << endl
                <<      "255"    << endl;
				
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

