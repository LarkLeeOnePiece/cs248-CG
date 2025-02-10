#pragma once
/*
Author: Da Li
Function: Header for vector operation
*/
#include"ofMain.h"
#include<iostream>
using namespace std;
class f3Vector
{

	public:
		float x;
		float y;
		float z;
		f3Vector::f3Vector() {
			x = 0.;
			y = 0.;
			z = 0.;
		}
		f3Vector::f3Vector(float fx , float fy , float fz ) {//NOTE:If I have two constrcutor you can make them do the same thing,like settin all as 0
			x = fx;
			y = fy;
			z = fz;
		}
		f3Vector operator+(const f3Vector& f3v);
		f3Vector operator-(const f3Vector& f3v);
		float operator*(const f3Vector& f3v);//dot product
		f3Vector operator*(float t);
		float norm();
		void normalize();
		void info();//Used for debugging
		ofVec3f toOfvec3();//Used for ofVec3
};
class Int3Vector
{
public:
	int x, y, z;
	Int3Vector(): x(0),y(0),z(0){}
	Int3Vector(int sx, int sy, int sz) :x(sx), y(sy), z(sz) {}
	Int3Vector operator+(const Int3Vector& f3v);
	Int3Vector operator-(const Int3Vector& f3v);
	Int3Vector operator*(const Int3Vector& f3v);
	Int3Vector operator*(float t);// care about the type of parameter if this function 
	float norm();
};
