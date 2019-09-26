#IFNDEF _PUNTO&DIRECCION_H_
#DEFINE _PUNTO&DIRECCION_H_


class punto {
  public:
  float x,y,z,w;
	point(float _x, float _y, float _z) {
		this.x = _x;
		this.y = _y;
		this.z = _z;
		this.w = 1;	}
	

   
};

class dir {
  public:
	dir(float _x, float _y, float _z) {
		this.x = _x;
		this.y = _y;
		this.z = _z;
		this.w = 0;
	}
};


	point operator +(point p, point q)         {     
		return point(p.x + q.x, p.y + q.y, p.z + q.z);     
	}   
	 
	/**
	* p es el punto de origen 
	* q es el punto de destino
	*/
	dir operator -(point p, point q)         {     
		return dir(q.x - p.x, q.y - p.y, q.z - p.z);   
	} 

	point operator +(point p, dir q)         {     
		return point(p.x + q.x, p.y + q.y, p.z + q.z);     
	}   

	dir operator +(dir p, dir q)         {     
		return dir(p.x + q.x, p.y + q.y, p.z + q.z);     
	}   

	dir operator -(dir p, dir q)         {     
		return dir(p.x - q.x, p.y - q.y, p.z - q.z);     
	}   

	dir operator *(dir p, float q)         {     
		return dir(p.x*q, p.y*q, p.z*q);     
	}   

	dir operator /(dir p, float q)         {     
		return dir(p.x/q, p.y/q, p.z/q);     
	}   

	float modulus(dir d){
		return sqrt( pow(d.x,2) + pow(d.y,2) + pow(d.z,2) );
	}
	
	float dot(dir d, dir e) {
		return d.x*e.x + d.y*e.y + d.z*e.z;
	}
	
	float dot(dir d, dir e, float beta) {
		return modulus(d)*modulus(e)*cos(beta);
	}
	   
	   
	float cross(dir d, dir e) {
		return dir(d.y*e.z - d.z*e.y, d.z*e.x - d.x*e.z, d.x*e.y -d.y*e.x);
	}
   

#ENDIF