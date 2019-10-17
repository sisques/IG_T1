#ifndef _TONEMAPPING_H_
#define _TONEMAPPING_H_

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

	int equalize(const double s, const double mc, const double equ){
		return (s * mc) * equ;
	}

	//Devuelce true si y solo si no ha habido ningún problema durante el tone mapping
    bool applyToneMapping(const string fileIn) {
		double MAX = 0, wide = 0, high = 0, cr = 0;
		string fileOut = fileIn;
		fileOut.erase(fileOut.end()-4,fileOut.end());
		fileOut += "-LDR.ppm";
		fstream flujoOut;
		fstream flujoIn;
		flujoIn.open(fileIn.c_str(), ios::in);
		if (flujoIn.fail()) {
			return false;
		}
		flujoOut.open(fileOut.c_str(), ios::out);
		flujoOut << "P3" << endl << "#MAX=255" << endl;
		string line;
		getline(flujoIn, line);
		getline(flujoIn, line);
		line.erase(0, 5);
		MAX = stod(line);
		getline(flujoIn, line);
		while(line[0] == '#'){
			flujoOut << line;
			getline(flujoIn, line);
		}
		int aux = line.find(" ");
		wide = stod(line.substr(0,aux));
		high = stod(line.substr(aux+1,line.length()-aux));;
		flujoIn >> cr;
		flujoOut << endl << wide << " " << high << endl << 255 << endl;
		double mc = MAX / cr, eq = 255 / MAX;
		double a,b,c;
		int x,y,z;
		for(int i = 0; i < high; ++i){
			for(int j = 0; j < wide; ++j){
				flujoIn >> a >> b >> c;
				x = equalize(a,mc,eq);
				y = equalize(b,mc,eq);
				z = equalize(c,mc,eq);
				flujoOut << x << " " << y << " " << z;
				if(j < wide-1){
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