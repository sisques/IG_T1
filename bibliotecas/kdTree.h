#ifndef KD_TREE_KDTREE_2_H
#define KD_TREE_KDTREE_2_H
#include "kdTree_smallPM.h"



class kdTree{
private:
    KDTree<photon, 3> arbol;
public:

    kdTree() = default;



    kdTree(std::list<photon> lista){
        std::vector<float> pto;
        for( photon p : lista){

            pto.push_back(p.at(0));
            pto.push_back(p.at(1));
            pto.push_back(p.at(2));


            arbol.store(pto, p);
            pto.clear();
        }
        arbol.balance();
    }



    //Busca los n puntos mas cercanos al punto dado
    std::list<photon> fotonesCercanos(const photon& pt, const int nPuntos){
        using Node = KDTree<photon, 3>::Node;




        std::list<photon> output;


        std::vector<float> pto;
        pto.push_back(pt.at(0));
        pto.push_back(pt.at(1));
        pto.push_back(pt.at(2));

        int index = 0;

        float max_distance;

        std::vector<const Node*> nodos;

        std::vector<pair<int,float>> distancias;

        arbol.find( pto, nPuntos, nodos, max_distance);


        for( const Node *n : nodos) {
            output.push_back(n->data());
        }

        return output;
    }


};

#endif //KD_TREE_KDTREE_2_H
