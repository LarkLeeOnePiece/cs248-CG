#pragma once
#include"vector.h"
#include"globalSettings.h"
#define BASE 5

class base {// cone should has a circluar base
public:
	f3Vector  MaterialColors;
	f3Vector  V;//vector from peak to base
	f3Vector  cBase;//center of base
	float  rBase;//radius for base
	base() {
	}
	base(f3Vector bPoint, float radius, f3Vector Vec, f3Vector color) :cBase(bPoint), MaterialColors(color), rBase(radius),V(Vec){
	}
	HITPOINT hit(ray r) {//Just need the ray to calculate the point
		double t;
		HITPOINT hitresult; //flag,objection,t
		t = (cBase.y - r.orig.y) / r.dir.y;//according the parameter equation to calculate the time
		f3Vector p = r.orig + r.dir * t;
		float length = sqrt((p.x - cBase.x) * (p.x - cBase.x) + (p.z - cBase.z) * (p.z - cBase.z));
		if (t > 0&&length<=rBase&&abs(p.y-cBase.y)<0.01) {
			hitresult.surfacetype = BASE;
			hitresult.solvers = 1;
			hitresult.t1 = t;
			hitresult.MaterialColors = this->MaterialColors;
		}
		return hitresult;
	}

	f3Vector getNormVec(f3Vector interpoint) {
		return V;
	}

};