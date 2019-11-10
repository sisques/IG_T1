#ifndef _PLY_READER_H_
#define _PLY_READER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <list>
#include "figuras.h"
#include "punto_direccion.h"

using namespace std;




	//Devuelve true si y solo si no ha habido ningún problema durante el tone mapping
    list<shared_ptr<figura>> plyReader(const string fileIn) {
        list<shared_ptr<figura>> ouput;

        fstream flujoIn;
        flujoIn.open(fileIn.c_str(), ios::in);


        string currentWord;
        int vertexN = 0;
        int faceN = 0;

        double x, y, z;
        int check;
        int a,b,c;
        point v0, v1, v2;

        flujoIn >> currentWord;
        if (currentWord == "ply") {
            while (currentWord != "end_header") {
                flujoIn >> currentWord;
                if (currentWord == "element"){
                    flujoIn >> currentWord;
                    if(currentWord == "vertex") {
                        flujoIn >> vertexN;
                    } else {
                        flujoIn >> faceN;
                    }
                }
            }
        }
        point puntos[vertexN];
        for(int i =  0; i < vertexN; i++){
            flujoIn >> x;
            flujoIn >> y;
            flujoIn >> z;
            puntos[i] = newPoint(x,y,z);
        }

        int R,G,B;
        for(int i =  0; i < faceN; i++) {
            flujoIn >> check;
            if (check != 3) {
                cerr << "FICHERO INCOMPATIBLE" << endl;
                exit(50);
            }
            flujoIn >> a;
            flujoIn >> b;
            flujoIn >> c;
            v0 = puntos[a];
            v1 = puntos[b];
            v2 = puntos[c];

         /*   flujoIn >> R;
            flujoIn >> G;
            flujoIn >> B;*/
            R = 255;
            G = 255;
            B = 255;
            shared_ptr<figura> tri = make_shared<triangulo>(triangulo(v0, v1, v2, R,G,B));
            ouput.push_back(tri);
        }


        flujoIn.close();
        return ouput;
    }

#endif