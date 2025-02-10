#pragma once
#include"ray.h"
#include"globalSetting.h"
#define ELLIPSOID 4
class ellipsoid {
public:
	float a2; float b2; float c2; float x0; float y0; float z0;
	f3Vector MaterialColors;
	//parameters for transformation
	f3Vector center;//the center of ellipsoid,this is fixed.
	f3Vector DisCenter;// for the distribution of the ellipsoid, just(a,b,c) this is fixed
	float SCALE;// set it as square form
	
	ofMatrix4x4 TsTolocal;
	ofMatrix4x4 LocalRoMat;
	ofMatrix4x4 RoToWorld;
	ofMatrix4x4 TsToWorld;
	ofMatrix4x4 PreMatrix;// to local matrix
	ofMatrix4x4 PostMatrix;// to real world matrix
	ofVec3f axis_x;
	ofVec3f axis_y;
	ofVec3f axis_z;
	f3Vector centerFIX;
	f3Vector Ro_ANGLES;
	ellipsoid() {

	}
	ellipsoid(float A, float B, float C, float xc, float yc, float zc,f3Vector MC) {
		a2 = A*A; b2 = B*B; c2 = C*C; x0 = xc; y0 = yc; z0 = zc; MaterialColors = MC;
		SCALE = 1;
		center = f3Vector(x0, y0, z0);
		centerFIX = center;
		DisCenter =f3Vector(a2, b2, c2);
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
		f3Vector d = r.dir;
		f3Vector e = r.orig;
		HITPOINT h;
		float ex = e.x ;//at the local coor,it should be (0,0,0)
		float ey = e.y;
		float ez = e.z;
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
		x0 = center.x;
		y0 = center.y;
		z0 = center.z;
		SCALE = ParaS;
		a2 = DisCenter.x * SCALE; b2 = DisCenter.y * SCALE; c2 = DisCenter.z * SCALE;
		this->TsTolocal = nTsTolocal;
		this->LocalRoMat = nLocalRoMat;
		this->RoToWorld = nRoToWorld;
		this->TsToWorld = nTsToWorld;
		this->PreMatrix = nPrematrix;
		this->PostMatrix = nPostmatrix;
	}
};

/*
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

*/