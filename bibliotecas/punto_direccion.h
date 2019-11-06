#ifndef _PUNTO_DIRECCION_H_
#define _PUNTO_DIRECCION_H_

#include <cmath>

struct point {
	public:
		double x;
		double y;
		double z;
		double w;
};

struct dir {
  public:
    	double x;
		double y;
		double z;
		double w;
};

	point newPoint(double _x, double _y, double _z) {
		point p;
		p.x = _x;
		p.y = _y;
		p.z = _z;
        p.w = 1;
		return p;
	}

	dir newDir(double _x, double _y, double _z) {
		dir p;
		p.x = _x;
		p.y = _y;
		p.z = _z;
        p.w = 0;
		return p;
	}


	point operator +(point p, point q)         {     
		return newPoint(p.x + q.x, p.y + q.y, p.z + q.z);     
	}   
	 
	dir operator -(point p, point q)         {     
		return newDir(p.x - q.x, p.y - q.y, p.z - q.z);   
	} 

	point operator +(point p, dir q)         {     
		return newPoint(p.x + q.x, p.y + q.y, p.z + q.z);     
	}

    point operator -(point p, dir q)         {
	    return newPoint(p.x - q.x, p.y - q.y, p.z - q.z);
    }

    point normalize(point p){
		if(p.w == 1){
			return p;
		}
		else {
			return newPoint(p.x/p.w, p.y/p.w, p.z/p.w);
		}
	}

	dir operator +(dir p, dir q)         {     
		return newDir(p.x + q.x, p.y + q.y, p.z + q.z);     
	}   

	dir operator -(dir p, dir q)         {     
		return newDir(p.x - q.x, p.y - q.y, p.z - q.z);     
	}   

	dir operator *( double q, dir p)         {     
		return newDir(p.x*q, p.y*q, p.z*q);     
	}

dir operator *( dir p, double q)         {
    return newDir(q*p.x, q*p.y, q*p.z);
}

dir operator /(dir p, double q)         {
		return newDir(p.x/q, p.y/q, p.z/q);     
	}   

	double mod(dir d){
		return sqrt( pow(d.x,2) + pow(d.y,2) + pow(d.z,2) );
	}

	double dist(point p, point q) {
		return mod(q-p);
	}
	
	double dot(dir d, dir e) {
		return d.x*e.x + d.y*e.y + d.z*e.z;
	}
	
	double angle(dir d, dir e) {
		return acos( ( dot(d,e) )/( mod(d)*mod(e) ) );
	}
	   
	   
	dir cross(dir d, dir e) {
		return newDir(d.y*e.z - d.z*e.y, d.z*e.x - d.x*e.z, d.x*e.y -d.y*e.x);
	}
   

#endif
