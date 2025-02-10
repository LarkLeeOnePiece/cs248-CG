#pragma once
#include<iostream>
#include"ray.h"
#include"plane.h"
#include"vector.h"
#include<cmath>
#include"globalSettings.h"
int Shadow(f3Vector lightSource, f3Vector interPoint, HITPOINT thp);
f3Vector LambertianIllumination(f3Vector lightSource, f3Vector interPoint, f3Vector normVec3,f3Vector materialColor) {
	ray sourceLight(interPoint, lightSource);//from the surface to the object
	//float cos = COS(sourceLight.dir, normVec3);   !!!!!
	f3Vector colVec3;
	double MAX0 = MAXfloat(0, sourceLight.dir * normVec3);
	//check the hit surface type,it makes difference for different materials
	colVec3.x = materialColor.x * cL.x * MAX0;
	colVec3.y= materialColor.y * cL.y * MAX0;
	colVec3.z = materialColor.z * cL.z * MAX0;
	return colVec3;
}
f3Vector PhongLightingModel(f3Vector lightSource, f3Vector interPoint, f3Vector normVec3, f3Vector materialColor, f3Vector viewVec3, HITPOINT thp) {
	int ShadowFlag = Shadow(lightSource, interPoint,thp);//0 not in shadow, 1 in shadow
	if (ShadowFlag) {
		return f3Vector(0, 0, 0);//in shadow, all is black
	}
	ray sourceLight(interPoint, lightSource);//from the surface to the object
	f3Vector ref = normVec3 * (2 * (sourceLight.dir * normVec3))- sourceLight.dir;
	f3Vector colVec3;
	double MAX0 = MAXfloat(0, viewVec3*ref);
	MAX0 = (float)pow(MAX0, PhongExp);
	//check the hit surface type,it makes difference for different materials
	colVec3.x = cL.x * MAX0;
	colVec3.y = cL.y * MAX0;
	colVec3.z = cL.z * MAX0;
	f3Vector LambertiancolorFactors = LambertianIllumination(LightSource, interPoint, normVec3, materialColor);
	colVec3 = colVec3 + LambertiancolorFactors;
	return colVec3;
}
extern plane Plane;
extern sphere Sphere;
extern cone Cone;
extern ellipsoid Ellipsoid;
int checkHIT(int TYPE,ray shadowray) {
	//return 0 not in shadow, 1 in shadow  
	HITPOINT thp;
	for (int o = 1; o <= OBJECTNUM; o++) {
		
		if (o == PLANE&&TYPE) {
			thp = Plane.hit(shadowray);
		}
		if (o == SPHERE && TYPE ) {
			thp = Sphere.hit(shadowray);
		}
		if (o == CONE && TYPE ) {
			thp = Cone.hit(shadowray);
		}
		if (o == ELLIPSOID && TYPE ) {
			thp = Ellipsoid.hit(shadowray);
		}
		if (thp.surfacetype != 0&&thp.t1>Sigma) {//detect once, end this loop

			return 1;
		}
	}
	return 0;
}
f3Vector AmbientShading(f3Vector MaterialColors,float IA=Ia) {
	f3Vector color;
	color = MaterialColors * IA;
	return color;
}
int Shadow(f3Vector lightSource, f3Vector interPoint,HITPOINT thp) {
	int IfShadow=0; //0 not in shadow; 1 in shadow
	ray shadowray(interPoint, lightSource);
	
	int TYPE = thp.surfacetype;
	IfShadow=checkHIT(TYPE, shadowray);
	return IfShadow;
}
f3Vector MRColor (f3Vector oriCOlor,f3Vector MaterialColors, float IA = Ia) {
	f3Vector color;
	color.x = MaterialColors.x * IA;
	color.y = MaterialColors.y * IA;
	color.z = MaterialColors.z * IA;
	return color;
}
f3Vector mirrorReflection(f3Vector rayOrigin,f3Vector dir) {
	ray r;
	r.orig = rayOrigin; r.dir = dir;
	f3Vector mRcolor;//second mirror colors
	HITPOINT thp;
	HITPOINT mhp;//hp used for mirror
	f3Vector mNormVec3;
	//#pragma omp parallel for num_threads(6)
	for (int o = 1; o <= OBJECTNUM; o++) {
		if (o == PLANE) {
			thp = Plane.hit(r);
		}
		if (o == SPHERE) {
			thp = Sphere.hit(r);
		}
		if (o == CONE) {
			thp = Cone.hit(r);
		}
		if (o == ELLIPSOID) {
			thp = Ellipsoid.hit(r);
		}
		if (thp.t1 < mhp.t1&&(thp.t1> Sigma)) {
			mhp = thp;//get the closest point
		}
	}
	//start the calculation of interpoint
	if (mhp.surfacetype != 0) {
		//multiple lights
		for (int t = 0; t < Lights.size(); t++) {
			//cout << t << ":" << "(" << Lights[t].x << "," << Lights[t].y << "," << Lights[t].z << "£©" << endl;
			f3Vector tLight = Lights[t];
			//cout << t << ":" << "(" << tLight.x << "," << tLight.y << "," << tLight.z << "£©" << endl;
			f3Vector TinterPoint = r.InterPoint(mhp.t1);
			//Get the norm of the surface
			int TYPE = mhp.surfacetype;
			switch (TYPE) {
			case PLANE:		mNormVec3 = Plane.getNormVec(); //cout << "CONE MIRROR" << endl;
				break;
			case SPHERE:	mNormVec3 = Sphere.getNormVec(TinterPoint); break;
			case CONE:	mNormVec3 = Cone.getNormVec(TinterPoint); break;
			case ELLIPSOID:	mNormVec3 = Ellipsoid.getNormVec(TinterPoint); break;
			}

			f3Vector viewVec = dir*-1;
			viewVec.normalize();
			f3Vector PhongcolorFactors = PhongLightingModel(tLight, TinterPoint, mNormVec3, mhp.MaterialColors, viewVec, mhp);
			f3Vector AmColor = AmbientShading(mhp.MaterialColors);
			mRcolor = (mRcolor + PhongcolorFactors+ AmColor);
		}
		return mRcolor*0.2;

	}
	else {
		return mRcolor;
	}
	mhp.intialize();
}