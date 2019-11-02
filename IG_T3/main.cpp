#include "../bibliotecas/punto_direccion.h"
#include "../bibliotecas/camara.h"
#include "../bibliotecas/esfera.h"
#include "../bibliotecas/plano.h"
#include "../bibliotecas/tranformations.h"


#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;


dir monteCarlo(camara c, int altura, int anchura, int numRayo, int rpp){
    point origen = c.o;
    point final = newPoint(3, (double)altura, (double)anchura);
    return final - origen;

}


void rtx(/* escena e*/ sphere s,  dir rayo, int& R, int& G, int& B){
    point p = newPoint(rayo.x, rayo.y, rayo.z);
    bool colision = sphereImplicit(p, s);


    if (colision) {
        R = s.R;
        G = s.G;
        B = s.B;
    } else {
        R = 0;
        G = 0;
        B = 0;
    }
}



int main(){
    /*
     * camara       x   y   z
     *      o   =   0   0   0
     *      u   =   0   1   0
     *      l   =   0   0   1
     *      f   =   1   0   0
     *
     *      aspect ratio        1:1
     */
	point origenCamara = newPoint(0,0,0);
    dir alturaPlano = newDir(0,1,0);
    dir anchuraPlano = newDir(0,0,1);
    dir distanciaPlano = newDir(1,0,0);

	camara c =  newCamara(origenCamara, alturaPlano, anchuraPlano, distanciaPlano);
    int rpp = 0, w = 0, h = 0;
    string fOut = "";
    cout << "Introduce la altura de la imagen deseada:" << endl;
    cin >> h;

    cout << "Introduce la anchura de la imagen deseada:" << endl;
    cin >> w;

	cout << "Introduce el numero de rayos por pixel deseado:" << endl;
    cin >> rpp;

    cout << "Introduce el nombre del fichero de salida:" << endl;
    cin >> fOut;
    fOut =  "/home/victor/4o/IG/IG_T1/imagenes/"+ fOut + ".ppm";
    fstream flujoOut;
    flujoOut.open(fOut.c_str(), ios::out);
    flujoOut    <<      "P3"     << endl
                <<   "#MAX=255"  << endl
                <<  "# " << fOut << endl
                << w << " " << h << endl
                <<      "255"    << endl;


    int R = 0, G = 0, B = 0;
    dir rayo;
    sphere s = newSphere(newPoint(3.0,0.0,0.0), 50.0, 63,127, 255);

    for (int i = h/2 ; i > -h/2; i--){
        for (int j = w/2; j > -w/2; j--){
            for (int k = 0; k < rpp; k++){
                rayo = monteCarlo(c, i,j,k, rpp);
                rtx(s, rayo, R, G, B);
                flujoOut << R << " " << G << " " << B;
                if(j < w-1){
                    flujoOut << "    ";
                }
            }
        }
    }
}
