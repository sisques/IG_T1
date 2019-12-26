#ifndef _SCENES_H_
#define _SCENES_H_

#include "../bibliotecas/punto_direccion.h"
#include "../bibliotecas/figuras.h"
#include "../bibliotecas/ply_reader.h"




list<shared_ptr<figura>> scene_1(){

    double reflection[] = {0,0.0,0.9};
    double limit[] = {0.0,0.0,0.0};
    double wall[] = {0.90,0.0,0.0};

    event_enum eventos[] = {PHONG, REFRACTION, REFLEXION };
    materialProperties mp = materialProperties(false, eventos, wall);
    mp.setAlfa(0);

    materialProperties light = materialProperties(true, eventos, wall,1);
    light.setKs(255,255,255);

    materialProperties reflexion = materialProperties(false, eventos, reflection,1.33);
    reflexion.setKdPhong(255,255,255);
    reflexion.setKsPhong(255,255,255);
    reflexion.setKs(255,255,255);
    reflexion.setAlfa(10);



    string human = "/home/victor/gitRepos/IG_T1/models/low_poly_tree.ply";

    Matrix transformation[] = { rotateZ(M_PI/4),
                                rotateY(M_PI),
                                rotateX(M_PI/2),
                                translate(-0.1,-0.45,0.65)};


    materialProperties mp_human = materialProperties(false, eventos, wall);
    mp_human.setAlfa(1);
    list<shared_ptr<figura>> elementos = plyReader(human,mp_human, transformation,4);



    light.setKs(255,255,255);

    mp.setKdPhong(255, 255, 255);
    mp.setKsPhong(255, 255, 255);
    shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,1), newDir(0,0,-1), mp));
    shared_ptr<figura> suelo = make_shared<plano>(plano(newPoint(0,-0.5,0), newDir(0,1,0),mp));
    shared_ptr<figura> techo = make_shared<plano>(plano(newPoint(0,0.5,0), newDir(0,-1,0),light));
    materialProperties li = materialProperties(false, eventos, limit);

    shared_ptr<figura> limite = make_shared<plano>(plano(newPoint(0,0,0), newDir(0,0,-1), mp));

    mp.setKdPhong(255,0,0);
    mp.setKsPhong(255,0,0);
    shared_ptr<figura> izquierda = make_shared<plano>(plano(newPoint(-0.5,0,0), newDir(1,0,0),mp));

    mp.setKdPhong(0,255,0);
    mp.setKsPhong(0,255,0);
    shared_ptr<figura> derecha = make_shared<plano>(plano(newPoint(0.5,0,0), newDir(-1,0,0),mp));



    shared_ptr<figura> reflexion_1 = make_shared<esfera>(newPoint(0.3,-0.3,0.8), 0.2, reflexion);


    point p1 = newPoint(-0.5,0.2,0.75);
    point p2 = newPoint(-0.5,-0.5,0.75);
    point p3 = newPoint(-0.1,0.2,1);
    point p4 = newPoint(-0.1,-0.5,1);

    shared_ptr<figura> espejo_1 = make_shared<triangulo>(triangulo(p1,p2,p3,reflexion));

     shared_ptr<figura> espejo_2 = make_shared<triangulo>(triangulo(p2,p3,p4,reflexion));

    elementos.push_back(espejo_1);
    elementos.push_back(espejo_2);

    elementos.push_back(fondo);
    elementos.push_back(suelo);
    elementos.push_back(techo);
    elementos.push_back(izquierda);
    elementos.push_back(derecha);



    elementos.push_back(reflexion_1);






    elementos.push_back(limite);

    return elementos;
}



list<shared_ptr<figura>> scene_2(){



    double reflection[] = {0,0,0.90};
    double refraction[] = {0,0.90,0.05};
    double wall[] = {0.90,0,0};
    event_enum eventos[] = {PHONG, REFRACTION, REFLEXION };
    materialProperties mp = materialProperties(false, eventos, wall);
    mp.setAlfa(0);

    materialProperties light = materialProperties(true, eventos, wall,VACUUM);
    light.setKs(255,255,255);

    materialProperties refraccion = materialProperties(false, eventos, refraction,VACUUM);
    refraccion.setKd(255,255,255);
    refraccion.setKs(1,1,1);

    materialProperties reflexion = materialProperties(false, eventos, reflection,VACUUM);
    reflexion.setKdPhong(255,255,255);
    reflexion.setKsPhong(255,255,255);
    reflexion.setKs(255,255,255);
    reflexion.setAlfa(10);




    mp.setKdPhong(255, 255, 255);
    mp.setKsPhong(255, 255, 255);
    shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,1), newDir(0,0,-1), mp));
    shared_ptr<figura> suelo = make_shared<plano>(plano(newPoint(0,-0.5,0), newDir(0,1,0),mp));
    shared_ptr<figura> techo = make_shared<plano>(plano(newPoint(0,0.5,0), newDir(0,-1,0),light));


    mp.setKdPhong(255,0,0);
    mp.setKsPhong(255,0,0);
    shared_ptr<figura> izquierda = make_shared<plano>(plano(newPoint(-0.5,0,0), newDir(1,0,0),mp));

    mp.setKdPhong(0,255,0);
    mp.setKsPhong(0,255,0);
    shared_ptr<figura> derecha = make_shared<plano>(plano(newPoint(0.5,0,0), newDir(-1,0,0),mp));




    mp.setKdPhong(255,0,0);
    mp.setKsPhong(255,0,0);
    shared_ptr<figura> refrac = make_shared<esfera>(newPoint(0,-0.3,0.6), 0.2, refraccion);
    shared_ptr<figura> phong1 = make_shared<esfera>(newPoint(-0.3,-0.3,0.8), 0.2, mp);

    mp.setKdPhong(0,255,0);
    mp.setKsPhong(0,255,0);
    shared_ptr<figura> phong2 = make_shared<esfera>(newPoint(0.3,-0.3,0.8), 0.2, mp);

    shared_ptr<figura> reflec1 = make_shared<esfera>(newPoint(0.3,0.3,0.8), 0.2, reflexion);
    shared_ptr<figura> reflec2 = make_shared<esfera>(newPoint(-0.3,0.3,0.8), 0.2, reflexion);




    double cosas[]  = {0,0,0,0};
    materialProperties limit = materialProperties(false, eventos, cosas ); limit.setKdPhong(255,0,0); limit.setKsPhong(0,0,255);
    shared_ptr<figura> limite = make_shared<plano>(plano(newPoint(0,0,0), newDir(0,0,-1), limit));


    list<shared_ptr<figura>> elementos;
    elementos.push_back(limite);
    elementos.push_back(fondo);
    elementos.push_back(suelo);
    elementos.push_back(techo);
    elementos.push_back(izquierda);
    elementos.push_back(derecha);
    elementos.push_back(refrac);
    elementos.push_back(phong1);
    elementos.push_back(phong2);
    elementos.push_back(reflec1);
    elementos.push_back(reflec2);



    return elementos;
}



list<shared_ptr<figura>> scene_3(){



    double reflection[] = {0,0,0.90};
    double refraction[] = {0,0.90,0.05};
    double wall[] = {0.90,0,0};


    event_enum eventos[] = {PHONG, REFRACTION, REFLEXION };
    materialProperties phong = materialProperties(false, eventos, wall,VACUUM);
    phong.setAlfa(1);

    materialProperties luz = materialProperties(true, eventos, wall,VACUUM);
    luz.setKs(255,255,255);

    luz.setAlfa(10);

    materialProperties refraccion = materialProperties(false, eventos, refraction,GLASS);

    refraccion.setKd(255,255,255);
    refraccion.setKs(10,10,10);
    refraccion.setKdPhong(255,255,255);
    refraccion.setKsPhong(255,255,255);

    materialProperties reflexion = materialProperties(false, eventos, reflection,WATER);

    reflexion.setKs(255,255,255);







    phong.setKdPhong(255,255,255);
    phong.setKsPhong(255,255,255);

    shared_ptr<figura> techo = make_shared<plano>(plano(newPoint(0,0.5,0), newDir(0,-1,0), luz));
    shared_ptr<figura> suelo = make_shared<plano>(plano(newPoint(0,-0.5,0), newDir(0,1,0), phong));
    shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,1), newDir(0,0,-1), phong));
    phong.setKdPhong(0, 209, 255);
    phong.setKsPhong(0, 209, 255);


    shared_ptr<figura> izda = make_shared<plano>(plano(newPoint(0.5,0,0), newDir(1,0,0), phong));
    shared_ptr<figura> dcha = make_shared<plano>(plano(newPoint(-0.5,0,0), newDir(-1,0,0), phong));

    phong.setKdPhong(0, 209, 0);
    phong.setKsPhong(0, 209, 0);
    shared_ptr<figura> ESFERAphong = make_shared<esfera>(newPoint(0.1,0.1,0.5), 0.1, phong);

    shared_ptr<figura> ESFERArefraccion = make_shared<esfera>(newPoint(-0.2,-0.2,0.7), 0.3, refraccion);

    shared_ptr<figura> ESFERAreflexion = make_shared<esfera>(newPoint(0.3,-0.3,0.8), 0.2, reflexion);


    list<shared_ptr<figura>> elementos;

/*
    point p1 = newPoint(-0.25,0.5,0.25);
    point p2 = newPoint(0.25,0.5,0.25);
    point p3 = newPoint(-0.25,0.5,0.75);
    point p4 = newPoint(0.25,0.5,0.75);
    shared_ptr<figura> lght_src_1 = make_shared<triangulo>(triangulo(p1,p2,p3,luz));
    shared_ptr<figura> lght_src_2 = make_shared<triangulo>(triangulo(p2,p3,p4,luz));

    elementos.push_back(lght_src_1);
    elementos.push_back(lght_src_2);
*/

    elementos.push_back(fondo);
    elementos.push_back(suelo);
    elementos.push_back(techo);
    elementos.push_back(izda);
    elementos.push_back(dcha);
    elementos.push_back(ESFERArefraccion);
    elementos.push_back(ESFERAphong);

    elementos.push_back(ESFERAreflexion);



    return elementos;
}


list <shared_ptr<figura>> scene_4(){

    double reflection[] = {0,0.0,0.9};
    double refraction[] = {0.0,0.9,0.0};
    double wall[] = {0.90,0.0,0.0};
    event_enum eventos[] = {PHONG, REFRACTION, REFLEXION };
    materialProperties mp = materialProperties(false, eventos, wall);
    mp.setAlfa(0);
    materialProperties light = materialProperties(true, eventos, wall,1);
    light.setKs(1.0,1.0,1.0);
    materialProperties reflexion = materialProperties(false, eventos, reflection,1.33);
    //reflexion.setKd(0,0,0);
    reflexion.setKs(1.0,1.0,1.0);
    reflexion.setKdPhong(1.0,1.0,1.0);
    reflexion.setKsPhong(1.0,1.0,1.0);
    reflexion.setAlfa(10);
    materialProperties refraccion = materialProperties(false, eventos, refraction,WATER);
    refraccion.setKd(1.0,1.0,1.0);
    refraccion.setKs(0.0,0.0,0.0);
    refraccion.setKdPhong(0.0,0.0,0.0);
    refraccion.setKsPhong(0.0,0.0,0.0);


    list<shared_ptr<figura>> elementos;
    shared_ptr<figura> puntoLuz = make_shared<punto>(punto(newPoint(0,0.49,0.5), light));
    //mp.setKd(120, 120, 120);
    //mp.setKs(120, 120, 120);
    mp.setKdPhong(0.5, 0.5, 0.5);
    mp.setKsPhong(0.5, 0.5, 0.5);
    shared_ptr<figura> fondo = make_shared<plano>(plano(newPoint(0,0,1), newDir(0,0,-1), mp));
    shared_ptr<figura> suelo = make_shared<plano>(plano(newPoint(0,-0.5,0), newDir(0,1,0),mp));
    shared_ptr<figura> techo = make_shared<plano>(plano(newPoint(0,0.5,0), newDir(0,-1,0),light));
    //mp.setKd(255,0,0);
    //mp.setKs(255,0,0);
    mp.setKdPhong(1.0,0.0,0.0);
    mp.setKsPhong(1.0,0.0,0.0);
    shared_ptr<figura> izquierda = make_shared<plano>(plano(newPoint(-0.5,0,0), newDir(1,0,0),mp));
    //mp.setKd(0,255,0);
    //mp.setKs(0,255,0);
    mp.setKdPhong(0.0,1.0,0.0);
    mp.setKsPhong(0.0,1.0,0.0);
    shared_ptr<figura> derecha = make_shared<plano>(plano(newPoint(0.5,0,0), newDir(-1,0,0),mp));
    shared_ptr<figura> ESFERArefraccion = make_shared<esfera>(newPoint(0.0,0.0,0.5), 0.15, refraccion);
    shared_ptr<figura> ESFERAreflexion = make_shared<esfera>(newPoint(0.3,-0.3,0.9), 0.2, reflexion);
    shared_ptr<figura> ESFERAreflexion2 = make_shared<esfera>(newPoint(0.3,-0.1,0.9), 0.2, reflexion);
    shared_ptr<figura> ESFERAreflexion3 = make_shared<esfera>(newPoint(-0.3,-0.15,0.7), 0.2, reflexion);
    shared_ptr<figura> ESFERAreflexion4 = make_shared<esfera>(newPoint(-0.3,-0.3,0.7), 0.2, reflexion);
    mp.setKdPhong(0.0,0.0,1.0);
    mp.setKsPhong(0.0,0.0,1.0);
    mp.setAlfa(4);
    shared_ptr<figura> ESFERAphong = make_shared<esfera>(newPoint(0,-0.4,0.8), 0.1, mp);
    shared_ptr<figura> ESFERAphong2 = make_shared<esfera>(newPoint(0.3,0.2,0.9), 0.1, mp);
    shared_ptr<figura> ESFERAphong3 = make_shared<esfera>(newPoint(-0.3,0.15,0.7), 0.1, mp);
    shared_ptr<figura> ESFERAphong4 = make_shared<esfera>(newPoint(0,0.35,0.8), 0.1, mp);
    shared_ptr<figura> ESFERAphong5 = make_shared<esfera>(newPoint(-0.15,0.275,0.75), 0.1, mp);
    shared_ptr<figura> ESFERAphong6 = make_shared<esfera>(newPoint(0.15,0.275,0.85), 0.1, mp);


    //elementos.push_back(puntoLuz);
    elementos.push_back(fondo);
    elementos.push_back(suelo);
    elementos.push_back(techo);
    elementos.push_back(izquierda);
    elementos.push_back(derecha);
    elementos.push_back(ESFERArefraccion);
    elementos.push_back(ESFERAreflexion);
    //elementos.push_back(ESFERAreflexion2);
    elementos.push_back(ESFERAreflexion3);
    //elementos.push_back(ESFERAreflexion4);
    elementos.push_back(ESFERAphong);
    elementos.push_back(ESFERAphong2);
    elementos.push_back(ESFERAphong3);
    elementos.push_back(ESFERAphong4);
    elementos.push_back(ESFERAphong5);
    elementos.push_back(ESFERAphong6);


    double brdfValues2[] = {0.0,0.0,0.0};
    materialProperties limit = materialProperties(false, eventos, brdfValues2,0);
    //limit.setKd(0,0,0);
    //limit.setKs(0,0,0);
    //limit.setKdPhong(0,0,0);
    //limit.setKsPhong(0,0,0);
    shared_ptr<figura> limite = make_shared<plano>(plano(newPoint(0,0,0), newDir(0,0,-1), limit));

   // elementos.push_back(limite);
    return elementos;

}

list<shared_ptr<figura>> scene_5 (){



    double reflection[] = {0.05,0,0.90};
    double refraction[] = {0,0.90,0.05};
    double wall[] = {0.90,0,0};


    event_enum eventos[] = {PHONG, REFRACTION, REFLEXION };
    materialProperties phong = materialProperties(false, eventos, wall,VACUUM);
    phong.setAlfa(1);

    materialProperties luz = materialProperties(true, eventos, wall,VACUUM);
    luz.setKs(1,1,1);

    luz.setAlfa(10);

    materialProperties refraccion = materialProperties(false, eventos, refraction,GLASS);

    refraccion.setKd(1,1,1);
    refraccion.setKs(0.1,0.1,0.1);
    refraccion.setKdPhong(1,1,1);
    refraccion.setKsPhong(1,1,1);

    materialProperties reflexion = materialProperties(false, eventos, reflection,WATER);

    reflexion.setKs(1,1,1);






    shared_ptr<figura> SKYBOX = make_shared<esfera>(newPoint(0,0,0), 10, phong);
    shared_ptr<figura> puntoLuz_1 = make_shared<punto>(punto(newPoint(0,1,3), luz));
    shared_ptr<figura> puntoLuz_2 = make_shared<punto>(punto(newPoint(-2,-1,5), luz));
    shared_ptr<figura> puntoLuz_3 = make_shared<punto>(punto(newPoint(5,2,7), luz));
    phong.setKdPhong(0,0,1);
    phong.setKsPhong(0,0,1);
    shared_ptr<figura> TIERRA = make_shared<esfera>(newPoint(0,10,0), 2, luz);


    string human = "/home/victor/gitRepos/IG_T1/models/Sputnik3.ply";

    int numElem = 4;
    Matrix transformation[4] = { rotateZ(M_PI/4),
                                rotateY(M_PI),
                                rotateX(M_PI/2),
                                translate(-0.1,-0.45,0.65)};


    list<shared_ptr<figura>> elementos = plyReader(human,reflexion, transformation,numElem);




    elementos.push_back(SKYBOX);
/*
    elementos.push_back(puntoLuz_1);
    elementos.push_back(puntoLuz_2);
    elementos.push_back(puntoLuz_3);

    elementos.push_back(TIERRA);
*/





    return elementos;
}

#endif



