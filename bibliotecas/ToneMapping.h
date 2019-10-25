#ifndef _TONEMAPPING_H_
#define _TONEMAPPING_H_

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Lab_RGB.h"


using namespace std;


	double reinhardt(const double Lw, const double lmax,const double alpha, const long double avg){
	    long double l = (alpha/avg)*Lw;
	    return (l	*	(1+	l / (pow(lmax,2)))) / (1+l);
	}

	double media(const double Lw, const double delta){
		return log(delta + Lw);
	}



//Devuelve true si y solo si no ha habido ningún problema durante el tone mapping
    bool applyToneMapping(const string fileIn, const double alpha, const double delta) {
	    /*long */double avgLum = 0;
		double MAX = 0, width = 0, height = 0, cr = 0, N = 0, lmax = 0;

        fstream flujoIn;
        flujoIn.open(fileIn.c_str(), ios::in);
        if (flujoIn.fail()) {
            return false;
        }
        string line;
        getline(flujoIn, line);
        getline(flujoIn, line);
        line.erase(0, 5);
        MAX = stod(line);
        getline(flujoIn, line);
        while(line[0] == '#'){
            getline(flujoIn, line);
        }
        int aux = line.find(" ");
        width = stod(line.substr(0,aux));
        height = stod(line.substr(aux+1,line.length()-aux));

        N = width * height;
        flujoIn >> cr;
        double mc = MAX / cr, eq = 255 / MAX;
        double RGB[3],  LAB[3];
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                flujoIn >> RGB[0] >> RGB[1] >> RGB[2];
                RGB[0] = RGB[0]*mc*eq;
                RGB[1] = RGB[1]*mc*eq;
				RGB[2] = RGB[2]*mc*eq;

				rgb2lab(RGB[0],RGB[1],RGB[2],LAB[0],LAB[1],LAB[2]);

				if(LAB[0] > lmax){lmax = LAB[0];}
                avgLum = avgLum + media(LAB[0], delta);
            }
        }
        avgLum = exp(avgLum/N);

        flujoIn.close();

        string fileOut = fileIn;
		fileOut.erase(fileOut.end()-4,fileOut.end());
		fileOut += "-LDR.ppm";
		fstream flujoOut;
		flujoIn.open(fileIn.c_str(), ios::in);
		if (flujoIn.fail()) {
			return false;
		}
		flujoOut.open(fileOut.c_str(), ios::out);
		flujoOut << "P3" << endl << "#MAX=255" << endl;
		getline(flujoIn, line);
		getline(flujoIn, line);
		line.erase(0, 5);
		MAX = stod(line);
		getline(flujoIn, line);
		while(line[0] == '#'){
			flujoOut << line;
			getline(flujoIn, line);
		}
		aux = line.find(" ");
		flujoIn >> cr;
		flujoOut << endl << width << " " << height << endl << 255 << endl;

		int x,y,z;
		for(int i = 0; i < height; ++i){
			for(int j = 0; j < width; ++j){
                flujoIn >> RGB[0] >> RGB[1] >> RGB[2];
                RGB[0] = RGB[0]*mc*eq;
                RGB[1] = RGB[1]*mc*eq;
                RGB[2] = RGB[2]*mc*eq;
                rgb2lab(RGB[0],RGB[1],RGB[2],LAB[0],LAB[1],LAB[2]);


                double lumAnt = LAB[0];
                double lumNueva = reinhardt(lumAnt, lmax, alpha, avgLum) * LAB[0];





                x = (RGB[0]/lumAnt)* lumNueva;
                y = (RGB[1]/lumAnt)* lumNueva;
                z = (RGB[2]/lumAnt)* lumNueva;
				flujoOut << x << " " << y << " " << z;
				if(j < width-1){
					flujoOut << "    ";
				}
			}
			flujoOut << endl;
		}
		
		flujoIn.close();
		flujoOut.close();
		return true;
	}

#endif