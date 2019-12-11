#ifndef _PLY_READER_H_
#define _PLY_READER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <list>
#include "figuras.h"
#include "punto_direccion.h"
#include "matrix.h"
#include "materialProperties.h"
#include "textures.h"

using namespace std;





list<shared_ptr<figura>> plyReader(const string fileIn,materialProperties mp, const Matrix trans[],int arrSize) {
    list<shared_ptr<figura>> output;

    fstream flujoIn;
    flujoIn.open(fileIn.c_str(), ios::in);


    string currentWord;
    int vertexN = 0;
    int faceN = 0;

    double x, y, z;
    int check;
    int a,b,c;
    point v0, v1, v2;
    bool normal = false;
    bool vertexColor = false;


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
            } else if (currentWord == "property"){
                flujoIn >> currentWord;
                flujoIn >> currentWord;
                if (currentWord == "nx"){
                    normal = true;
                }else if (currentWord == "red"){
                    vertexColor = true;
                }

            }
        }
    }
    int R,G,B, alpha;
    point puntos[vertexN];
    dir normals[vertexN];
    int Red[vertexN];
    int Green[vertexN];
    int Blue[vertexN];
    for(int i =  0; i < vertexN; i++) {
        flujoIn >> x;
        flujoIn >> y;
        flujoIn >> z;
        point pto = newPoint(x, y, z);
        for (int j = 0; j < arrSize; j++){
            pto = trans[j] * pto;
        }
        puntos[i] = pto;
        if (normal){
            flujoIn >> x;
            flujoIn >> y;
            flujoIn >> z;
            dir d = newDir(x, y, z);
            normals[i] = d;
        } if (vertexColor){
            flujoIn >> R;
            flujoIn >> G;
            flujoIn >> B;
            Red[i] = R;
            Green[i] = G;
            Blue[i] = B;
            flujoIn >> alpha;
        }
    }

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
        if (vertexColor) {
            R = (Red[a] + Red[b] + Red[c]) / 3;
            G = (Green[a] + Green[b] + Green[c]) / 3;
            B = (Blue[a] + Blue[b] + Blue[c]) / 3;
        } else{
            R = 0;
            G = 0;
            B = 0;
        }

        //mp.setKd(R,G,B);
        //mp.setKs(R,G,B);
        mp.setKdPhong(R,G,B);
        mp.setKsPhong(R,G,B);

        shared_ptr<figura> fig;
        if(normal) {
            fig = make_shared<triangulo>(triangulo(v0, v1, v2, normals[a],mp));
        } else {
            fig = make_shared<triangulo>(triangulo(v0, v1, v2,mp));
        }
        output.push_back(fig);
    }


    flujoIn.close();
    return output;
}

#endif