#pragma once
#include"TBASE.h"
#include"vector.h"
#include"globalSettings.h"
#define CONE 3

class cone {// cone should has a circluar base
public:
	f3Vector  peak;
	f3Vector  MaterialColors;
	f3Vector  V;//vector from peak to base
	f3Vector  cBase;//center of base
	float height;
	float  degree;//I use the function:(x-x0)^2+(z-z0)^2-c^2(h-y+y0)^2=0 to present our surface
	float  costhea;
	float  rBase;//radius for base
	base ConeBase;
	cone() {
	}
	cone(f3Vector pPoint,  float deg,float h,f3Vector color) :peak(pPoint) , degree(deg), height(h), MaterialColors(color){
		costhea = cos(degree*3.14159/180);
		V =  f3Vector(0, 1, 0);//a normalize vector;
		V.normalize();
		cBase = f3Vector(peak.x, peak.y + h, peak.z);
		rBase = height * tan(degree * 3.14159 / 180);
		ConeBase=base(cBase, rBase,V, MaterialColors);
	}
	HITPOINT hit(ray r) {
		//according to the formula in textbook,it is easy to check the discriminant b^2-4ac, (b)^2-a((ec·ec)-R^2),where a=d^2,b=d·ec
		f3Vector d = r.dir;
		f3Vector e = r.orig;
		f3Vector CO = e - peak;
		float costheta2 = costhea * costhea;
		HITPOINT h;
		float a11 = d * V;
		float b11 = (CO * V);
		float a = a11*a11 - costheta2;
		float b = 2 * (a11*b11 - (d * CO) * costheta2);
		float c = b11*b11 - CO * CO * costheta2;
		float discriminant = b * b - 4 * a * c;
		float a_2 = 2 * a;
		if (discriminant > 0) {//hit the surface

			float r1 = (-b + sqrt(discriminant)) / a_2;
			float r2 = (-b - sqrt(discriminant)) / a_2;
			r1 = MAXfloat(0., r1);//get rid of negative
			r2 = MAXfloat(0., r2);
			float solver = MINfloat(r1, r2);
			f3Vector iP = r.InterPoint(solver);
			float testcos = (iP - peak) * V;

			if (solver >= 0.&& testcos>0.&&(iP.y<=peak.y+height)) {
				// valid solver
				h.surfacetype = CONE;
				h.solvers = 1;
				h.t1 = solver;
				h.MaterialColors = this->MaterialColors;
			}
		}
		HITPOINT basehit = ConeBase.hit(r);
		if (basehit.t1 < h.t1&&basehit.t1>0) {
			basehit.surfacetype = CONE;
			return basehit;
		}
		return h;

	}
	void GetCone() {
		cout << "peak:(" << peak.x << "," << peak.y << "," << peak.z << "),degree:" << degree <<",costhea:" << costhea << endl;
	}
	f3Vector getNormVec(f3Vector interpoint) {
		f3Vector N;
		f3Vector CP = interpoint - peak;
		if (abs(interpoint.y - cBase.y)<0.001) {
			//cout << "Get CONE BASE Vec" << endl;
			return V;
		}
		else {
			N = CP - (V * (CP.norm() / costhea));
			N.normalize();
			return N;
		}

	}

};