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
        double get(int index) const{return getDimension(index,pto.p);}

        //Devuelve la diustancia entre el punto de este nodo y el pasado
        //por parametro
        double distance(const photon& pt) const{return dist(pt.p, pto.p);}

    };

    std::shared_ptr<node> raiz;
    std::shared_ptr<node> mejorNodo;
    double mejorDist;
    int nodosVisitados;
    int dimensiones = 3;
    std::vector<node> nodos;

    // Comparador de nodos (utilizado para la ordenacion)
    struct node_cmp{
        int index_;
        explicit node_cmp(int index) : index_(index){}
        bool operator()(const node& n1, const node& n2) const{
            return getDimension(index_,n1.pto.p) <  getDimension(index_, n2.pto.p);
        }
    };

    // Crea un arbol dados 2 indices y la profundidad actual
    std::shared_ptr<node> make_tree(int begin, int end, int index)    {
        if (end <= begin){
            return nullptr;
        }
        size_t n = begin + (end - begin)/2;
        std::nth_element(&nodos[begin], &nodos[n], &nodos[end], node_cmp(index));
        index = (index + 1) % dimensiones;
        nodos[n].left_ = make_tree(begin, n, index);
        nodos[n].right_ = make_tree(n + 1, end, index);
        return std::make_shared<node> (nodos[n]);
    }

    //Calcula el punto mas cercano al pasado como parametro
    void cercano(const std::shared_ptr<node>& root, const photon& nuevo, size_t index) {
        if (root == nullptr) {
            return;
        }
        ++nodosVisitados;
        double d = root->distance(nuevo);
        if (mejorNodo == nullptr || d < mejorDist) {
            mejorDist = d;
            mejorNodo = root;
        }
        if (mejorDist == 0){
            return;
        }
        double dx = root->get(index) - getDimension(index, nuevo.p);
        index = (index + 1) % dimensiones;
        cercano(dx > 0 ? root->left_ : root->right_, nuevo, index);
        if (dx * dx >= mejorDist) {
            return;
        }
        cercano(dx > 0 ? root->right_ : root->left_, nuevo, index);
    }

public:

    //Constructores de la clase
    kdtree(const kdtree&) = delete;
    kdtree& operator=(const kdtree& in){
        this->raiz = in.raiz;
        this->mejorNodo = nullptr;
        this->mejorDist = 0;
        this->nodosVisitados = 0;
        this->dimensiones = 3;
        this->nodos = in.nodos;

    }



    //Crea un arbol pasada una lista de fotones
    kdtree(std::list<photon> in){
        auto inicio = in.begin();
        auto fin = in.end();
        dimensiones = 3;
        mejorNodo = nullptr;
        mejorDist = 0;
        nodosVisitados = 0;
        nodos.reserve(std::distance(inicio, fin));
        for (auto i = inicio; i != fin; ++i) {
            nodos.emplace_back(*i);
        }
        raiz = make_tree(0, nodos.size(), 0);
    }



    //Devuelve cierto si el arbol esta vacio
    bool esVacio() const    {
        return nodos.empty();
    }

    //Devuelve el numero de nodos visitados en la llamada a cercano
    size_t getNodosVisitados() const    {
        return nodosVisitados;
    }

    //Devuelve la distancia entre el nuevo punto y su punto mas cercano tras llamar a cercano
    double getMejorDist() const    {
        return mejorDist;
    }


    //Busca el punto mas cercano al punto dado
    photon cercano(const photon& pt){
        if (raiz == nullptr) {
            std::cerr << "tree is empty" << std::endl;
        }
        mejorNodo = nullptr;
        nodosVisitados = 0;
        mejorDist= 0;
        cercano(raiz, pt, 0);
        return mejorNodo->pto;
    }

    //Busca los n puntos mas cercanos al punto dado
    std::list<photon> cercanos(const photon& pt, const std::list<photon> listaPuntos, const int nPuntos){
        if (raiz == nullptr) {
            std::cerr << "tree is empty" << std::endl;
        }
        std::list<photon> output;
        std::list<photon> puntosAux = listaPuntos;
        kdtree arbolAuxiliar(puntosAux);
        for(int i = 0; i < nPuntos; i++){
            arbolAuxiliar.mejorNodo = nullptr;
            arbolAuxiliar.nodosVisitados = 0;
            arbolAuxiliar.mejorDist= 0;
            arbolAuxiliar.cercano(arbolAuxiliar.raiz, pt, 0);
            photon puntoResul = arbolAuxiliar.mejorNodo->pto;
            output.push_back(puntoResul);
            puntosAux.remove(puntoResul);
            arbolAuxiliar = kdtree(puntosAux);
        }
        return output;
    }


};



#endif //_KDTREE_H
