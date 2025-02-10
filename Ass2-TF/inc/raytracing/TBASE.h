#pragma once
#include"vector.h"
#include"globalSetting.h"
#include"utils.h"
#define BASE 5

class base {// cone should has a circluar base
public:
	f3Vector  MaterialColors;
	f3Vector  V;//vector from peak to base, it means the normal vector of the base
	f3Vector  cBase;//center of base
	f3Vector  ocBase;//center at origin since this is a plane, it should be always at(0,0,0) !!! not(0,0,0) center is the center of cone
	float  rBase;//radius for base
	ofMatrix4x4 TsTolocal;
	ofMatrix4x4 LocalRoMat;
	ofMatrix4x4 RoToWorld;
	ofMatrix4x4 TsToWorld;
	ofMatrix4x4 PreMatrix;// to local matrix
	ofMatrix4x4 PostMatrix;// to real world matrix
	base() {
	}
	base(f3Vector bPoint, float radius, f3Vector Vec, f3Vector color) :cBase(bPoint), MaterialColors(color), rBase(radius),V(Vec){
		TsTolocal = ofMatrix4x4();
		LocalRoMat = ofMatrix4x4();
		RoToWorld = ofMatrix4x4();
		TsToWorld = ofMatrix4x4();
		PreMatrix = ofMatrix4x4();
		PostMatrix = ofMatrix4x4();
		ocBase = ofTof3Vec(cBase.toOfvec3() * TsTolocal);//transform peak to orginal
	}
	ray rayToLocal(ray r) {
		ray newray;
		newray.orig = ofTof3Vec(r.orig.toOfvec3() * TsTolocal);// only origin, dir no need to Ts
		newray.dir = r.dir;
		newray.orig = ofTof3Vec(newray.orig.toOfvec3() * LocalRoMat);// Rotate origin
		newray.dir = ofTof3Vec(newray.dir.toOfvec3() * LocalRoMat);// Rotate dir
		newray.dir.normalize();
		return newray;
	}
	HITPOINT hit(ray r) {//Just need the ray to calculate the point
		HITPOINT hitresult; //flag,objection,t
		//r = rayToLocal(r);
		f3Vector o = r.orig;
		f3Vector d = r.dir;
		float t = ((ocBase - o) * V )/ (d*V);
		f3Vector p = r.orig + r.dir * t;//point in base
		f3Vector v_inBase = p - ocBase;
		//First  v_inBase dot product V should be zero -> coplane
		float coplane = v_inBase * V;
		float pcx = (p.x - ocBase.x) * (p.x - ocBase.x);
		float pcy = (p.y - ocBase.y) * (p.y - ocBase.y);
		float pcz = (p.z - ocBase.z) * (p.z - ocBase.z);
		float length = sqrt(pcx +pcy +pcz );
		//cout << "CONEBASE:" << endl;
		//ocBase.info();
		if (t > 0&&length<=rBase&&abs(coplane)<=0.001) {
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
	void SetParas(f3Vector newcenter, float Coneradius,ofMatrix4x4 nTsTolocal, ofMatrix4x4 nLocalRoMat, ofMatrix4x4 nRoToWorld, ofMatrix4x4 nTsToWorld, ofMatrix4x4 nPreMatrix, ofMatrix4x4 nPostMatrix){
		cBase = newcenter;
		rBase = Coneradius;
		this->TsTolocal = nTsTolocal;
		this->LocalRoMat = nLocalRoMat;
		this->RoToWorld = nRoToWorld;
		this->TsToWorld = nTsToWorld;
		ocBase = ofTof3Vec(cBase.toOfvec3() * TsTolocal);//transform peak to orginal
	}
};
/*
	void SetParas(f3Vector newcenter, ofMatrix4x4 nLocalRoMat, ofMatrix4x4 nRoToWorld) {
		cBase = newcenter;
		ofMatrix4x4 nTsTolocal;
		nTsTolocal.makeTranslationMatrix(-cBase.x, -cBase.y, -cBase.z);

		ofMatrix4x4 nTsToWorld;
		nTsToWorld.makeTranslationMatrix(cBase.x, cBase.y, cBase.z);

		ofMatrix4x4 Prematrix = nTsTolocal * nLocalRoMat;//Ts first, then Rotation

		ofMatrix4x4 Postmatrix = nRoToWorld * nTsToWorld;//Rotation first, then Ts
		this->TsTolocal = nTsTolocal;
		this->LocalRoMat = nLocalRoMat;
		this->RoToWorld = nRoToWorld;
		this->TsToWorld = nTsToWorld;
		ocBase = ofTof3Vec(cBase.toOfvec3() * TsTolocal);//transform peak to orginal
	}

*/

/*
	HITPOINT hit(ray r) {//Just need the ray to calculate the point
		HITPOINT hitresult; //flag,objection,t
		f3Vector o = r.orig;
		f3Vector d = r.dir;
		float t = ((cBase - o) * V )/ (d*V);
		f3Vector p = r.orig + r.dir * t;//point in base
		f3Vector v_inBase = p - cBase;
		//First  v_inBase dot product V should be zero -> coplane
		float coplane = v_inBase * V;
		float pcx = (p.x - cBase.x) * (p.x - cBase.x);
		float pcy = (p.y - cBase.y) * (p.y - cBase.y);
		float pcz = (p.z - cBase.z) * (p.z - cBase.z);
		float length = sqrt(pcx +pcy +pcz );
		if (t > 0&&length<=rBase&&abs(coplane)<=0.001) {
			hitresult.surfacetype = BASE;
			hitresult.solvers = 1;
			hitresult.t1 = t;
			hitresult.MaterialColors = this->MaterialColors;
		}
		return hitresult;
	}
*/