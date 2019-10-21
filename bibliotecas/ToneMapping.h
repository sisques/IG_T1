#ifndef _TONEMAPPING_H_
#define _TONEMAPPING_H_

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../bibliotecas/enum.h"


using namespace std;


	int reinhardt(const double Lw, const double alpha, const double avg, const double max, const double cr){
	    double realValue = Lw*max/cr;
	    double l = (alpha/avg)*realValue;
	    int writeValue = l*cr/max;
	    return (writeValue)/*/(1+writeValue)*/;
	}

	double media(const double Lw, const double delta, const double max, const double cr){
	    double realValue = Lw*max/cr;
	    return log10(delta + realValue);
	}


	//Devuelve true si y solo si no ha habido ningún problema durante el tone mapping
    bool applyToneMapping(const string fileIn, tipo algoritmo) {
	    double N = 0, avgR = 0, avgG = 0, avgB = 0;
	    double alpha = 0.18, delta = 0.05;
		double MAX = 0, width = 0, height = 0, cr = 0;



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
        height = stod(line.substr(aux+1,line.length()-aux));;
        N = width * height;
        flujoIn >> cr;
        double a,b,c;
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                flujoIn >> a >> b >> c;
                avgR = avgR + media(a, delta, MAX, cr);
                avgG = avgG + media(b, delta, MAX, cr);
                avgB = avgB + media(c, delta, MAX, cr);
            }
        }
        avgR = exp(avgR/N) ;
        avgG = exp(avgG/N);
        avgB = exp(avgB/N);


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
		double mc = MAX / cr, eq = 255 / MAX;
		int x,y,z;
		for(int i = 0; i < height; ++i){
			for(int j = 0; j < width; ++j){
				flujoIn >> a >> b >> c;
                x = reinhardt(a, alpha, avgR, MAX, cr);
                y = reinhardt(b, alpha, avgG, MAX, cr);
                z = reinhardt(c, alpha, avgB, MAX, cr);
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