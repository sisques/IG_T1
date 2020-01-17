//
// Created by victor on 14/1/20.
//

#ifndef KD_TREE_KDTREE_2_H
#define KD_TREE_KDTREE_2_H

#include <algorithm>


class kdNode{
public:
    photon data;
    kdNode* izda;
    kdNode* dcha;

    kdNode() = default;
    kdNode(photon _p):data(_p), izda(nullptr), dcha(nullptr){}
    kdNode(photon _p, kdNode* _izda, kdNode* _dcha):
            data(_p), izda(_izda), dcha(_dcha) {}

};

kdNode* newNode(photon p){
    kdNode* temp = new kdNode;
    temp->data = p;
    temp->izda = nullptr;
    temp->dcha = nullptr;
    return temp;
}

//https://www.cs.cmu.edu/~ckingsf/bioinfo-lectures/kdtrees.pdf

class kdTree{


private:
    kdNode* insertRec(kdNode* root, photon p, unsigned depth){
        //Si el arbol esta vacio
        if ( root == NULL ){
            return newNode(p);
        }

        unsigned cd = depth%DIM;

        //Comparar el nuevo punto con la raiz respecto de la
        //dimension actual y decidir entre el subarbol derecho
        //e izquierdo
        if(p.at(cd) < root->data.at(cd)){
            root->izda = insertRec(root->izda, p, depth+1);
        } else {
            root->dcha = insertRec(root->dcha, p, depth+1);
        }
        return root;
    }

    bool searchRec(kdNode* root, photon p, unsigned depth){
        //Casos base
        if( root == nullptr ){
            return false;
        } if ( p == root->data ){
            return true;
        }

        //La dimension actual se computa usando la profundidad
        //actual y las dimensiones totales
        unsigned cd = depth%DIM;

        //Comparar el punto con la raiz respecto de cd
        if(p.at(cd) < root->data.at(cd)){
            return searchRec(root->izda, p, depth+1);
        }
        return searchRec(root->dcha, p, depth+1);
    }


    kdNode* minNode(kdNode *x, kdNode *y, kdNode *z, int d){
        kdNode *resultado = x;
        if(y != nullptr && y->data.at(d) < resultado->data.at(d)){
            resultado = y;
        }
        if(z != nullptr && z->data.at(d) < resultado->data.at(d)){
            resultado = z;
        }
        return resultado;
    }

    kdNode* findMinRec(kdNode* root, int d, unsigned depth){
        //Casos base
        if(root == nullptr){
            return nullptr;
        }

        //La dimension actual se computa usando la profundidad
        //actual y las dimensiones totales
        unsigned  cd = depth%DIM;

        //Comprar el punto con la raiz respecto de cd
        if(cd == d){
            if(root->izda == nullptr){
                return root;
            }
            return findMinRec(root->izda, d, depth+1);
        }

        //Si el subarbol actual es diferente, el minimo puede
        //estar en cualquier otro lado
        return minNode(root,
                       findMinRec(root->izda, d, depth + 1),
                       findMinRec(root->dcha, d, depth + 1),
                       d);
    }

    kdNode* deleteNodeRec(kdNode* root, photon p, unsigned  depth){
        //El punto pasado como parametro no esta en el arbol
        if(root == nullptr){
            return nullptr;
        }
        // Se calcula la dimension del nodo actual
        unsigned cd = depth%DIM;

        //Se ha encontrado el punto a eliminar
        if(root->data == p){
            //Si el hijo derecho no es nulo
            if(root->dcha != nullptr){
                kdNode* min = findMinRec(root->dcha, cd, depth+1);
                root->data = min->data;
                root->dcha = deleteNodeRec(root->dcha, root->data, depth+1);
            } else if(root->izda != nullptr){
                kdNode* min = findMinRec(root->izda, cd, depth+1);
                root->data = min->data;
                root->dcha = deleteNodeRec(root->izda, root->data, depth+1);
                root->izda = nullptr;
            } else {
                delete root;
                return nullptr;
            }
            return root;
        }

        //El nodo actual no contiene el punto
        if(p.at(cd) < root->data.at(cd)){
            root->izda = deleteNodeRec(root->izda, p, depth+1);
        } else {
            root->dcha = deleteNodeRec(root->dcha, p, depth+1);
        }
        return root;
    }




    void printRec(kdNode* root, int depth, int pos){
        if(root == nullptr){
            std::cout << "" << std::endl;
        }
        std::cout << depth << std::flush;
        switch (pos){
            case 0:
                std::cout << " raiz " << std::flush;
                break;
            case 1:
                std::cout << " izda " << std::flush;
                break;
            case 2:
                std::cout << " dcha " << std::flush;
                break;
        }
        std::cout << " --> (" << root->data.p.x<< ", " <<  root->data.p.y << ", " << root->data.p.z << ")\t\t" << std::endl;
        if(root->izda != nullptr) {
            printRec(root->izda, depth + 1, 1);
        }
        if(root->dcha != nullptr){
            printRec(root->dcha, depth+1,2);
        }
    }


    //Calcula el punto mas cercano al pasado como parametro mediante recursion
    void nearestRec(kdNode* root, const photon& nuevo, int dimension) {
        if (root == nullptr) {
            return;
        }
        double d = dist(root->data, nuevo);
        if (mejorNodo == nullptr || d < mejorDist) {
            mejorDist = d;
            mejorNodo = root;
        }
        if (mejorDist == 0){
            return;
        }
        int cd = dimension%DIM;
        double dx = root->data.at(cd) - nuevo.at(cd);

        nearestRec(dx > 0 ? root->izda : root->dcha, nuevo, dimension + 1);
        if (dx * dx >= mejorDist) {
            return;
        }
        nearestRec(dx > 0 ? root->dcha : root->izda, nuevo, dimension + 1);
    }
public:
    kdNode* raiz;
    int DIM;
    double mejorDist;
    kdNode* mejorNodo;
    int numElem;

    kdNode* insert(photon p){
        numElem++;
        return insertRec(this->raiz,p,0);
    }


    bool search(photon p){
        return searchRec(this->raiz,p,0);
    }

    kdNode* findMin(kdNode* root, int d){
        return findMinRec(root, d,0);
    }

    photon findMin(int d){
        return findMin(this->raiz, d)->data;
    }

    // Elimina el photon p y devuelve la raiz del arbol
    kdNode* deleteNode( photon p){
        numElem--;
        return deleteNodeRec(this->raiz,p, 0);
    }
    // Elimina el photon p y devuelve la raiz del arbol
    void nearest(photon p){
        nearestRec(this->raiz,p, 0);
    }

    kdTree() = default;



    kdTree(std::list<photon> lista){
        DIM = 3;
        raiz = nullptr;
        mejorDist = std::numeric_limits<double>::max();
        mejorNodo = nullptr;
        for( photon p : lista){
            raiz = insert(p);
        }
    }


    kdTree(std::list<photon> lista, int d){
        DIM = d;
        raiz = nullptr;
        mejorDist = std::numeric_limits<double>::max();
        mejorNodo = nullptr;
        for( photon p : lista){
            raiz = insert(p);
        }
    }


    //Busca los n puntos mas cercanos al punto dado
    std::list<photon> fotonesCercanos(const photon& pt, const int nPuntos){

        std::list<photon> output;

        for(int i = 0; i < nPuntos; i++){

            if(this->raiz == nullptr){
                break;
            }
            this->mejorNodo = nullptr;
            this->mejorDist = std::numeric_limits<double>::max();

            this->nearest(pt);
            photon puntoResul = this->mejorNodo->data;
            output.push_back(puntoResul);

            this->raiz = this->deleteNode(puntoResul);



        }
        for(photon p : output){
            this->raiz = insert(p);
        }


        return output;
    }


    void print(){
        printRec(this->raiz, 0,0);
        std::cout << "----------" << std::endl;
    }

};


#endif //KD_TREE_KDTREE_2_H
