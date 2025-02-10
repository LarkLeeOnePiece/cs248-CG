#pragma once
#include"vector.h"
#include"ray.h"

#include<vector>
f3Vector viewPoint;
#define INFINITE 1000000
#define OBJECTNUM 4
#define Sigma 0.1 //small value for mirror andshadow
class HITPOINT //the class to save the hit point information surfacetype, solver num, t1,t2
{
	public:
		int surfacetype;
		int solvers;
		float t1;
		float t2;
		f3Vector MaterialColors;
		HITPOINT() {
			surfacetype = 0;
			solvers = 0;
			t1 = INFINITE;
			t2 = INFINITE;
			MaterialColors=f3Vector(0,0,0);
	}
		void info() {
			cout << "HIPOINT:(" << surfacetype << "," << solvers << "," << t1 << "," << t2 << ")" << endl;
		}
		void intialize() {
			surfacetype = 0;
			solvers = 0;
			t1 = INFINITE;
			t2 = INFINITE;
			MaterialColors = f3Vector(0, 0, 0);
		}
};
float COS(f3Vector vec1, f3Vector vec2) {
	double cos = 0;
	cos = vec1*vec2 / (vec1.norm() * vec2.norm());
	return cos;
}
float MAXfloat(float f1, float f2) {
	return (f1 > f2) ? f1 : f2;
}
float MINfloat(float f1, float f2) {
	return (f1 > f2) ? f2 : f1;
}
void ColorSpace(f3Vector &color) {
	if (color.x >= 1) {
		color.x = 1;
	}
	if (color.y >= 1) {
		color.y = 1;
	}
	if (color.z >= 1) {
		color.z = 1;
	}
}


f3Vector cRed = f3Vector(1, 0, 0);//the triple for color disfuse
f3Vector cGre = f3Vector(0, 1, 0);
f3Vector cBlu = f3Vector(0, 0, 1);
f3Vector PlaneColor(0.5, 0.5, 0.5);
f3Vector SphereColor(1, 0, 0);
f3Vector ConeColor(0, 1, 0);
f3Vector EllipsoidColor(0, 0, 1);
f3Vector cL = f3Vector(0.1, 0.1, 0.1);//the triple for light intensity  !!!!!Important if it is too large,it might occur some value overleak.
f3Vector LightSource = f3Vector(0, 5, -3);//the triple for light position
f3Vector LightSource2 = f3Vector(0, 5, 5);//the triple for light position
f3Vector LightSource3 = f3Vector(5, 5, 5);//the triple for light position
vector<f3Vector> Lights = { LightSource ,LightSource2,LightSource3 };
float planeY = -1.4;
float ViewPlaneZ = 0;
float PhongExp = 32;
float Ia = 0.2;//used for ambient shading



