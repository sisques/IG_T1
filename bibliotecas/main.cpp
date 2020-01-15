#include "punto_direccion.h"
#include "esfera.h"
#include "plano.h"
#include <iostream>
#include "math.h"
#include <tranformations.h>

using namespace std;


int main(){
	double x, y, z;
	double incl1, incl2;
	double azim1, azim2;
	double r1, r2;
	bool ok;
	point cero = newPoint(0,0,0);
	point centro1, centro2, ciudad1, ciudad2, estacion1, estacion2, norte1, norte2;
	dir ejeX1, ejeY1, ejeZ1, ejeX2, ejeY2, ejeZ2;
	dir eje1, eje2;
	
	//----------PLANETA EMISOR----------//

	// Primero se le solicita al usuario las coordenadas en UCS del centro, eje 
	// y cuidad de referencia del planeta emisor
	cout << endl << "Introduce las coordenadas del centro del planeta emisor en UCS" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	centro1 = newPoint(x,y,z);

	cout << endl << "Introduce la direccion del eje del planeta emisor en UCS" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	eje1 = newDir(x,y,z);

	cout << endl << "Introduce las coordenadas de la ciudad de referencia del planeta emisor en UCS" << endl;
	cout << "Las coordenadas de la ciudad de referencia se usarán para definir el 0-meridiano" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	ciudad1 = newPoint(x,y,z);


	// Ahora se comprueba si el radio definido por el eje y por la distancia de la
	// cuidad de referencia es el mismo ( maximo error de 10 a la - 6)
	r1 = mod(eje1)/2;
	r2 = dist(centro1, ciudad1);

	ok = abs(r1-r2) <= 0.000001 ? true : false ;

	if (ok) {
		cout << endl << "Los valores introducidos son correctos." << endl;
	} else {
		cout << endl << "Valores incorrectos, se va a detener la ejecución." << endl;
		exit(0);
	}


	// Acto seguido se pasará a solicitar al usuario la inclinación y azimut que definen la estacion
	// planetaria del planeta emisor
	cout << endl << "Introduce la inclinación de la estación planetaria del planeta emisor, rango (0,pi)" << endl;
	cout << "La inclinación es el angulo de la estación respecto al eje del planeta" << endl;
	cout << "Inclinación: " ;cin >> incl1;

	if (!(incl1 > 0 && incl1 < M_PI)) {
		cout << endl << "La inclinación introducida no es valida, se va a detener la ejecución." << endl;
		exit(1);
	}
	
	cout << endl << "Introduce el azimut de la estación planetaria del planeta emisor, rango (-pi, pi]" << endl;
	cout << "El azimut es el angulo de la estación espacial respecto del 0-meridiano" << endl;
	cout << "Azimut: " ;cin >> azim1;

	if (!(azim1 > -M_PI && azim1 <= M_PI)) {
		cout << endl << "El azimut introducida no es valida, se va a detener la ejecución." << endl;
		exit(1);
	}

	//Cálculo de la estación 1
	estacion1 = sphereParametric(cero,r1, incl1, azim1);
	norte1 = centro1 + 2 / eje1;
	ejeY1  = 2 / eje1;
	if(ejeY1.y < 0){ejeY1  = -1 * eje1;}
	ejeX1 = cross((ciudad1 - centro1), ejeY1);
	ejeZ1 = cross(ejeY1, ejeX1);
	estacion1 = basec(ejeX1,ejeY1,ejeZ1,centro1, estacion1);

	//----------PLANETA RECEPTOR----------//


	// Luego se le solicita al usuario las coordenadas en UCS del centro, eje 
	// y cuidad de referencia del planeta receptor
	cout << "Introduce las coordenadas del centro del planeta receptor en UCS" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	centro2 = newPoint(x,y,z);

	cout << endl << "Introduce la direccion del eje del planeta receptor en UCS" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	eje2 = newDir(x,y,z);

	cout << endl << "Introduce las coordenadas de la ciudad de referencia del planeta receptor en UCS" << endl;
	cout << "Las coordenadas de la ciudad de referencia se usarán para definir el 0-meridiano" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	ciudad2 = newPoint(x,y,z);


	// Ahora se comprueba si el radio definido por el eje y por la distancia de la
	// cuidad de referencia es el mismo ( maximo error de 10 a la - 6)
	r1 = mod(eje2)/2;
	r2 = dist(centro2, ciudad2);

	ok = abs(r1-r2) <= 0.000001 ? true : false ;

	if (ok) {
		cout << endl << "Los valores introducidos son correctos." << endl;
	} else {
		cout << endl << "Valores incorrectos, se va a detener la ejecución." << endl;
		exit(0);
	}


	// Acto seguido se pasará a solicitar al usuario la inclinación y azimut que definen la estacion
	// planetaria del planeta receptor
	cout << endl << "Introduce la inclinación de la estación planetaria del planeta receptor, rango (0,pi)" << endl;
	cout << "La inclinación es el angulo de la estación respecto al eje del planeta" << endl;
	cout << "Inclinación: " ;cin >> incl2;

	if (!(incl2 > 0 && incl2 < M_PI)) {
		cout << endl << "La inclinación introducida no es valida, se va a detener la ejecución." << endl;
		exit(1);
	}

	cout << endl << "Introduce el azimut de la estación planetaria del planeta receptor, rango (-pi, pi]" << endl;
	cout << "El azimut es el angulo de la estación espacial respecto del 0-meridiano" << endl;
	cout << "Azimut: " ;cin >> azim2;

	if (!(azim2 > -M_PI && azim2 <= M_PI)) {
		cout << endl << "El azimut introducido no es valido, se va a detener la ejecución." << endl;
		exit(1);
	}

	//Cálculo de la estación 2
	estacion2 = sphereParametric(cero,r1, incl2, azim2);
	norte2 = centro2 + 2 / eje2;
	ejeY2  = 2 / eje2;
	if(ejeY2.y < 0){ejeY2  = -1 * eje2;}
	ejeX2 = cross((ciudad2 - centro2), ejeY2);
	ejeZ2 = cross(ejeY2, ejeX2);
	estacion2 = basec(ejeX2,ejeY2,ejeZ2,centro2, estacion2);


	//----------CÁLCULO DE LA CONEXIÓN----------//
	dir conex = estacion2 - estacion1;
	
	ejeY1 = estacion1 - centro1;
	ejeZ1 = norte1 - estacion1;
	ejeX1 = cross(ejeY1, ejeZ1);
	ejeZ1 = cross(ejeX1, ejeY1);
	
	ejeY2 = estacion2 - centro2;
	ejeZ2 = norte2 - estacion2;
	ejeX2 = cross(ejeY2, ejeZ2);
	ejeZ2 = cross(ejeX2, ejeY2);
	
	dir d1 = desbasec(ejeX1, ejeY1, ejeZ1, estacion1, conex);
	dir d2 = -1 * desbasec(ejeX2, ejeY2, ejeZ2, estacion2, conex);
	
	cout << endl << "La estación emisora lanzará en la dirección x: " << d1.x << " y: " << d1.y << " z: " << d1.z << endl;
	if(d1.y < 0){cout << "Problema en la dirección de emisión. ALERTA" << endl;}
	cout << endl << "La estación receptora recibira en la dirección x: " << d2.x << " y: " << d2.y << " z: " << d2.z << endl;
	if(d2.y < 0){cout << "Problema en la dirección de recepción. ALERTA" << endl;}
}
