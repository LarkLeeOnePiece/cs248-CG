#pragma once
#include"ray.h"
#include"vector.h"
#include"globalSetting.h"
#define PLANE 1
class plane {
public:
	float y;
	f3Vector materialcolar;
	plane(){
		
	};
	plane(float position,f3Vector materialCor) {
		y = position;
		materialcolar = materialCor;
	}

	f3Vector getNormVec() {//normal vector
		f3Vector Norm = f3Vector(0, 1, 0);
		return Norm;
	}
	HITPOINT hit(ray r) {//Just need the ray to calculate the point
		double t;
		HITPOINT hitresult; //flag,objection,t
		t = (y - r.orig.y) / r.dir.y;//according the parameter equation to calculate the time
		if (t > 0) {
			hitresult.surfacetype = PLANE;
			hitresult.solvers = 1;
			hitresult.t1 = t;
			hitresult.MaterialColors = this->materialcolar;
		}
		return hitresult;
	}
};