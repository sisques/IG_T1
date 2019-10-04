#ifndef _TRANSFORMATIONS_H_
#define _TRANSFORMATIONS_H_

#include <math.h>
#include <punto_direccion.h>
#include <iostream>

using namespace std;

	void mulAux(const double m[4][4], double& x, double& y, double& z, double& w){
		double a, b, c, d;
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
	
	point detrans(const point p, const double x, const double y, const double z){
		double m[4][4] = { {1, 0, 0, -x},
						   {0, 1, 0, -y},
						   {0, 0, 1, -z},
						   {0, 0, 0, 1}
						};
		return m * p;
	}
	
	dir detrans(const dir d, const double x, const double y, const double z){
		double m[4][4] = { {1, 0, 0, -x},
						   {0, 1, 0, -y},
						   {0, 0, 1, -z},
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
	
	point descale(const point p, const double x, const double y, const double z){
		double m[4][4] = { {1/x, 0, 0, 0},
						   {0, 1/y, 0, 0},
						   {0, 0, 1/z, 0},
						   {0, 0, 0, 1}
						};
		return m * p;
	}
	
	dir descale(const dir d, const double x, const double y, const double z){
		double m[4][4] = { {1/x, 0, 0, 0},
						   {0, 1/y, 0, 0},
						   {0, 0, 1/z, 0},
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
	
	point basec(const dir u, const dir v, const dir w, const point o, const point p){
		double m[4][4] = { {u.x, v.x, w.x, o.x},
						   {u.y, v.y, w.y, o.y},
						   {u.z, v.z, w.z, o.z},
						   {0,	 0,	  0,   1}
						};
		return m * p;
	}
	
	dir basec(const dir u, const dir v, const dir w, const point o, const dir d){
		double m[4][4] = { {u.x, v.x, w.x, o.x},
						   {u.y, v.y, w.y, o.y},
						   {u.z, v.z, w.z, o.z},
						   {0,	 0,	  0,   1}
						};
		return m * d;
	}
	
	point desbasec(const dir u, const dir v, const dir w, const point o, const point p){
		double m[4][4] = { {u.x, v.x, w.x, o.x},
						   {u.y, v.y, w.y, o.y},
						   {u.z, v.z, w.z, o.z},
						   {0,	 0,	  0,   1}
						};
		double aux[4][4];
		inver(m,aux);
		return aux * p;
	}
	
	dir desbasec(const dir u, const dir v, const dir w, const point o, const dir d){
		double m[4][4] = { {u.x, v.x, w.x, o.x},
						   {u.y, v.y, w.y, o.y},
						   {u.z, v.z, w.z, o.z},
						   {0,	 0,	  0,   1}
						};
		double aux[4][4];
		inver(m,aux);
		return aux * d;
	}
	
	double det3x3(const double m[3][3]){
		return m[0][0]*m[1][1]*m[2][2] + m[0][1]*m[1][2]*m[2][0] + m[1][0]*m[2][1]*m[0][2]
			   - m[0][2]*m[1][1]*m[2][0] - m[1][2]*m[2][1]*m[0][0] - m[1][0]*m[0][1]*m[2][2];
	}
	
	double det4x4(const double m[4][4]){
		double det = 0.0;
		if(m[0][0] != 0){
			double aux[3][3] = { {m[1][1], m[1][2], m[1][3]},
						{m[2][1], m[2][2], m[2][3]},
						{m[3][1], m[3][2], m[3][3]}
						};
			det += m[0][0]*det3x3(aux);
		}
		if(m[1][0] != 0){
			double aux[3][3] = { {m[0][1], m[0][2], m[0][3]},
						{m[2][1], m[2][2], m[2][3]},
						{m[3][1], m[3][2], m[3][3]}
						};
			det -= m[1][0]*det3x3(aux);
		}
		if(m[2][0] != 0){
			double aux[3][3] = { {m[0][1], m[0][2], m[0][3]},
						{m[1][1], m[1][2], m[1][3]},
						{m[3][1], m[3][2], m[3][3]}
						};
			det += m[2][0]*det3x3(aux);
		}
		if(m[3][0] != 0){
			double aux[3][3] = { {m[0][1], m[0][2], m[0][3]},
						{m[1][1], m[1][2], m[1][3]},
						{m[2][1], m[2][2], m[2][3]}
						};
			det -= m[3][0]*det3x3(aux);
		}
		
		return det;
	}
	
	void adj(const double m[4][4], double adj[4][4]){
		for(int i = 0; i < 4; ++i){
			for(int j = 0; j < 4; ++j){
				double aux[3][3];
				int h = 0, k = 0;
				for(int x = 0; x < 4; ++x){
					for(int y = 0; y < 4 && h <3 && k < 3; ++y){
						if(y != j){
							aux[h][k] = m[x][y];
							++k;
						}
					}
					if(x != i){++h;}
					k = 0;
				}
				adj[i][j] = pow(-1.0, i+j)*det3x3(aux);
			}
		}
	}
	
	void trasp(const double m[4][4], double adj[4][4]){
		for(int i = 0; i < 4; ++i){
			for(int j = 0; j < 4; ++j){
				adj[i][j] = m[j][i];
			}
		}
	}
	
	void div(const double m[4][4], double x, double d[4][4]){
		for(int i = 0; i < 4; ++i){
			for(int j = 0; j < 4; ++j){
				d[i][j] = m[i][j]/x;
			}
		}
	}
	
	void inver(const double m[4][4], double inv[4][4]){
		double aux[4][4], aux2[4][4];
		adj(m,aux);
		trasp(aux, aux2);
		div(aux2, det4x4(m), inv);
	}
	
	#endif
