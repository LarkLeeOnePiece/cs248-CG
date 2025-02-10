#pragma once
#include"vector.h"
#include"globalSetting.h"
#include"utils.h"
#define SPHERE 2
class sphere {
public:
	f3Vector center;
	float Real_radius;//this is for normal vector
	float FIX_radius;//this is fixed for reset
	f3Vector MaterialColors;
	float radius_2;
	f3Vector centerFIX;//the center of ellipsoid,this is fixed, for normalvector
	float SCALE;// set it as square form
	f3Vector Ro_ANGLES;
	ofMatrix4x4 TsTolocal;
	ofMatrix4x4 LocalRoMat;
	ofMatrix4x4 RoToWorld;
	ofMatrix4x4 TsToWorld;
	ofMatrix4x4 PreMatrix;// to local matrix
	ofMatrix4x4 PostMatrix;// to real world matrix
	ofVec3f axis_x;
	ofVec3f axis_y;
	ofVec3f axis_z;
	sphere() {
		center = f3Vector(0, 0, 0), Real_radius = 0.;
	};
	sphere(f3Vector cen, float rad,f3Vector materialCol) {
		SCALE = 1;
		FIX_radius = rad;
		center = cen;
		centerFIX = center;
		MaterialColors = materialCol;
		Real_radius = rad;
		radius_2 = Real_radius * Real_radius;
		TsTolocal = ofMatrix4x4();
		TsTolocal.makeTranslationMatrix(-center.x, -center.y, -center.z);
		LocalRoMat = ofMatrix4x4();
		RoToWorld = ofMatrix4x4();
		TsToWorld = ofMatrix4x4();
		PreMatrix = ofMatrix4x4();
		PostMatrix = ofMatrix4x4();
		axis_x = ofVec3f(1, 0, 0);
		axis_y = ofVec3f(0, 1, 0);
		axis_z = ofVec3f(0, 0, 1);
	}
	ray rayToLocal(ray r) {
		ray newray;
		newray.orig = ofTof3Vec(r.orig.toOfvec3() * TsTolocal);// only origin, dir no need to Ts
		//cout << "Ellipsoid->Test TsTolocal:" << endl;
		//ofMatPrint(TsTolocal);
		newray.dir = r.dir;
		newray.orig = ofTof3Vec(newray.orig.toOfvec3() * LocalRoMat);// Rotate origin
		newray.dir = ofTof3Vec(newray.dir.toOfvec3() * LocalRoMat);// Rotate dir
		newray.dir.normalize();
		return newray;
	}
	HITPOINT hit(ray r) {
		//according to the formula in textbook,it is easy to check the discriminant b^2-4ac, (b)^2-a((ec·ec)-R^2),where a=d^2,b=d·ec
		r = rayToLocal(r);
		f3Vector d = r. dir;
		f3Vector ec = r.orig ;
		HITPOINT h;
		float a = d * d;
		float b = d * ec;
		float discriminant = b * b - a*((ec * ec) - radius_2);
		if (discriminant > 0) {
			
			float r1 = (-b + sqrt(discriminant)) / radius_2;
			float r2 = (-b - sqrt(discriminant)) / radius_2;
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
		cout << "center:(" << center.x << "," << center.y << "," << center.z << "),Radius:" << FIX_radius << endl;
	}
	f3Vector getNormVec(f3Vector interpoint) {
		f3Vector N;
		N = (interpoint - center)*(1/Real_radius);
		N.normalize();
		return N;
	}
	void SetParas(f3Vector newcenter, float ParaS, f3Vector RoSlider) {
		center = newcenter;
		Ro_ANGLES = RoSlider;
		f3Vector RoinLocalVec3(-RoSlider.x, -RoSlider.y, -RoSlider.z);//This a the rotation matrix for the ray in local coor, so the roatation angle is negative 
		f3Vector RobackRealVec3(RoSlider.x, RoSlider.y, RoSlider.z);//the point from local back to real world
		ofMatrix4x4 nTsTolocal;
		nTsTolocal.makeTranslationMatrix(-center.x, -center.y, -center.z);

		ofMatrix4x4 nLocalRoMat;
		nLocalRoMat.makeRotationMatrix(RoinLocalVec3.x, axis_x, RoinLocalVec3.y, axis_y, RoinLocalVec3.z, axis_z);

		//3.Rotation back to real world
		//4.Translate back to real world
		//In fact 1,2 combines a matrix,3 and 4 combines another matrix
		ofMatrix4x4 nRoToWorld;
		nRoToWorld.makeRotationMatrix(RobackRealVec3.x, axis_x, RobackRealVec3.y, axis_y, RobackRealVec3.z, axis_z);

		ofMatrix4x4 nTsToWorld;
		nTsToWorld.makeTranslationMatrix(center.x, center.y, center.z);

		ofMatrix4x4 nPrematrix = nTsTolocal * nLocalRoMat;//Ts first, then Rotation

		ofMatrix4x4 nPostmatrix = nRoToWorld * nTsToWorld;//Rotation first, then Ts

		SCALE = ParaS;
		Real_radius = FIX_radius* ParaS;
		radius_2 = Real_radius * Real_radius;
		this->TsTolocal = nTsTolocal;
		this->LocalRoMat = nLocalRoMat;
		this->RoToWorld = nRoToWorld;
		this->TsToWorld = nTsToWorld;
		this->PreMatrix = nPrematrix;
		this->PostMatrix = nPostmatrix;
	}
};