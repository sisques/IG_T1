#ifndef _TONEMAPPING_H_
#define _TONEMAPPING_H_

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>


using namespace std;


	double reinhardt(const double Lw, const double lmax,const double alpha, const long double avg){
	    long double l = (alpha/avg)*Lw;
	    return (l * (1.f + l / (lmax * lmax))) / (1.f + l);
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
        height = stod(line.substr(aux+1,line.length()-aux));;
        N = width * height;
        flujoIn >> cr;
        double a,b,c;
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                flujoIn >> a >> b >> c;
				a = a/cr;
				b = b/cr;
				c = c/cr;
				/*long */double lum = (0.2126*a + 0.7152*b + 0.0722*c);
				if(lum > lmax){lmax = lum;}
                avgLum = avgLum + media(lum, delta);
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
		double mc = MAX / cr, eq = 255.f / MAX;
		int x,y,z;
		for(int i = 0; i < height; ++i){
			for(int j = 0; j < width; ++j){
				flujoIn >> a >> b >> c;
				a = a/cr;
				b = b/cr;
				c = c/cr;
				/*long */double lum = (0.2126*a + 0.7152*b + 0.0722*c);
				/*long */double newLum = reinhardt(lum, lmax, alpha, avgLum);
                a = (a/lum) * newLum;
                b = (b/lum) * newLum;
                c = (c/lum) * newLum;
				x = a*255.f; if(x > 255){x = 255;}
				y = b*255.f; if(y > 255){y = 255;}
				z = c*255.f; if(z > 255){z = 255;}
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