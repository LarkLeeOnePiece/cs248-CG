#include "ofApp.h"
#include "globalSetting.h"
#include "plane.h"
#include"sphere.h"
#include"cone.h"
#include"ellipsoid.h"
#include"TBASE.h"
#include"Shading.h"
#include "mat.h"
#include<iostream>
#include"mThreads.h"
// cpp_compiler_options_openmp.cpp
#include <omp.h>
//OBJECTS Setup
plane Plane = plane(planeY, PlaneColor);
sphere Sphere = sphere(f3Vector(0, 2, -3), 1, SphereColor);
cone Cone = cone(f3Vector(0, 1, -2), 30, 1, ConeColor);
ellipsoid Ellipsoid = ellipsoid(1, 2, 1, -3, 2, -2, EllipsoidColor);
f3Vector ANGS(90, 0, 0);
//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);// To avoid "tearing" effects
	cout << "max thread:" << omp_get_max_threads() << endl;//  max thread 16
	//View Setup
	viewTYPE = 1;//0 for parallel,1 for perspective
	vp = VIEWPLANE(-4.0, 4.0, 2.5, -2.5);

	//WINDOWS Setup
	w = ofGetWidth();
	h = ofGetHeight();
	colorPixels.allocate(w, h, OF_PIXELS_RGB);
	viewPoint = f3Vector(0, 0, ViewPlaneZ+Fn);// Using right-hand coordination = f3Vector(0, 0, 1);// Using right-hand coordination

	//Update setup
	UPDATE = false;
	ANIMATED = false;
	RoSlider.addListener(this, &ofApp::RotationSlider);
	CenSlider.addListener(this, &ofApp::CenterSlider);
	viewTsSlider.addListener(this, &ofApp::CamTsSliderFn);
	CamRoSlider.addListener(this, &ofApp::CamRoSliderFn);
	LightPos.addListener(this, &ofApp::LightSliderFn);
	gui.setup(); // most of the time you don't need a name
	gui.add(CenSlider.set("CENTER", { 0, 0 ,0 ,1}, { vp.left * 4, vp.bottom * 4,ViewPlaneZ-10,0.1 }, { vp.right * 4, vp.top * 4,ViewPlaneZ-3,2}));
	gui.add(RoSlider.set("ROTATION", {0,0 ,0 }, { -360,-360,-360 }, { 360,360,360 }));
	gui.add(viewTsSlider.set("Camera PAN", { viewPoint.x,viewPoint.y ,viewPoint.z }, { vp.left * 4, vp.bottom * 4,ViewPlaneZ-15 }, { vp.right * 4, vp.top * 4,ViewPlaneZ +15}));
	gui.add(CamRoSlider.set("Camera Orbit", { 0,0 ,0 }, { -180,-360,0 }, { 0,360,0 }));
	gui.add(LightPos.set("L2 Pos", { Lights[1].x,Lights[1].y ,Lights[1].z }, {0,0,0}, {10,10,10}));
	frame.load("vag.ttf", 10);

	//threads setting
	colPixels = w;//1 dimension thread
	numThreads = 4;

	//for axis
	axis_x = ofVec3f(1,0,0);
	axis_y = ofVec3f(0, 1, 0);
	axis_z = ofVec3f(0, 0, 1);


	/*
	*/
}
//--------------------------------------------------------------

void ofApp::update() {
	//parameters update here
	//ray tracing here

	if (UPDATE) {

		double start = omp_get_wtime();
		if (ANIMATED) {
			float sinx = sin(start);
			//cout << "sinx" << sinx<< endl;
			f3Vector center(Sphere.center.x + sinx, Sphere.center.y, Sphere.center.z);
			Sphere.SetParas(center, Sphere.SCALE, Sphere.Ro_ANGLES);
		}
		for (ys = 0; ys < h; ys++) {
			//#pragma omp parallel for num_threads(40)
			#pragma omp parallel for num_threads(12)
			for (xs = 0; xs < w; xs++) {
				HITPOINT hp;
				f3Vector pixelVec = f3Vector(vp.left + (vp.right - vp.left) * (xs + 0.5) / w, vp.top - (vp.top - vp.bottom) * (ys + 0.5) / h, ViewPlaneZ);
				if (viewTYPE == 1) {
					viewPoint = f3Vector(0, 0, ViewPlaneZ + Fn);// Using right-hand coordination
				}
				else {
					viewPoint = f3Vector(pixelVec.x, pixelVec.y, ViewPlaneZ + Fn);
				}
				viewPoint = ofTof3Vec(viewPoint.toOfvec3() * CameraTsMatrix);// only origin, dir no need to Ts
				pixelVec= ofTof3Vec(pixelVec.toOfvec3() * CameraTsMatrix);// only origin, dir no need to Ts
				viewPoint = ofTof3Vec(viewPoint.toOfvec3() * CameraRoMatrix);// only origin, dir no need to Ts
				pixelVec = ofTof3Vec(pixelVec.toOfvec3() * CameraRoMatrix);// only origin, dir no need to Ts
				ray r = ray(viewPoint, pixelVec);

				hp = HitCheck(r);

				f3Vector mcolors;//total colors
				mcolors = Shading(hp, r, pixelVec);

				colorPixels.setColor(xs, ys, ofColor((mcolors.x) * 255, (mcolors.y) * 255, (mcolors.z) * 255));
				hp.intialize();
			}

		}
		texColor.allocate(colorPixels);
		double finish = omp_get_wtime();
		//cout << "finish:" << finish << endl;
		elapsed = finish - start;
		FPS = int(1 / elapsed);
		//cout << "Time measured:" << elapsed << "seconds" << endl;;
	}
	else {
		texColor.allocate(colorPixels);
	}
	//UPDATE = false;

}

//--------------------------------------------------------------
void ofApp::draw() {
	// draw new picture here after the ray tracing at update stage, because update and draw are called in an infinite loop one after another in that order, until we finish the application.

	texColor.draw(0, 0, w, h);//draws the texture at a given point (x,y), with a given width (w) and height (h). Draws the texture at the x, y and w, h.
	gui.draw();

	//timeString = "FPS:" + ofToString( ofGetFrameRate()) + "\n elapsed: " + ofToString(elapsed) + " s";
	timeString = "FPS:" + ofToString(FPS) + " \nelapsed: " + ofToString(elapsed) + " s";
	frame.drawString(timeString, w - 150, 20);
}


//seperate ray tracing
HITPOINT HitCheck(ray r) {
	HITPOINT thp;//temp Hitpoint
	HITPOINT fhp;//final Hitpoint
	//for each object, I need to transilate the oject and ray
	//and rotate the ray get hit point, and then rotate the hitpoint back to real world
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
		if (thp.t1 < fhp.t1) {
			fhp = thp;//get the closest point

		}
	}
	return fhp;
}
f3Vector Shading(HITPOINT hp,ray r,f3Vector pixelVec) {
	

	
	f3Vector mcolors;//total colors
	if (hp.surfacetype != 0) {
		
		//multiple lights
		for (int t = 0; t < Lights.size(); t++) {
			f3Vector interPoint;
			f3Vector NormVec3;
			//cout << t << ":" << "(" << Lights[t].x << "," << Lights[t].y << "," << Lights[t].z << "）" << endl;
			f3Vector tLight = Lights[t];
			//cout << t << ":" << "(" << tLight.x << "," << tLight.y << "," << tLight.z << "）" << endl;
			interPoint = r.InterPoint(hp.t1);
			//Get the norm of the surface
			int TYPE = hp.surfacetype;
			switch (TYPE) {
			case PLANE:		NormVec3 = Plane.getNormVec();
				break;
			case SPHERE:	NormVec3 = Sphere.getNormVec(interPoint); break;
			case CONE:	
				NormVec3 = Cone.getNormVec(interPoint);
				break;
			case ELLIPSOID:	NormVec3 = Ellipsoid.getNormVec(interPoint); break;
			}

			f3Vector viewVec = viewPoint - pixelVec;
			viewVec.normalize();
			f3Vector PhongcolorFactors;


			f3Vector MrayDir = r.dir - NormVec3 * 2 * (r.dir * NormVec3);
			MrayDir.normalize();
			f3Vector mirrorColor;

			f3Vector AmColor;
			AmColor = AmbientShading(hp.MaterialColors);


			PhongcolorFactors = PhongLightingModel(tLight, interPoint, NormVec3, hp.MaterialColors, viewVec, hp);
			if (TYPE==PLANE) mirrorColor = mirrorReflection(interPoint, MrayDir);
			mcolors = mcolors + PhongcolorFactors + mirrorColor + AmColor;
			ColorSpace(mcolors);
		}
		//colorPixels.setColor(x, y, ofColor((mcolors.x) * 255, (mcolors.y) * 255, (mcolors.z) * 255));

	}
	else {
		mcolors = f3Vector(0, 0, 0);
		//colorPixels.setColor(x, y, ofColor(0, 0, 0));
	}
	return mcolors;
}
void ofApp::CamTsSliderFn(glm::vec3& viewTsSlider) {
	f3Vector center(viewTsSlider.x, viewTsSlider.y, viewTsSlider.z);
	Se_viewPoint.z = center.z;//global object select
	ofMatrix4x4 CamTs;
	CamTs.makeTranslationMatrix(center.x, center.y, center.z);
	CameraTsMatrix = CamTs;
	//ofMatPrint(CameraTsMatrix);
	UPDATE = true;
}
void ofApp::LightSliderFn(glm::vec3& LightPos) {
	f3Vector center(LightPos.x, LightPos.y, LightPos.z);
	Lights[1] = center;//update the pos of the second light
}
void ofApp::CamRoSliderFn(glm::vec3& CamRoSlider) {
	f3Vector center(CamRoSlider.x, CamRoSlider.y, CamRoSlider.z);
	ofMatrix4x4 CamTo;
	CamTo.makeRotationMatrix(center.x, axis_x, center.y, axis_y, center.z, axis_z);
	CameraTsMatrix = CamTo;
	//ofMatPrint(CameraTsMatrix);
	UPDATE = true;
}
//--------------------------------------------------------------
//Some functions for parameters update
void ofApp::CenterSlider(glm::vec4& CenSlider) {
	
	f3Vector center(CenSlider.x, CenSlider.y, CenSlider.z);
	float SCALE = CenSlider.w;
	UPDATE = true;
	switch (SELECTED) {
	case PLANE:
		break;
	case SPHERE:
		Sphere.SetParas(center, SCALE, Sphere.Ro_ANGLES);
		break;
	case CONE:
		Cone.SetParas(center, SCALE, Cone.Ro_ANGLES);
		break;
	case ELLIPSOID:
		Ellipsoid.SetParas(center, SCALE, Ellipsoid.Ro_ANGLES);
		break;
	default:
		break; //without any choice, no n
	}
	UPDATE = true;
}
void ofApp::RotationSlider(glm::vec3& RoSlider) {
	f3Vector SliderAngles(RoSlider.x, RoSlider.y, RoSlider.z);
	switch (SELECTED) {
	case PLANE:	
			break;
	case SPHERE:
			Sphere.SetParas(Sphere.center, Sphere.SCALE, Sphere.Ro_ANGLES);
			break;
	case CONE:{
			Cone.SetParas(Cone.center, Cone.SCALE, SliderAngles);
			break;
			}
	case ELLIPSOID:
			Ellipsoid.SetParas(Ellipsoid.center, Ellipsoid.SCALE, SliderAngles);
			break;
	default:
		{
		}
	}
	UPDATE = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	//cout << "key:" << key << endl;//space==32, 's'=115  'r'==114
	if (key == 114) {
		//key='r',reset the transform
		switch (SELECTED) {
		case SPHERE:
			Sphere.SetParas(Sphere.centerFIX,1, f3Vector(0,0,0));
			CenSlider.set({ Sphere.center.x ,Sphere.center.y ,Sphere.center.z ,Sphere.SCALE });
			RoSlider.set({ Sphere.Ro_ANGLES.x ,Sphere.Ro_ANGLES.y ,Sphere.Ro_ANGLES.z });
			break;
		case CONE:
			Cone.SetParas(Cone.centerFIX, 1, f3Vector(0, 0, 0));
			CenSlider.set({ Cone.center.x ,Cone.center.y ,Cone.center.z ,Cone.SCALE });
			RoSlider.set({ Cone.Ro_ANGLES.x ,Cone.Ro_ANGLES.y ,Cone.Ro_ANGLES.z });
			break;
		case ELLIPSOID:
			Ellipsoid.SetParas(Ellipsoid.centerFIX, 1, f3Vector(0, 0, 0));
			CenSlider.set({ Ellipsoid.center.x ,Ellipsoid.center.y ,Ellipsoid.center.z ,Ellipsoid.SCALE });
			RoSlider.set({ Ellipsoid.Ro_ANGLES.x ,Ellipsoid.Ro_ANGLES.y ,Ellipsoid.Ro_ANGLES.z });
			break;
		default://If not selected object, reset all
			Sphere.SetParas(Sphere.centerFIX, 1, f3Vector(0, 0, 0));
			Cone.SetParas(Cone.centerFIX, 1, f3Vector(0, 0, 0));
			Ellipsoid.SetParas(Ellipsoid.centerFIX, 1, f3Vector(0, 0, 0));
			CenSlider.set({ 0,0,0,1});
			RoSlider.set({ 0,0,0});
			break; //without any choice, no n
		}
	
	}
	if (key == 32) {
		//key is 'space',animated starts;!
		ANIMATED = !ANIMATED;//reverse the state
	}
	if (key == 115) {
		f3Vector TviewPoint = f3Vector(0, 0, ViewPlaneZ + Fn);
		viewTsSlider.set({ TviewPoint.x,TviewPoint.y ,TviewPoint.z });
		CamRoSlider.set({ 0,0 ,0 });
		//key is s, reset the camera transform

	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
		// if the mouse is pressed over the circle an event will be notified (broadcasted) 
		// the circleEvent object will contain the mouse position, so this values are accesible to any class that is listening.
		glm::vec2 mousePos = glm::vec2(x,y);
		//cout << "CLICK:(" << x << "," << y << ")" << endl;
		Se_pixelVec = f3Vector(vp.left + (vp.right - vp.left) * (x + 0.5) / w, vp.top - (vp.top - vp.bottom) * (y + 0.5) / h, ViewPlaneZ);
		if (viewTYPE == 1) {
			Se_viewPoint = f3Vector(0, 0, ViewPlaneZ + Fn);// Using right-hand coordination
		}
		else {
			Se_viewPoint = f3Vector(Se_pixelVec.x, Se_pixelVec.y, ViewPlaneZ + Fn);
		}
		Se_viewPoint = ofTof3Vec(Se_viewPoint.toOfvec3() * CameraTsMatrix);// only origin, dir no need to Ts
		Se_pixelVec = ofTof3Vec(Se_pixelVec.toOfvec3() * CameraTsMatrix);// only origin, dir no need to Ts
		Se_viewPoint = ofTof3Vec(Se_viewPoint.toOfvec3() * CameraRoMatrix);// only origin, dir no need to Ts
		Se_pixelVec = ofTof3Vec(Se_pixelVec.toOfvec3() * CameraRoMatrix);// only origin, dir no need to Ts
		ray r = ray(Se_viewPoint, Se_pixelVec);
		HITPOINT select_hp = HitCheck(r);
		SELECTED = select_hp.surfacetype;
		//cout << "Selected OBJ:" << select_hp.surfacetype << endl;
		switch (SELECTED) {
		case PLANE:
			break;
		case SPHERE: {
			CenSlider.set({ Sphere.center.x ,Sphere.center.y ,Sphere.center.z ,Sphere.SCALE });
			RoSlider.set({ Sphere.Ro_ANGLES.x ,Sphere.Ro_ANGLES.y ,Sphere.Ro_ANGLES.z });
				break;
		}
		case CONE:
			CenSlider.set({ Cone.center.x ,Cone.center.y ,Cone.center.z ,Cone.SCALE });
			RoSlider.set({ Cone.Ro_ANGLES.x ,Cone.Ro_ANGLES.y ,Cone.Ro_ANGLES.z });
			break;
		case ELLIPSOID:
			CenSlider.set({ Ellipsoid.center.x ,Ellipsoid.center.y ,Ellipsoid.center.z ,Ellipsoid.SCALE });
			RoSlider.set({ Ellipsoid.Ro_ANGLES.x ,Ellipsoid.Ro_ANGLES.y ,Ellipsoid.Ro_ANGLES.z });
			break;
		default:
			break; //without any choice, no n
		}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
