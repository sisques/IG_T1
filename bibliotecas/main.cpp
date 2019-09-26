#include "punto_direccion.h"
#include <iostream>
using namespace std;


int main(){
	point p = newPoint(0,1.0,1.0);
	point q = newPoint(0,2.0,3.0);
	point r = newPoint(0,3.0,2.0);
	cout << "puntos" << endl;
	cout << p.x << " " << p.y << " " << p.z << endl; 
	cout << q.x << " " << q.y << " " << q.z << endl; 
	cout << r.x << " " << r.y << " " << r.z << endl; 

	dir d1 = newDir(0,1.0,4.0);
	dir d2 = q - p;
	dir d3 = d1 + d2; 
	dir d4 = (r-q) - d3;

	dir d5 = 3.1*d1;
	dir d6 = 3.1/d5;
	
	cout << "direcciones" << endl;
	cout << d1.x << " " << d1.y << " " << d1.z << endl; 
	cout << d2.x << " " << d2.y << " " << d2.z << endl; 
	cout << d3.x << " " << d3.y << " " << d3.z << endl;
	cout << d4.x << " " << d4.y << " " << d4.z << endl;
	cout << d5.x << " " << d5.y << " " << d5.z << endl;
	cout << d6.x << " " << d6.y << " " << d6.z << endl;


	cout << "modulo de d5" << endl; 
	cout << mod(d5) << endl;

	point s = p + d1;

	cout << "punto s" << endl;
	cout << s.x << " " << s.y << " " << s.z << endl; 


	cout << "calculo de producto escalar" << endl;

	dir a = newDir(0,1.0,1.0);
	dir b = newDir (0,5.0,0);

	cout << a.x << " " << a.y << " " << a.z << endl; 
	cout << b.x << " " << b.y << " " << b.z << endl;

	double escalar = dot(a,b);
	double angulo = angle(a,b);
	
	cout << escalar << endl;
	cout << angulo  << endl;


	cout << "calculo de producto vectorial" << endl;
;

	cout << a.x << " " << a.y << " " << a.z << endl; 
	cout << b.x << " " << b.y << " " << b.z << endl;

	dir vectorial = cross(a,b);
	
	cout << vectorial.x << " " << vectorial.y << " " << vectorial.z << endl;
	double angulo_ab = angle(a,b);
	double angulo_bvect = angle(b,vectorial);
	double angulo_vecta = angle(vectorial,a);
	cout << angulo_ab << endl; 
	cout << angulo_bvect << endl; 
	cout << angulo_vecta << endl; 

}
