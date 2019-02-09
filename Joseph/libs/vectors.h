/////////////////////////////////////////////////////////////////////
// Author:		Anton Erymin										
// E-mail:		anton-erymin@mail.ru
// Version:		0.1
// Date:        2 July 2007
// Description:	Vectors, vertex and triangles structures and math functions.
//////////////////////////////////////////////////////////////////////

#ifndef vector_h
#define vector_h

#include <math.h>

#define PI_DIV_180	0.017453292

struct Vector
{
	GLfloat x, y, z;

	Vector() {};
	Vector(GLfloat _x, GLfloat _y, GLfloat _z):x(_x), y(_y), z(_z) {};
	Vector (GLfloat *xyz):x(xyz[0]), y(xyz[1]), z(xyz[2]) {};
	Vector operator+(Vector &v)
	{
		return Vector(x+v.x, y+v.y, z+v.z);
	}

	Vector operator-(Vector &v)
	{
		return Vector(x-v.x, y-v.y, z-v.z);
	}

	Vector operator*(float &c)
	{
		return Vector(c*x, c*y, c*z);
	}

	Vector operator/(float &c)
	{
		return Vector(x/c, y/c, z/c);
	}	

	void operator+=(Vector &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	void operator-=(Vector &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	void operator*=(float &c)
	{
		x *= c;
		y *= c;
		z *= c;
	}

	void operator/=(float &c)
	{
		x /= c;
		y /= c;
		z /= c;
	}
};

GLfloat Length(Vector v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector Normalize(Vector v)
{
	GLfloat l = Length(v); 
	v /= l;
	return Vector(v.x, v.y, v.z);
}

GLfloat DotProduct(Vector v1, Vector v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

Vector CrossProduct(Vector v1, Vector v2)
{
	GLfloat x = v1.y*v2.z - v1.z*v2.y;
	GLfloat y = -v1.x*v2.z + v1.z*v2.x;
	GLfloat z = v1.x*v2.y - v1.y*v2.x;
	
	//GLfloat x = v1.z*v2.y - v1.y*v2.z;
	//GLfloat y = -v1.x*v2.y + v1.y*v2.x;
	//GLfloat z = v1.x*v2.z - v1.z*v2.x;
	return Vector(x, y, z);
}


void Cross(float *u,float *v, float *res) 
{  
  res[0] = u[1]*v[2] - u[2]*v[1]; 
  res[1] = u[2]*v[0] - u[0]*v[2]; 
  res[2] = u[0]*v[1] - u[1]*v[0]; 
}

void Normalize(float *v, float *r)
{
	float l = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	r[0] = v[0]/l;
	r[1] = v[1]/l;
	r[2] = v[2]/l;
}

void RotateVector(float *v1, float *v2, float a) 
{ 
  v1[0] = v1[0]*cos(a*PI_DIV_180) - v2[0]*sin(a*PI_DIV_180); 
  v1[1] = v1[1]*cos(a*PI_DIV_180) - v2[1]*sin(a*PI_DIV_180); 
  v1[2] = v1[2]*cos(a*PI_DIV_180) - v2[2]*sin(a*PI_DIV_180); 
  Normalize(v1, v1);
};
#endif

