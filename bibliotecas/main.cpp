#include "punto_direccion.h"
#include "esfera.h"
#include "plano.h"
#include <iostream>
#include "math.h"
using namespace std;


int main(){
	double x, y, z;
	double incl1, incl2;
	double azim1, azim2;
	double r1, r2;
	bool ok;
	point centro1, centro2;
	point ciudad1, ciudad2;
	dir eje1, eje2;
	
	//----------PLANETA 1----------//

	// Primero se le solicita al usuario las coordenadas en UCS del centro, eje 
	// y cuidad de referencia del planeta 1
	cout << "Introduce las coordenadas del centro del planeta 1 en UCS" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	centro1 = newPoint(x,y,z);

	cout << "Introduce la direccion del eje del planeta 1 en UCS" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	eje1 = newDir(x,y,z);

	cout << "Introduce las coordenadas de la ciudad de referencia del planeta 1 en UCS" << endl;
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
		cout << "Los valores introducidos son correctos." << endl;
	} else {
		cout << "Valores incorrectos, se va a detener la ejecución." << endl;
		exit(0);
	}


	// Acto seguido se pasará a solicitar al usuario la inclinación y azimut que definen la estacion
	// planetaria del planeta 1
	cout << "Introduce la inclinación de la estación planetaria del planeta 1, rango (0,pi)" << endl;
	cout << "La inclinación es el angulo de la estación respecto al eje del planeta" << endl;
	cout << "Inclinación: " ;cin >> incl1;

	if (!(incl1 > 0 && incl1 < M_PI)) {
		cout << "La inclinación introducida no es valida, se va a detener la ejecución." << endl;
		exit(1);
	}
	
	cout << "Introduce el azimut de la estación planetaria del planeta 1, rango (-pi, pi]" << endl;
	cout << "El azimut es el angulo de la estación espacial respecto del 0-meridiano" << endl;
	cout << "Azimut: " ;cin >> azim1;

	if (!(azim1 > -M_PI && azim1 <= M_PI)) {
		cout << "El azimut introducida no es valida, se va a detener la ejecución." << endl;
		exit(1);
	}


	// Finalmente se calculara la posición de la estación en el sistema de coordenadas del planeta
	//TODO


	//----------PLANETA 2----------//


	// Luego se le solicita al usuario las coordenadas en UCS del centro, eje 
	// y cuidad de referencia del planeta 2
	cout << "Introduce las coordenadas del centro del planeta 2 en UCS" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	centro2 = newPoint(x,y,z);

	cout << "Introduce la direccion del eje del planeta 2 en UCS" << endl;
	cout << "x: " ;cin >> x;
	cout << "y: " ;cin >> y;
	cout << "z: " ;cin >> z;
	eje2 = newDir(x,y,z);

	cout << "Introduce las coordenadas de la ciudad de referencia del planeta 2 en UCS" << endl;
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
		cout << "Los valores introducidos son correctos." << endl;
	} else {
		cout << "Valores incorrectos, se va a detener la ejecución." << endl;
		exit(0);
	}


	// Acto seguido se pasará a solicitar al usuario la inclinación y azimut que definen la estacion
	// planetaria del planeta 2
	cout << "Introduce la inclinación de la estación planetaria del planeta 2, rango (0,pi)" << endl;
	cout << "La inclinación es el angulo de la estación respecto al eje del planeta" << endl;
	cout << "Inclinación: " ;cin >> incl1;

	if (!(incl2 > 0 && incl2 < M_PI)) {
		cout << "La inclinación introducida no es valida, se va a detener la ejecución." << endl;
		exit(1);
	}

	cout << "Introduce el azimut de la estación planetaria del planeta 2, rango (-pi, pi]" << endl;
	cout << "El azimut es el angulo de la estación espacial respecto del 0-meridiano" << endl;
	cout << "Azimut: " ;cin >> azim2;

	if (!(azim2 > -M_PI && azim2 <= M_PI)) {
		cout << "El azimut introducido no es valido, se va a detener la ejecución." << endl;
		exit(1);
	}

	// Finalmente se calculara la posición de la estación en el sistema de coordenadas del planeta
	//TODO


	//----------CÁLCULO DE LA CONEXIÓN----------//
}



