#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <cstdlib>
#include <cstdio>
#include <math.h>
#include "punto_direccion.h"


// Basado en ->http://www.speqmath.com/tutorials/matrix/matrix.html
// Clase que representa una matriz de 4x4
class Matrix{
public:
    double p[4][4];     // puntero a matriz de doubles

    // constructor
    Matrix(){
        for (int f = 0; f < 4; f++){
            for (int c = 0; c < 4; c++) {
                p[f][c] = 0;
            }
        }
    }
	
	// Define una matriz vacía
    Matrix empty(){
        Matrix m;
        return m;
    }

    // Operador de asignacion
    Matrix& operator= (const Matrix& a){
        for (int f = 0; f < 4; f++) {
            for (int c = 0; c < 4; c++){
                p[f][c] = a.p[f][c];
            }
        }
        return *this;
    }




};

// Función de multiplicación auxiliar
void mulAux(const Matrix& m, double& x, double& y, double& z, double& w){
    double a, b, c, d;
    a = m.p[0][0]*x + m.p[0][1]*y + m.p[0][2]*z + m.p[0][3]*w;
    b = m.p[1][0]*x + m.p[1][1]*y + m.p[1][2]*z + m.p[1][3]*w;
    c = m.p[2][0]*x + m.p[2][1]*y + m.p[2][2]*z + m.p[2][3]*w;
    d = m.p[3][0]*x + m.p[3][1]*y + m.p[3][2]*z + m.p[3][3]*w;
    x = a; y = b; z = c; w = d;
}

// Operador de suma
Matrix operator+ (const Matrix& a, const Matrix& b)    {
    Matrix res = Matrix();
    for (int f = 0; f < 4; f++){
        for (int c = 0; c < 4; c++){
            res.p[f][c] = a.p[f][c] + b.p[f][c];
        }
    }
    return res;
}



// Operador de multiplicacion
Matrix operator* (const Matrix& a, const Matrix& b)    {
    Matrix res = Matrix();
    for (int f = 0; f < 4; f++){
        for (int c_res = 0; c_res < 4; c_res++){
            for (int c = 0; c < 4; c++){
                res.p[f][c_res] += a.p[f][c] * b.p[f][c_res];
            }
        }
    }
    return res;
}

// Operador de division entre real
Matrix operator/ (const Matrix& a, const double& b)    {
    Matrix res = Matrix();
    for (int f = 0; f < 4; f++){
        for (int c = 0; c < 4; c++){
            res.p[f][c] = a.p[f][c] / b;
        }
    }
    return res;
}


// Operador de multiplicacion por punto
point operator* (const Matrix& m, const point p)    {
    double a,b,c,d;
    a = p.x; b = p.y; c = p.z; d = p.w;
    mulAux(m, a, b, c, d);
    return newPoint(a, b, c);
}

// Operador de multiplicacion por direccion
dir operator* (const Matrix& m, const dir& p)    {
    double a,b,c,d;
    a = p.x; b = p.y; c = p.z; d = p.w;
    mulAux(m, a, b, c, d);
    return newDir(a, b, c);
}

// Operador de multiplicacion por punto
point operator* (const point p, const Matrix& m)    {
    double a,b,c,d;
    a = p.x; b = p.y; c = p.z; d = p.w;
    mulAux(m, a, b, c, d);
    return newPoint(a, b, c);
}

// Operador de multiplicacion por direccion
dir operator* (const dir& p, const Matrix& m)    {
    double a,b,c,d;
    a = p.x; b = p.y; c = p.z; d = p.w;
    mulAux(m, a, b, c, d);
    return newDir(a, b, c);
}

// Devuelve el determinante de una matriz 3x3 según la ley de sarrus
double det3x3(const double m[3][3]){
    return  m[0][0]*m[1][1]*m[2][2] +
            m[0][1]*m[1][2]*m[2][0] +
            m[1][0]*m[2][1]*m[0][2] -
            m[0][2]*m[1][1]*m[2][0] -
            m[1][2]*m[2][1]*m[0][0] -
            m[1][0]*m[0][1]*m[2][2];
}

// Devuelve el determinante de una matriz
double det(const Matrix& m){
    double det = 0;
    if(m.p[0][0] != 0){
        double aux[3][3] = { {m.p[1][1], m.p[1][2], m.p[1][3]},
                             {m.p[2][1], m.p[2][2], m.p[2][3]},
                             {m.p[3][1], m.p[3][2], m.p[3][3]}
        };
        det += m.p[0][0]*det3x3(aux);
    }
    if(m.p[1][0] != 0){
        double aux[3][3] = { {m.p[0][1], m.p[0][2], m.p[0][3]},
                             {m.p[2][1], m.p[2][2], m.p[2][3]},
                             {m.p[3][1], m.p[3][2], m.p[3][3]}
        };
        det -= m.p[1][0]*det3x3(aux);
    }
    if(m.p[2][0] != 0){
        double aux[3][3] = { {m.p[0][1], m.p[0][2], m.p[0][3]},
                             {m.p[1][1], m.p[1][2], m.p[1][3]},
                             {m.p[3][1], m.p[3][2], m.p[3][3]}
        };
        det += m.p[2][0]*det3x3(aux);
    }
    if(m.p[3][0] != 0){
        double aux[3][3] = { {m.p[0][1], m.p[0][2], m.p[0][3]},
                             {m.p[1][1], m.p[1][2], m.p[1][3]},
                             {m.p[2][1], m.p[2][2], m.p[2][3]}
        };
        det -= m.p[3][0]*det3x3(aux);
    }

    return det;
}

// Devuelve la matriz adjunta de a
Matrix adj(const Matrix& a){
    Matrix res = Matrix();
    for(int f = 0; f < 4; f++){
        for(int c = 0; c < 4; c++){
            double aux[3][3];
            int h = 0, k = 0;
            for(int x = 0; x < 4; x++){
                for(int y = 0; y < 4 && h <3 && k < 3; y++){
                    if(y != c){
                        aux[h][k] = a.p[x][y];
                        k++;
                    }
                }
                if(x != f){h++;}
                k = 0;
            }
            double test =  pow(-1.0, f+c)*det3x3(aux);
            res.p[f][c] = test;
        }
    }
    return res;
}

// Devuelve la matriz traspuesta de a
Matrix transp(const Matrix& a){
    Matrix res = Matrix();
    for(int f = 0; f < 4; f++){
        for(int c = 0; c < 4; c++){
            res.p[f][c] = a.p[c][f];
        }
    }
    return res;
}

// Devuelve la matriz inversa de a
Matrix inverse(const Matrix& a){
    Matrix adjM = adj(a);
    Matrix trasnpM = transp(adjM);
    double d = det(a);
    return trasnpM / d;
}

// Devuelve una matriz para escalar en base a x_s, y_s y z_s
Matrix scale(const double x_s, const double y_s, const double z_s){
    // x_s  0   0   0
    // 0    y_s 0   0
    // 0    0   z_s 0
    // 0    0   0   1
    Matrix res = Matrix();
    res.p[0][0] = x_s;
    res.p[1][1] = y_s;
    res.p[2][2] = z_s;
    res.p[3][3] = 1;
    return res;
}

// Devuelve una matriz de traslacción en base a x_s, y_s y z_s
Matrix translate(const double x_t, const double y_t, const double z_t){
    // 1    0   0   x_t
    // 0    1   0   y_t
    // 0    0   1   z_t
    // 0    0   0   1
    Matrix res = Matrix();
    res.p[0][0] = 1;
    res.p[1][1] = 1;
    res.p[2][2] = 1;
    res.p[3][3] = 1;
    res.p[0][3] = x_t;
    res.p[1][3] = y_t;
    res.p[2][3] = z_t;
    return res;
}

// Devuelve una matriz para rotar en el eje x en base a i
Matrix rotateX(const double i){
    //1      0       0       0
    //0      cos(i)  -sin(i) 0
    //0      sin(i)  cos(i)  0
    //0      0       0       1

    Matrix res = Matrix();
    res.p[0][0] = 1;
    res.p[3][3] = 1;
    res.p[1][1] = cos(i);
    res.p[2][2] = cos(i);
    res.p[1][2] = -sin(i);
    res.p[2][1] = sin(i);
    return res;
}

// Devuelve una matriz para rotar en el eje y en base a i
Matrix rotateY(const double i){
    //cos(i)    0       sin(i)  0
    //0         1       0       0
    //-sin(i)   0       cos(i)  0
    //0         0       0       1


    Matrix res = Matrix();
    res.p[1][1] = 1;
    res.p[3][3] = 1;
    res.p[0][0] = cos(i);
    res.p[2][2] = cos(i);
    res.p[2][0] = -sin(i);
    res.p[0][2] = sin(i);
    return res;
}

// Devuelve una matriz para rotar en el eje z en base a i
Matrix rotateZ(const double i){
    //cos(i)    -sin(i)     0       0
    //sin(i)    cos(i)      0       0
    //0         0           1       0
    //0         0           0       1

    Matrix res = Matrix();
    res.p[2][2] = 1;
    res.p[3][3] = 1;
    res.p[0][0] = cos(i);
    res.p[1][1] = cos(i);
    res.p[0][1] = -sin(i);
    res.p[1][0] = sin(i);
    return res;
}

// Crea una matriz para la base representada por u,v, w y o
Matrix originalBase(const dir u, const dir v, const dir w, const point o){
    //u.x       v.x         w.x       o.x
    //u.y       v.y         w.y       o.y
    //u.z       v.z         w.z       o.z
    //0         0           0         1

    Matrix res = Matrix();
    res.p[0][0] = u.x;
    res.p[1][0] = u.y;
    res.p[2][0] = u.z;


    res.p[0][1] = v.x;
    res.p[1][1] = v.y;
    res.p[2][1] = v.z;


    res.p[0][2] = w.x;
    res.p[1][2] = w.y;
    res.p[2][2] = w.z;


    res.p[0][3] = o.x;
    res.p[1][3] = o.y;
    res.p[2][3] = o.z;


    res.p[3][3] = 1;

    return res;

}

// Crea una matriz para pasar a la nueva base representada por u,v, w y o
Matrix newBase(const dir u, const dir v, const dir w, const point o){
    Matrix res = inverse(originalBase(u,v,w,o));

    return res;
}



#endif