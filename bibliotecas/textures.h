#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "punto_direccion.h"
#include <iostream>
#include <math.h>
#include "perlinNoise.h"
#include "ppm_reader.h"
#include <list>
#include "matrix.h"

using namespace std;

enum texture_enum { NO_TEXTURE, WOOD, PERLIN_NOISE, IMAGE};

class texture{
protected:
	int cr = 256;
public:
    texture(){}
	texture(int _cr){cr = _cr;}

	virtual int getR(point p, int r){ return r;}
    virtual int getG(point p, int g){ return g;}
    virtual int getB(point p, int b){ return b;}

	virtual int getR(point c, point p, int r){ return r;}
    virtual int getG(point c, point p, int g){ return g;}
    virtual int getB(point c, point p, int b){ return b;}

	virtual int getR(point c, point p){ return cr-1;}
    virtual int getG(point c, point p){ return cr-1;}
    virtual int getB(point c, point p){ return cr-1;}
};

//Madera
class texture1 : public texture {
private:
	perlinNoise pn;
public:
	texture1():texture(){
		pn = perlinNoise();
	}
	texture1(int _cr):texture(_cr){
		pn = perlinNoise();
	}

	int getR(point p, int r) override {
		double n = 20 * pn.noise(p.x/10, p.y/10, p.z/10);
		n = n - floor(n);
		return floor(r * n);
	}
    int getG(point p, int g) override {
		double n = 20 * pn.noise(p.x/10, p.y/10, p.z/10);
		n = n - floor(n);
		return floor(g * n);
	}
    int getB(point p, int b) override {
		double n = 20 * pn.noise(p.x/10, p.y/10, p.z/10);
		n = n - floor(n);
		return floor(b * n);
	}
};

//Perlin_noise
class texture2 : public texture {
private:
	perlinNoise pn;
public:
	texture2():texture(){
		pn = perlinNoise();
	}
	texture2(int _cr):texture(_cr){
		pn = perlinNoise();
	}

	int getR(point p, int r) override {
		double n = pn.noise(p.x/10, p.y/10, p.z/10);
		return floor(r * n);
	}
    int getG(point p, int g) override {
		double n = pn.noise(p.x/10, p.y/10, p.z/10);
		return floor(g * n);
	}
    int getB(point p, int b) override {
		double n = pn.noise(p.x/10, p.y/10, p.z/10);
		return floor(b * n);
	}
};

//Imagen
class texture3 : public texture {
    int **Rs;
    int **Gs;
    int **Bs;
    int width, height;
    dir n;
public:
    texture3(string im, dir _n):texture(){
        n = _n;
        list<int*> pixels = ppmReader(im);
        width = pixels.front()[0];
        height = pixels.front()[1];
        pixels.pop_front();

        Rs = new int*[height];
        for (int i = 0; i < height; ++i){Rs[i] = new int[width];}
        Gs = new int*[height];
        for (int i = 0; i < height; ++i){Gs[i] = new int[width];}
        Bs = new int*[height];
        for (int i = 0; i < height; ++i){Bs[i] = new int[width];}

        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                Rs[i][j] = pixels.front()[0];
                Gs[i][j] = pixels.front()[1];
                Bs[i][j] = pixels.front()[2];
                pixels.pop_front();
            }
        }

    }
    ~texture3(){
        for (int i = 0; i < height; ++i){
            delete [] Rs[i];
        }
        delete [] Rs;
        for (int i = 0; i < height; ++i){
            delete [] Gs[i];
        }
        delete [] Gs;
        for (int i = 0; i < height; ++i){
            delete [] Bs[i];
        }
        delete [] Bs;
    }
    int getR(point c, point p) override {
        dir aux = c - p;
        int x = (int)aux.x%width;
        int y = (int)aux.y%height;
        if(n.x == 0 && n.z == 0){
            x = (int)aux.x%width;
            y = (int)aux.z%height;
        }
        else if(n.y == 0 && n.z == 0){
            x = (int)aux.z%width;
            y = (int)aux.y%height;
        }
        if(x < 0){x = width + x;}
        if(y < 0){y = height + y;}
        return Rs[y][x];
    }
    int getG(point c, point p) override {
        dir aux = c - p;
        int x = (int)aux.x%width;
        int y = (int)aux.y%height;
        if(n.x == 0 && n.z == 0){
            x = (int)aux.x%width;
            y = (int)aux.z%height;
        }
        else if(n.y == 0 && n.z == 0){
            x = (int)aux.z%width;
            y = (int)aux.y%height;
        }
        if(x < 0){x = width + x;}
        if(y < 0){y = height + y;}
        return Gs[y][x];
    }
    int getB(point c, point p) override {
        dir aux = c - p;
        int x = (int)aux.x%width;
        int y = (int)aux.y%height;
        if(n.x == 0 && n.z == 0){
            x = (int)aux.x%width;
            y = (int)aux.z%height;
        }
        else if(n.y == 0 && n.z == 0){
            x = (int)aux.z%width;
            y = (int)aux.y%height;
        }
        if(x < 0){x = width + x;}
        if(y < 0){y = height + y;}
        return Bs[y][x];
    }
};
#endif
