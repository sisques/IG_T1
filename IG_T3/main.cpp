#include "../bibliotecas/punto_direccion.h"
#include "../bibliotecas/camara.h"
#include "../bibliotecas/figuras.h"
#include "../bibliotecas/monteCarlo.h"
#include "../bibliotecas/ply_reader.h"

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
    reflexion.setKdPhong(255,255,255);
    reflexion.setKsPhong(255,255,255);
    reflexion.setAlfa(10);



    string macaco = "/home/victor/gitRepos/IG_T1/models/macaco.ply";

    Matrix transformation[] = { scale(0.05,0.05,0.05)};



    //list<shared_ptr<figura>> elementos = plyReader(macaco,mp, transformation,1);

    list<shared_ptr<figura>> elementos;


    light.setKs(120,0,0);
    shared_ptr<figura> lght_src_1 = make_shared<plano>(plano(newPoint(-0.5,0.4,0), newDir(1,-1,0),light));
    light.setKs(0,120,0);
    shared_ptr<figura> lght_src_2 = make_shared<plano>(plano(newPoint(0.5,0.4,0), newDir(-1,-1,0),light));
    light.setKs(120,120,120);
    shared_ptr<figura> lght_src_3 = make_shared<plano>(plano(newPoint(0,0.4,1), newDir(0,-1,-1),light));





    elementos.push_back(lght_src_1);
    elementos.push_back(lght_src_2);
    elementos.push_back(lght_src_3);

    mp.setKdPhong(120, 120, 120);
    mp.setKsPhong(120, 120, 120);
    shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,1), newDir(0,0,-1), mp));
    shared_ptr<figura> suelo = make_shared<plano>(plano(newPoint(0,-0.5,0), newDir(0,1,0),mp));
    shared_ptr<figura> techo = make_shared<plano>(plano(newPoint(0,0.5,0), newDir(0,-1,0),mp));


    mp.setKdPhong(255,0,0);
    mp.setKsPhong(255,0,0);
    shared_ptr<figura> izquierda = make_shared<plano>(plano(newPoint(-0.5,0,0), newDir(1,0,0),mp));

    mp.setKdPhong(0,255,0);
    mp.setKsPhong(0,255,0);
    shared_ptr<figura> derecha = make_shared<plano>(plano(newPoint(0.5,0,0), newDir(-1,0,0),mp));



    shared_ptr<figura> reflexion_1 = make_shared<esfera>(newPoint(0.3,-0.3,0.8), 0.2, reflexion);

    mp.setKdPhong(0,0,255);
    mp.setKsPhong(0,0,255);
    shared_ptr<figura> phong_1 = make_shared<esfera>(newPoint(0.4,-0.02,0.9), 0.1, mp);


    point p1 = newPoint(-0.5,0.2,0.75);
    point p2 = newPoint(-0.5,-0.5,0.75);
    point p3 = newPoint(-0.1,0.2,1);
    point p4 = newPoint(-0.1,-0.5,1);

    shared_ptr<figura> espejo_1 = make_shared<triangulo>(triangulo(p1,p2,p3,reflexion));

     shared_ptr<figura> espejo_2 = make_shared<triangulo>(triangulo(p2,p3,p4,reflexion));

    elementos.push_back(espejo_1);
    elementos.push_back(espejo_2);

    elementos.push_back(fondo);
    elementos.push_back(suelo);
    elementos.push_back(techo);
    elementos.push_back(izquierda);
    elementos.push_back(derecha);



    elementos.push_back(reflexion_1);
    elementos.push_back(phong_1);




    double brdfValues2[] = {0.9,0.0,0.0};
    materialProperties limit = materialProperties(false, eventos, brdfValues2,0);
    limit.setKdPhong(0,0,0);
    limit.setKsPhong(0,0,0);
    shared_ptr<figura> limite = make_shared<plano>(plano(newPoint(0,0,0), newDir(0,0,-1), limit));

    elementos.push_back(limite);

    return elementos;
}

int completadas = 0;

void generateScene( monteCarlo mc, const list<shared_ptr<figura>> &e,
                    const string &fOut, const int &hMin, const int &hMax, const int &w, const int &h){
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

    clock_t tStart = clock();
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
    h = 1000;
    w = 1000;
    rpp = 512;
    int threads = 4;
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

    clock_t tEnd = clock();

    cout << endl;
    cout << "segundos en ejecución -> " <<(double)(tEnd - tStart)/CLOCKS_PER_SEC << endl;


}
