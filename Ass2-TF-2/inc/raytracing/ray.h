#pragma once
#include"vector.h"
#include"mat.h"
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
	void GetRay();
	f3Vector InterPoint(float t);


};