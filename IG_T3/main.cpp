#include "../bibliotecas/ply_reader.h"


#include "../bibliotecas/punto_direccion.h"
#include "../bibliotecas/camara.h"
#include "../bibliotecas/figuras.h"
#include "../bibliotecas/tranformations.h"


#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <memory>
#include <limits>
using namespace std;


dir monteCarlo(camara c, double altura, double anchura, int numRayo, int rpp){
    point origen = c.o;
    point final = newPoint(anchura, altura, c.f.z);
    return final - origen;

}


void rtx( camara c, list<shared_ptr<figura>> e,  dir rayo, int& R, int& G, int& B){
    double t = 0;
    double delta = 0;
    double distMin = numeric_limits<double>::max();
    double distActual = 0;
    bool colision;
    shared_ptr<figura> nearest;


    for( auto it = e.begin(); it != e.end(); ++it){
        shared_ptr<figura> f = *it;
        colision = f->intersection(rayo, c.o, t, delta);
        if (colision) {
            point p = c.o + rayo * (t - delta);
            distActual = mod(c.o - p);
            if (distActual < distMin) {
                nearest = f;
                distMin = distActual;
            }
        }
    }
    R = nearest->getR();
    G = nearest->getG();
    B = nearest->getB();
}




list<shared_ptr<figura>> setUpScene(){
    //string file = "/home/victor/4o/IG/IG_T1/models/test.ply";
    list<shared_ptr<figura>> elementos/* = plyReader(file)*/;


    shared_ptr<figura> p1 = make_shared<esfera>(esfera(newPoint(-50,0,100), 25, 255,255, 255));
    shared_ptr<figura> p2 = make_shared<esfera>(esfera(newPoint(50,0,100), 25, 255,255, 255));
    shared_ptr<figura> p3 = make_shared<esfera>(esfera(newPoint(0,50,100), 25, 255,255, 255));
    shared_ptr<figura> p4 = make_shared<esfera>(esfera(newPoint(0,-50,100), 25, 255,255, 255));


    shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,500), newDir(0,0,-1), 0,0,0));
    elementos.push_back(p1);
    elementos.push_back(p2);
    elementos.push_back(p3);
    elementos.push_back(p4);
    elementos.push_back(fondo);



    return elementos;
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


	camara c =  newCamara(origenCamara, alturaPlano, anchuraPlano, distanciaPlano);
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



    string ruta = "/home/victor/4o/IG/IG_T1/imagenes/";
    fOut = fOut + ".ppm";
    fstream flujoOut;
    string fOutAux = ruta + fOut;
    flujoOut.open((fOutAux).c_str(), ios::out);
    flujoOut    <<      "P3"     << endl
                <<   "#MAX=255"  << endl
                <<  "# " << fOut << endl
                << w << " " << h << endl
                <<      "255"    << endl;


    int R = 0, G = 0, B = 0;
    dir rayo;



    list<shared_ptr<figura>> e = setUpScene();


    int _R, _G, _B;
    double altura;
    double anchura;
    for (int i = h/2 ; i > -h/2; i--){
        for (int j = -w/2; j < w/2; j++){
            _R = 0;
            _G = 0;
            _B = 0;
            anchura = j/(w/2);
            altura = i/(h/2);

            for (int k = 0; k < rpp; k++){
                rayo = monteCarlo(c,altura,anchura,k, rpp);
                rtx(c, e, rayo, R, G, B);
                _R += R;
                _G += G;
                _B += B;
            }
            R = _R/rpp;
            G = _G/rpp;
            B = _B/rpp;


            flujoOut << R << " " << G << " " << B;
            if(j < w-1){
                flujoOut << "    ";
            }

        }
    }
}

