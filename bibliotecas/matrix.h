#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <cstdlib>
#include <cstdio>
#include <math.h>
#include "punto_direccion.h"


// Basado en ->http://www.speqmath.com/tutorials/matrix/matrix.html

class Matrix{
public:
    double** p;     // puntero a matriz de doubles


    // constructor
    Matrix(){
        p = new double*[4];
        for (int f = 0; f < 4; f++){
            p[f] = new double[4];
            for (int c = 0; c < 4; c++) {
                p[f][c] = 0;
            }
        }
    }



    // operador de asignacion
    Matrix& operator= (const Matrix& a){
        p = new double*[4];
        for (int f = 0; f < 4; f++) {
            p[f] = new double[4];
            for (int c = 0; c < 4; c++){
                p[f][c] = a.p[f][c];
            }
        }
        return *this;
    }




};

void mulAux(const Matrix& m, double& x, double& y, double& z, double& w){
    double a, b, c, d;
    a = m.p[0][0]*x + m.p[0][1]*y + m.p[0][2]*z + m.p[0][3]*w;
    b = m.p[1][0]*x + m.p[1][1]*y + m.p[1][2]*z + m.p[1][3]*w;
    c = m.p[2][0]*x + m.p[2][1]*y + m.p[2][2]*z + m.p[2][3]*w;
    d = m.p[3][0]*x + m.p[3][1]*y + m.p[3][2]*z + m.p[3][3]*w;
    x = a; y = b; z = c; w = d;
}


// operador de multiplicacion
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



// operador de multiplicacion por punto
point operator* (const Matrix& m, const point p)    {
    double a,b,c,d;
    a = p.x; b = p.y; c = p.z; d = p.w;
    mulAux(m, a, b, c, d);
    return newPoint(a, b, c);
}

// operador de multiplicacion por direccion
dir operator* (const Matrix& m, const dir& p)    {
    double a,b,c,d;
    a = p.x; b = p.y; c = p.z; d = p.w;
    mulAux(m, a, b, c, d);
    return newDir(a, b, c);
}

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


#endif