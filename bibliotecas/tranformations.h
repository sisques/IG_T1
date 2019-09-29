#ifndef _TRANSFORMATIONS_H_
#define _TRANSFORMATIONS_H_

#include <math.h>
#include <punto_direccion.h>

	void mulAux(const double m[4][4], double& x, double& y, double& z, double& w){
		int a, b, c, d;
		a = m[0][0]*x + m[0][1]*y + m[0][2]*z + m[0][3]*w;
		b = m[1][0]*x + m[1][1]*y + m[1][2]*z + m[1][3]*w;
		c = m[2][0]*x + m[2][1]*y + m[2][2]*z + m[2][3]*w;
		d = m[3][0]*x + m[3][1]*y + m[3][2]*z + m[3][3]*w;
		x = a; y = b; z = c; w = d;
	}
	
	point operator *(const double m[4][4], const point p){     
		double a,b,c,d;
		a = p.x; b = p.y; c = p.z; d = p.w; 
		mulAux(m, a, b, c, d);
		return newPoint(a, b, c);
	}
	
	dir operator *(const double m[4][4], const dir dr){     
		double a,b,c,d;
		a = dr.x; b = dr.y; c = dr.z; d = dr.w; 
		mulAux(m, a, b, c, d);
		return newDir(a, b, c);
	}
	
	point trans(const point p, const double x, const double y, const double z){
		double m[4][4] = { {1, 0, 0, x},
						   {0, 1, 0, y},
						   {0, 0, 1, z},
						   {0, 0, 0, 1}
						};
		return m * p;
	}
	
	dir trans(const dir d, const double x, const double y, const double z){
		double m[4][4] = { {1, 0, 0, x},
						   {0, 1, 0, y},
						   {0, 0, 1, z},
						   {0, 0, 0, 1}
						};
		return m * d;
	}
	
	point scale(const point p, const double x, const double y, const double z){
		double m[4][4] = { {x, 0, 0, 0},
						   {0, y, 0, 0},
						   {0, 0, z, 0},
						   {0, 0, 0, 1}
						};
		return m * p;
	}
	
	dir scale(const dir d, const double x, const double y, const double z){
		double m[4][4] = { {x, 0, 0, 0},
						   {0, y, 0, 0},
						   {0, 0, z, 0},
						   {0, 0, 0, 1}
						};
		return m * d;
	}
	
	point rotaX(const point p, const double x){
		double m[4][4] = { {1, 0, 		0, 		0},
						   {0, cos(x), -sin(x), 0},
						   {0, sin(x),  cos(x), 0},
						   {0, 0,		 0,		1}
						};
		return m * p;
	}
	
	point rotaY(const point p, const double y){
		double m[4][4] = { {cos(y), 0, sin(y), 0},
						   {0, 		1, 0,	   0},
						   {-sin(y),0, cos(y), 0},
						   {0, 		0, 0,	   1}
						};
		return m * p;
	}
	
	point rotaZ(const point p, const double z){
		double m[4][4] = { {cos(z), -sin(z), 0, 0},
						   {sin(z), cos(z),  0, 0},
						   {0,		0,		 1, 0},
						   {0,		0,		 0, 1}
						};
		return m * p;
	}
	
	point rotat(const point p, const double x, const double y, const double z){
		point q = p;
		q = rotaX(q, x);
		q = rotaY(q, y);
		q = rotaZ(q, z);
		return q;
	}
	
	dir rotaX(const dir d, const double x){
		double m[4][4] = { {1, 0, 		0, 		0},
						   {0, cos(x), -sin(x), 0},
						   {0, sin(x),  cos(x), 0},
						   {0, 0,		 0,		1}
						};
		return m * d;
	}
	
	dir rotaY(const dir d, const double y){
		double m[4][4] = { {cos(y), 0, sin(y), 0},
						   {0, 		1, 0,	   0},
						   {-sin(y),0, cos(y), 0},
						   {0, 		0, 0,	   1}
						};
		return m * d;
	}
	
	dir rotaZ(const dir d, const double z){
		double m[4][4] = { {cos(z), -sin(z), 0, 0},
						   {sin(z), cos(z),  0, 0},
						   {0,		0,		 1, 0},
						   {0,		0,		 0, 1}
						};
		return m * d;
	}
	
	dir rotat(const dir d, const double x, const double y, const double z){
		dir q = d;
		q = rotaX(q, x);
		q = rotaY(q, y);
		q = rotaZ(q, z);
		return q;
	}
	
	point basec(const dir u, const dir v, const dir w, const point o, const point d){
		double m[4][4] = { {u.x, v.x, w.x, o.x},
						   {u.y, v.y, w.y, o.y},
						   {u.z, v.z, w.z, o.z},
						   {0,	 0,	  0,   1}
						};
		return m * d;
	}
	
	dir basec(const dir u, const dir v, const dir w, const point o, const dir d){
		double m[4][4] = { {u.x, v.x, w.x, o.x},
						   {u.y, v.y, w.y, o.y},
						   {u.z, v.z, w.z, o.z},
						   {0,	 0,	  0,   1}
						};
		return m * d;
	}
	
	#endif
