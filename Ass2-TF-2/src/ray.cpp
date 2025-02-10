#include "ray.h"
#include "vector.h"
void ray:: GetRay() {
	cout << "orig.e[0]=" << orig.x << ", orig.e[1]=" << orig.y << ", orig.e[2]=" << orig.z << endl;
	cout << "dir.e[0]=" << dir.x << ", dir.e[1]=" << dir.y << ", dir.e[2]=" << dir.z << endl;
}
f3Vector ray::InterPoint(float t) {//This function to calculate the point of intersection

	return orig + dir * t;

	//cout << "In ray, t=" <<t << ",  intpoint=(" << intepoint.x << "," << intepoint.y << "," << intepoint.z << ")" << endl;
}