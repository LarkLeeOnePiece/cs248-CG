#pragma once
#include"vector.h"
#include"globalSettings.h"
#define SPHERE 2
class sphere {
public:
	f3Vector center;
	float radius;
	float radius_2;
	f3Vector MaterialColors;
	sphere() {
		center = f3Vector(0, 0, 0), radius = 0.;
	};
	sphere(f3Vector cen, float rad,f3Vector materialCol) {
		radius = rad;
		center = cen;
		MaterialColors = materialCol;
		radius_2 = radius * radius;
	}
	HITPOINT hit(ray r) {
		//according to the formula in textbook,it is easy to check the discriminant b^2-4ac, (b)^2-a((ec·ec)-R^2),where a=d^2,b=d·ec
		f3Vector d = r. dir;
		f3Vector ec = r.orig - center;
		HITPOINT h;
		float a = d * d;
		float b = d * ec;
		float discriminant = b * b - a*((ec * ec) - radius_2);
		if (discriminant > 0) {
			
			float r1 = (-b + sqrt(discriminant)) / a;
			float r2 = (-b - sqrt(discriminant)) / a;
			float solver;
			r1 = MAXfloat(0., r1);//get rid of negative
			r2 = MAXfloat(0., r2);
			solver = MINfloat(r1, r2);
			if (solver >= 0.) {
				// valid solver
				h.surfacetype = SPHERE;
				h.solvers = 1;
				h.t1 = solver;
				h.MaterialColors = this->MaterialColors;
				return h;
			}else {
				return h;
			}
		}
		else {
			return h;
		}
	}
	void GetSphere() {
		cout << "center:(" << center.x << "," << center.y << "," << center.z << "),Radius:" << radius << endl;
	}
	f3Vector getNormVec(f3Vector interpoint) {
		f3Vector N;
		N = (interpoint - center)*(1/radius);
		N.normalize();
		return N;
	}

};