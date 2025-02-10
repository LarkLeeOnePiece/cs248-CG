#pragma once
#include"vector.h"
class ray {
public:
	f3Vector orig;
	f3Vector dir;

	ray(f3Vector origin, f3Vector pixel) {
		double normal = 0;
		orig = origin;
		dir = pixel - origin;
		dir.normalize();
	}
	ray() {
		orig = f3Vector(0, 0, 0);
		dir  = f3Vector(0, 0, 0);
	}
	void GetRay() {
		cout << "orig.e[0]=" << orig.x << ", orig.e[1]=" << orig.y << ", orig.e[2]=" << orig.z << endl;
		cout << "dir.e[0]=" << dir.x << ", dir.e[1]=" << dir.y << ", dir.e[2]=" << dir.z << endl;
	}
	f3Vector InterPoint(float t) {//This function to calculate the point of intersection

		return orig + dir*t;
		
		//cout << "In ray, t=" <<t << ",  intpoint=(" << intepoint.x << "," << intepoint.y << "," << intepoint.z << ")" << endl;
	}

};