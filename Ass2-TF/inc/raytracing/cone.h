#pragma once
#include"TBASE.h"
#include"vector.h"
#include"globalSetting.h"
#include"mat.h"
#include"ray.h"
#include"utils.h"
#define CONE 3

class cone {// cone should has a circluar base
public:
	f3Vector  peak;//real
	f3Vector  MaterialColors;//real
	f3Vector  V;//vector from peak to base//local
	f3Vector  cBase;//center of base//real
	f3Vector  center;//real
	float height;//real
	float  degree;
	float  costhea;
	float  rBase;//radius for base//real
	base ConeBase;
	float SCALE;
	float FirstH;//for scale
	f3Vector Opeak;
	f3Vector roV;////real
	ofMatrix4x4 TsTolocal;
	ofMatrix4x4 LocalRoMat;
	ofMatrix4x4 RoToWorld;
	ofMatrix4x4 TsToWorld;
	ofMatrix4x4 PreMatrix;// to local matrix
	ofMatrix4x4 PostMatrix;// to real world matrix
	ofVec3f axis_x;
	ofVec3f axis_y;
	ofVec3f axis_z;
	//for reset
	f3Vector Ro_ANGLES;
	f3Vector centerFIX;
	cone() {
	}
	cone(f3Vector pPoint,  float deg,float h,f3Vector color,f3Vector V_dir=f3Vector(0,-1,0)) :peak(pPoint) , degree(deg), height(h), MaterialColors(color){
		costhea = cos(degree*3.14159/180);
		V = V_dir;//a normalize vector;
		V.normalize();
		cBase = peak+V* height;
		rBase = height * tan(degree * 3.14159 / 180);
		center = (cBase + peak) * 0.5;// center between cBase and peak
		centerFIX = center;
		ConeBase=base(cBase, rBase,V, MaterialColors);
		FirstH = height;
		SCALE = 1;
		TsTolocal= ofMatrix4x4();
		LocalRoMat = ofMatrix4x4();
		RoToWorld= ofMatrix4x4();
		TsToWorld = ofMatrix4x4();
		PreMatrix = ofMatrix4x4();
		PostMatrix = ofMatrix4x4();
		axis_x = ofVec3f(1, 0, 0);
		axis_y = ofVec3f(0, 1, 0);
		axis_z = ofVec3f(0, 0, 1);


		Opeak = ofTof3Vec(peak.toOfvec3() * TsTolocal);//transform peak to orginal
	}
	ray rayToLocal(ray r) {
		ray newray;
		newray.orig = ofTof3Vec(r.orig.toOfvec3()*TsTolocal);// only origin, dir no need to Ts
		newray.dir = r.dir;
		newray.orig = ofTof3Vec(newray.orig.toOfvec3()*LocalRoMat );// Rotate origin
		newray.dir = ofTof3Vec(newray.dir.toOfvec3()*LocalRoMat );// Rotate dir
		newray.dir.normalize();
		return newray;
	}

	HITPOINT hit(ray r) {
		r = rayToLocal(r);
		Opeak= ofTof3Vec(peak.toOfvec3() * TsTolocal);//transform peak to orginal,should be update once when parameter update
		f3Vector d = r.dir;
		f3Vector e = r.orig;
		f3Vector CO = e - Opeak;
		float costheta2 = costhea * costhea;
		HITPOINT h;
		float a11 = d * V;
		float b11 = (CO * V);
		float a = a11 * a11 - costheta2;
		float b = 2 * (a11 * b11 - (d * CO) * costheta2);
		float c = b11 * b11 - CO * CO * costheta2;
		float discriminant = b * b - 4 * a * c;
		float a_2 = 2 * a;
		if (discriminant > 0) {//hit the surface

			float r1 = (-b + sqrt(discriminant)) / a_2;
			float r2 = (-b - sqrt(discriminant)) / a_2;
			r1 = MAXfloat(0., r1);//get rid of negative
			r2 = MAXfloat(0., r2);
			float solver = MINfloat(r1, r2);
			f3Vector iP = r.InterPoint(solver);
			float testcos = (iP - Opeak) * V;
			float H_Limit = (iP - Opeak) * V / V.norm();
			if (solver >= 0.&& testcos>0.&&(H_Limit <=height)) {
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
			h= basehit;
		}
		return h;

	}

	void GetCone() {
		cout << "peak:(" << peak.x << "," << peak.y << "," << peak.z << "),degree:" << degree <<",costhea:" << costhea << endl;
	}
	f3Vector getNormVec(f3Vector interpoint) {
		f3Vector N;
		f3Vector CP = interpoint - peak;
		f3Vector v_inBase = interpoint - cBase;
		//First  v_inBase dot product V should be zero -> coplane
		float coplane = v_inBase * roV;
		
		if (abs(coplane) <= 0.001) {
			//cout << "Get CONE BASE Vec" << endl;
			return roV;
		}
		else {
			N = CP - (roV * (CP.norm() / costhea));
			N.normalize();
			return N;
		}

	}
	void SetParas(f3Vector newcenter, float ParaS,f3Vector RoSlider) {
		center = newcenter;
		Ro_ANGLES = RoSlider;
		f3Vector RoinLocalVec3(-RoSlider.x, -RoSlider.y, -RoSlider.z);//This a the rotation matrix for the ray in local coor, so the roatation angle is negative 
		f3Vector RobackRealVec3(RoSlider.x, RoSlider.y, RoSlider.z);//the point from local back to real world

		ofMatrix4x4 nTsTolocal;
		nTsTolocal.makeTranslationMatrix(-center.x,-center.y, -center.z);

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
		height = SCALE * FirstH;
		//cBase = peak + V * height;
		//center = (cBase + peak) * 0.5;// center between cBase and peak.update center
		peak = center - V * height*0.5;
		cBase = peak + V * height;
		rBase = height * tan(degree * 3.14159 / 180);
		roV = ofTof3Vec(V.toOfvec3() * RoToWorld);//Since I need to use V to calculate the Normal Vector, I need to rotation the vector
		this->TsTolocal = nTsTolocal;
		this->LocalRoMat = nLocalRoMat;
		this->RoToWorld = nRoToWorld;
		this->TsToWorld = nTsToWorld;
		this->PreMatrix = nPrematrix;
		this->PostMatrix = nPostmatrix;
		ConeBase.SetParas(cBase, rBase, nTsTolocal,nLocalRoMat, nRoToWorld, nTsToWorld, nPrematrix, nPostmatrix);
	}
};

/*
	HITPOINT hit(ray r) {
	//r = rayToLocal(r);
	//cout << "Initial Ray:" << endl;
	//r.GetRay();
	r.orig = ofTof3Vec(r.orig.toOfvec3() * TsTolocal);// only origin, dir no need to Ts
	r.orig = ofTof3Vec(r.orig.toOfvec3() * LocalRoMat);// Rotate origin
	r.dir = ofTof3Vec(r.dir.toOfvec3() * LocalRoMat);// Rotate dir
	r.dir.normalize();
	//cout << "CONE->Test TsTolocal:" << endl;
	//ofMatPrint(TsTolocal);
	//cout << "CONE->Test LocalRoMat:" << endl;
	//ofMatPrint(LocalRoMat);
	//cout << "Convert Ray:" << endl;
	//r.GetRay();
	f3Vector d = r.dir;
	f3Vector e = r.orig;
	f3Vector CO = e - peak;
	float costheta2 = costhea * costhea;
	HITPOINT h;
	float a11 = d * V;
	float b11 = (CO * V);
	float a = a11 * a11 - costheta2;
	float b = 2 * (a11 * b11 - (d * CO) * costheta2);
	float c = b11 * b11 - CO * CO * costheta2;
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
		float H_Limit = (iP - peak) * V / V.norm();
		if (solver >= 0.&& testcos>0.&&(H_Limit <=height)) {
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
		h= basehit;
	}
	return h;

}

*/
/*
	f3Vector getNormVec(f3Vector interpoint) {
		f3Vector N;
		f3Vector CP = interpoint - peak;
		f3Vector v_inBase = interpoint - cBase;
		//First  v_inBase dot product V should be zero -> coplane
		float coplane = v_inBase * V;
		f3Vector roV = ofTof3Vec(V.toOfvec3() * RoToWorld);
		if (abs(coplane) <= 0.001) {
			//cout << "Get CONE BASE Vec" << endl;
			return V;
		}
		else {
			N = CP - (V * (CP.norm() / costhea));
			N.normalize();
			return N;
		}

	}

*/