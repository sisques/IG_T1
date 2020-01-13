//
// Created by victor on 22/12/19.
//

#ifndef _KDTREE_H
#define _KDTREE_H

#include <algorithm>
#include <memory>
#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include "punto_direccion.h"
#include "photon.h"

// Devuelve la coordenada correspondiente a la dimension pasada como parametro
double getDimension(int dimension, point p){
    if (dimension % 3 == 0){
        return p.x;
    } else if (dimension % 3 == 1){
        return p.y;
    } else if (dimension % 3 == 2){
        return p.z;
    }
    return 0;
}





/**
 * Implementacion de arbol K-dimensional basado en la implementacion de https://rosettacode.org/wiki/K-d_tree#C.2B.2B
 */

class kdtree{
private:
    struct node{
        photon pto;
        std::shared_ptr<node> left_;
        std::shared_ptr<node> right_;

        //Costructor de la clase
        explicit node(const photon& pt) : pto(pt), left_(nullptr), right_(nullptr){}

        //Obtiene el valor de la dimension indicada como parametro
        double get(int dimension) const{return getDimension(dimension,pto.p);}

        //Devuelve la diustancia entre el punto de este nodo y el pasado
        //por parametro
        double distance(const point& pt) const{return dist(pt, pto.p);}
    };

    std::shared_ptr<node> raiz;
    std::shared_ptr<node> mejorNodo;
    double mejorDist;
    int nodosVisitados;
    int dimensiones = 3;

    // Comparador de nodos (utilizado para la ordenacion)
    struct node_cmp{
        int dimension;
        explicit node_cmp(int _dimension) : dimension(_dimension){}
        bool operator()(const node& n1, const node& n2) const{
            return getDimension(dimension,n1.pto.p) <  getDimension(dimension, n2.pto.p);
        }
    };



    std::shared_ptr<node> insert(std::shared_ptr<node> nodoActual, photon p, int depth) {
        if(nodoActual == nullptr){
            return make_shared<node>(node(p));
        }

        int cd = depth % dimensiones;

        double aux1 = getDimension(cd, p.p);
        double aux2 = getDimension(cd, nodoActual->pto.p);

        if( aux1 < aux2){
            nodoActual->left_ = insert(nodoActual->left_, p, depth+1);
        } else {
            nodoActual->right_ = insert(nodoActual->right_, p, depth+1);
        }
        return nodoActual;
    }

    std::shared_ptr<node> insert(std::shared_ptr<node> raiz, photon p){
        return insert(raiz, p, 0);
    }

    //Calcula el punto mas cercano al pasado como parametro mediante recursion
    void cercano(const std::shared_ptr<node>& nodoActual, const point& nuevo, int dimension) {
        if (nodoActual == nullptr) {
            return;
        }
        ++nodosVisitados;
        double d = nodoActual->distance(nuevo);
        if (mejorNodo == nullptr || d < mejorDist) {
            mejorDist = d;
            mejorNodo = nodoActual;
        }
        if (mejorDist == 0){
            return;
        }
        int cd = dimension%dimensiones;
        double dx = nodoActual->get(cd) - getDimension(cd, nuevo);

        cercano(dx > 0 ? nodoActual->left_ : nodoActual->right_, nuevo, dimension + 1);
        if (dx * dx >= mejorDist) {
            return;
        }
        cercano(dx > 0 ? nodoActual->right_ : nodoActual->left_, nuevo, dimension + 1);
    }

    //Calcula el punto mas cercano al pasado como parametro
    void cercano(const point& nuevo) {
        cercano(this->raiz, nuevo, 0);
    }




    std::shared_ptr<node> minimo(std::shared_ptr<node> x, std::shared_ptr<node> y, std::shared_ptr<node> z, int d){
        std::shared_ptr<node> resul = x;

        if(y != nullptr && getDimension(d, y->pto.p) < getDimension(d,resul->pto.p)){
            resul = y;
        }
        if(z != nullptr && getDimension(d, z->pto.p) < getDimension(d,resul->pto.p)){
            resul = z;
        }
        return resul;
    }

    std::shared_ptr<node> minimo(std::shared_ptr<node> nodoActual, int d, int depth){
        if(nodoActual == nullptr){
            return nullptr;
        }
        int cd = depth%dimensiones;

        if(cd == d){
            if(nodoActual->left_ == nullptr){
                return nodoActual;
            }
            return minimo(nodoActual->left_, d, depth + 1);
        }

        return minimo(nodoActual,
                      minimo(nodoActual->left_, d, depth + 1),
                      minimo(nodoActual->right_, d, depth + 1),
                      d);

    }

    std::shared_ptr<node> minimo(std::shared_ptr<node> nodoActual, int depth){
        return minimo(nodoActual, depth, 0);
    }

    //Devuelve la raiz del arbol modificado
    std::shared_ptr<node> remove(const std::shared_ptr<node>& nodoActual,const photon& p, int depth){
        if(nodoActual == nullptr){
            return nullptr;
        }
        int cd = depth%dimensiones;



        if( nodoActual->pto == p ){

            if(nodoActual->right_ != nullptr){
                std::shared_ptr<node> min = minimo(nodoActual->right_, cd);

                nodoActual->pto = min->pto;



                nodoActual->right_ = remove(nodoActual->right_, min->pto, depth + 1);
            } else if (nodoActual->left_ != nullptr){
                std::shared_ptr<node> min = minimo(nodoActual->left_, cd);

                nodoActual->pto = min->pto;


                nodoActual->left_ = remove(nodoActual->left_, min->pto, depth + 1);
            } else {
               return nullptr;
            }
            return nodoActual;
        }

        if(getDimension(cd, p.p) < getDimension(cd, nodoActual->pto.p)){
            nodoActual->left_ = remove(nodoActual->left_, p, depth + 1);
        } else {
            nodoActual->right_ = remove(nodoActual->right_, p, depth + 1);
        }


        return nodoActual;
    }


    //Calcula el punto mas cercano al pasado como parametro
    std::shared_ptr<node> remove(const photon& pt) {
        return remove(this->raiz, pt, 0);
    }



public:

    //Constructores de la clase
    kdtree(const kdtree&) = default;
	kdtree() = default;

    kdtree& operator=(const kdtree& in){
        this->raiz = in.raiz;
        this->mejorNodo = nullptr;
        this->mejorDist = 0;
        this->nodosVisitados = 0;
        this->dimensiones = 3;


    }



    //Crea un arbol pasada una lista de fotones
    kdtree(std::list<photon> in){
        dimensiones = 3;
        mejorNodo = nullptr;
        mejorDist = 0;
        nodosVisitados = 0;


        for (photon p : in) {
            raiz = insert(raiz, p);
        }
    }




    //Devuelve el numero de nodos visitados en la llamada a cercano
    int getNodosVisitados() const    {
        return nodosVisitados;
    }

    //Devuelve la distancia entre el nuevo punto y su punto mas cercano tras llamar a cercano
    double getMejorDist() const    {
        return mejorDist;
    }


    //Busca el punto mas cercano al punto dado
    photon fotonCercano(const point& pt){
        if (raiz == nullptr) {
            return photon();
        }
        mejorNodo = nullptr;
        nodosVisitados = 0;
        mejorDist= 0;
        cercano(pt);
        return mejorNodo->pto;
    }

    //Busca los n puntos mas cercanos al punto dado
    std::list<photon> fotonesCercanos(const point& pt, const int nPuntos){
		std::list<photon> output;
        if (raiz == nullptr) {
            return output;
        }
        kdtree arbolAuxiliar = *this;
        for(int i = 0; i < nPuntos; i++){
            if(arbolAuxiliar.raiz == nullptr){
                break;
            }
            arbolAuxiliar.mejorNodo = nullptr;
            arbolAuxiliar.nodosVisitados = 0;
            arbolAuxiliar.mejorDist= 0;
            arbolAuxiliar.cercano(pt);
            photon puntoResul = arbolAuxiliar.mejorNodo->pto;
            output.push_back(puntoResul);
            std::shared_ptr<node> nuevaRaiz= arbolAuxiliar.remove(puntoResul);
            arbolAuxiliar.raiz = nuevaRaiz;
        }
        return output;
    }


};



#endif //_KDTREE_H