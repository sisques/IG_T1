#include "../bibliotecas/figuras.h"
#include "../bibliotecas/camara.h"
#include "../bibliotecas/monteCarlo.h"
#include "../bibliotecas/punto_direccion.h"
#include "../bibliotecas/ply_reader.h"
#include "../bibliotecas/scenes.h"


#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

using namespace std;

camara c;


int completadas = 0;



void generateScene( monteCarlo mc, const list<shared_ptr<figura>> &e,
                    const string &fOut, const int &hMin, const int &hMax, const int &w, const int &h){
    int R,G,B;
    fstream flujoOut;
    flujoOut.open((fOut).c_str(), ios::out);
    for (int i = hMin; i <= hMax; ++i){
        for (int j = 0; j < w; j++){
            mc.rtx(e,i,j,R,G,B,false, false);
            flujoOut << R << " " << G << " " << B << " ";
        }
        completadas++;
        double prog = (double)completadas / (double)h;
        cout << "\r" << "progreso: " << fixed << setprecision(2) << setw(6) << prog*100 << "%" << flush;
    }
    flujoOut.close();
}

int main(){

    time_t tStart = time (NULL);;
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
    h = 128;
    w = 128;
    rpp = 128;
    int threads = 4;
    if (threads > h || threads > w){
        cerr << "Numero de threads incompatible con la resolucion de la imagen" << endl;
        exit(5);
    }
    fOut = "test";
    string ruta = "/home/victor/gitRepos/IG_T1/imagenes/";


    list<shared_ptr<figura>> e = scene_4();
    thread th[threads];
    int hMin = 0, hMax = - 1 + h/threads;

    for(int i = 0; i < threads;++i){
        camara c2 = c;
        monteCarlo mc(c2,h,w,rpp);
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

    time_t tEnd = time (NULL);;

    cout << endl;
    cout << "segundos en ejecución -> " << tEnd - tStart << endl;


}
