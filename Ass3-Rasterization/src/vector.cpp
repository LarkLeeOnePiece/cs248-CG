/*
Author：Da Li
Function: Define operation for vector
*/
#include "vector.h"
#include <cmath>



void f3Vector::info() {
	cout << "(" << this->x << "," << this->y << "," << this->z << ")" << endl;
}
f3Vector f3Vector::operator+(const f3Vector& f3v) {
	f3Vector ftv;
	ftv.x = this->x + f3v.x;
	ftv.y = this->y + f3v.y;
	ftv.z = this->z + f3v.z;
	return ftv;
}
f3Vector f3Vector::operator-(const f3Vector& f3v) {
	f3Vector ftv;
	ftv.x = this->x - f3v.x;
	ftv.y = this->y - f3v.y;
	ftv.z = this->z - f3v.z;
	return ftv;
}
float f3Vector::operator*(const f3Vector& f3v) {
	float ftv;
	ftv = this->x * f3v.x+ this->y * f3v.y+ this->z * f3v.z;
	return ftv;
}
f3Vector f3Vector::operator*(float t) {
	f3Vector ftv;
	ftv.x = this->x * t;
	ftv.y = this->y * t;
	ftv.z = this->z * t;
	return ftv;
}
float f3Vector::norm() {
	float norm = sqrt((this->x) * (this->x) + (this->y) * (this->y) + (this->z) * (this->z));
	return norm;
}
void f3Vector::normalize() {
	float normsize = this->norm();
	this->x = this->x / normsize;
	this->y = this->y / normsize;
	this->z = this->z / normsize;
}
f3Vector f3Vector::crossProduct(f3Vector b) {
	f3Vector result;//cross product result
	result.x = this->y * b.z - this->z * b.y;
	result.y = this->z * b.x - this->x * b.z;
	result.z = this->x * b.y - this->y * b.x;
	return result;
}

ofVec3f f3Vector::toOfvec3() {
	ofVec3f newofVec;
	newofVec.x = this->x;
	newofVec.y = this->y;
	newofVec.z = this->z;
	return newofVec;
}

Int3Vector Int3Vector::operator+(const Int3Vector& f3v) {
	Int3Vector ftv;
	ftv.x = this->x + f3v.x;
	ftv.y = this->y + f3v.y;
	ftv.z = this->z + f3v.z;
	return ftv;
}
Int3Vector Int3Vector::operator-(const Int3Vector& f3v) {
	Int3Vector ftv;
	ftv.x = this->x - f3v.x;
	ftv.y = this->y - f3v.y;
	ftv.z = this->z - f3v.z;
	return ftv;
}
Int3Vector Int3Vector::operator*(const Int3Vector& f3v) {
	Int3Vector ftv;
	ftv.x = this->x * f3v.x;
	ftv.y = this->y * f3v.y;
	ftv.z = this->z * f3v.z;
	return ftv;
}
Int3Vector Int3Vector::operator*(float t) {
	Int3Vector ftv;
	ftv.x = this->x * t;
	ftv.y = this->y * t;
	ftv.z = this->z * t;
	return ftv;
}
float Int3Vector::norm() {
	float norm = sqrt((this->x) * (this->x) + (this->y) * (this->y) + (this->z) * (this->z));
	return norm;
}