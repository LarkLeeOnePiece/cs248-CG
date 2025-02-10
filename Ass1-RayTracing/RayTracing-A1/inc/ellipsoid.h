#pragma once
#include"ray.h"
#include"globalSettings.h"
#define ELLIPSOID 4
class ellipsoid {
public:
	float a2; float b2; float c2; float x0; float y0; float z0;
	f3Vector MaterialColors;
	ellipsoid() {

	}
	ellipsoid(float A, float B, float C, float xc, float yc, float zc,f3Vector MC) {
		a2 = A*A; b2 = B*B; c2 = C*C; x0 = xc; y0 = yc; z0 = zc; MaterialColors = MC;
	}
	HITPOINT hit(ray r) {
		//according to the formula in textbook,it is easy to check the discriminant b^2-4ac, (b)^2-a((ec·ec)-R^2),where a=d^2,b=d·ec
		f3Vector d = r.dir;
		f3Vector e = r.orig;
		HITPOINT h;
		float ex = e.x - x0;
		float ey = e.y - y0;
		float ez = e.z - z0;
		float a = (d.x * d.x / a2 + d.y * d.y / b2 + d.z * d.z / c2);
		float b = 2 * (ex * d.x / a2 + ey * d.y / b2 + ez * d.z / c2);
		float c = ex * ex / a2 + ey * ey / b2 + ez * ez / c2 - 1;
		float discriminant = b * b - 4*a*c;
		float a_2 = 2 * a;
		if (discriminant > 0) {

			float r1 = (-b + sqrt(discriminant)) / a_2;
			float r2 = (-b - sqrt(discriminant)) / a_2;
			r1 = MAXfloat(0., r1);//get rid of negative
			r2 = MAXfloat(0., r2);
			float solver = MINfloat(r1, r2);
			if (solver >= 0.) {
				// valid solver
				h.surfacetype = ELLIPSOID;
				h.solvers = 1;
				h.t1 = solver;
				h.MaterialColors = this->MaterialColors;
				return h;
			}
			else {
				return h;
			}
		}
		else {
			return h;
		}
	}
	void getEllipsoid() {
		cout << "ELLIPSOID:" << sqrt(a2) << " " << sqrt(b2) << " " << sqrt(c2) << " " << x0 << " " << y0 << " " << z0 << endl;
	}
	f3Vector getNormVec(f3Vector interpoint) {
		f3Vector N;
		N.x = (interpoint.x - x0) / a2;
		N.y = (interpoint.y - y0) / b2;
		N.z = (interpoint.z - z0) / c2;
		N.normalize();
		return N;
	}
};