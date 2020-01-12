#ifndef _PPM_READER_H_
#define _PPM_READER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <list>
#include "figuras.h"
#include "punto_direccion.h"

using namespace std;

	// Función para leer una imagen en formato ppm y transformarla
	// en una lista de "pixeles" representado por un vector de dimensión 3
	// El primero "pixel" contiene la altura y anchura de la imagen
	list<int*> ppmReader(const string fileIn) {
		list<int*> ouput;
		fstream flujoIn;
		flujoIn.open(fileIn.c_str(), ios::in);
		string ignore;
		flujoIn >> ignore;
		int width, height, cr, r,g,b;
		flujoIn >> width >> height >> cr;
		int *aux = new int[3];
		aux[0] = width; aux[1] = height; aux[2] = cr;
		ouput.push_back(aux);
		while(!flujoIn.eof()){
			aux = new int[3];
			flujoIn >> r >> g >> b;
			aux[0] = r; aux[1] = g; aux[2] = b;
			ouput.push_back(aux);
		}
		
		flujoIn.close();
		return ouput;
	}

#endif