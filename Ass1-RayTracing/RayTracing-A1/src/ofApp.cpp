#pragma once
// cpp_compiler_options_openmp.cpp
#include <omp.h>
#include "ofApp.h"
#include"globalSettings.h"
#include "plane.h"
#include"sphere.h"
#include"cone.h"
#include"ellipsoid.h"
#include"TBASE.h"
#include"Shading.h"

#include<time.h>

plane Plane = plane(planeY, PlaneColor);
//sphere Sphere = sphere(f3Vector(0, 0, 0), 0.5, SphereColor);
sphere Sphere = sphere(f3Vector(0, 2, 3), 1, SphereColor);
cone Cone = cone(f3Vector(0, -1.5, 2), 30, 1, ConeColor);
ellipsoid Ellipsoid = ellipsoid(1, 1, 4, -3, 2, 2, EllipsoidColor);
base Base = base(f3Vector(0, -1, 1), 1, f3Vector(0,1,0),f3Vector(0.5, 0.7, 0.7));
//--------------------------------------------------------------
void ofApp::setup() {
	//set up parameters
	w = ofGetWidth();
	h = ofGetHeight();
	colorPixels.allocate(w, h, OF_PIXELS_RGB);
	int viewTYPE = 1;//0 for parallel,1 for perspective
	cout << "max thread:" << omp_get_max_threads() << endl;//  max thread 128
	float left = -2.0, right = 2.0, top = 1.5, bottom = -1.5;
	// color pixels, use x and y to control red and green
	double start = omp_get_wtime();
	//clock_t start = clock();
	//#pragma omp parallel for num_threads(40)
	for (int y = 0; y < h; y++) {
		#pragma omp parallel for num_threads(40)
		for (int x = 0; x < w; x++) {
			HITPOINT hp;
			f3Vector pixelVec;
			f3Vector interPoint;
			f3Vector NormVec3;
			pixelVec = f3Vector(left + (right - left) * (x + 0.5) / w, top - (top - bottom) * (y + 0.5) / h, ViewPlaneZ);
			if (viewTYPE == 1) {
				viewPoint = f3Vector(0, 0, -1);
			}
			else {
				viewPoint = f3Vector(pixelVec.x, pixelVec.y, -1);
			}
			ray r = ray(viewPoint, pixelVec);
			//#pragma omp parallel for num_threads(5)
			for (int o = 1; o <= OBJECTNUM; o++) {
				HITPOINT thp;
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
				//#pragma omp critical
				{
					if (thp.t1 < hp.t1) {
						hp = thp;//get the closest point

					}
				}

			}
			
			bool Test = false;
				//start the calculation of interpoint
				if (hp.surfacetype != 0) {
					f3Vector mcolors;//total colors
					//multiple lights
					f3Vector PhongcolorFactors;
					f3Vector mirrorColor;
					f3Vector AmColor;
					for (int t = 0; t < Lights.size(); t++) {
						//cout << t << ":" << "(" << Lights[t].x << "," << Lights[t].y << "," << Lights[t].z << "£©" << endl;
						f3Vector tLight = Lights[t];
						//cout << t << ":" << "(" << tLight.x << "," << tLight.y << "," << tLight.z << "£©" << endl;
						interPoint = r.InterPoint(hp.t1);
						//Get the norm of the surface
						int TYPE = hp.surfacetype;
						switch (TYPE) {
						case PLANE:		NormVec3 = Plane.getNormVec();
							break;
						case SPHERE:	NormVec3 = Sphere.getNormVec(interPoint); break;
						case CONE:	NormVec3 = Cone.getNormVec(interPoint);  break;
						case ELLIPSOID:	NormVec3 = Ellipsoid.getNormVec(interPoint); break;
						}

						f3Vector viewVec = viewPoint - pixelVec;
						viewVec.normalize();
						f3Vector MrayDir = r.dir - NormVec3 * 2 * (r.dir * NormVec3);
						MrayDir.normalize();				
						PhongcolorFactors = PhongLightingModel(tLight, interPoint, NormVec3, hp.MaterialColors, viewVec, hp);
						if(TYPE== PLANE) mirrorColor = mirrorReflection(interPoint, MrayDir); //Plane mirror reflection
						PhongcolorFactors = PhongcolorFactors + PhongcolorFactors;
						mirrorColor = mirrorColor + mirrorColor;
					}
					AmColor = AmbientShading(hp.MaterialColors);
					mcolors = mcolors + PhongcolorFactors + mirrorColor + AmColor;
					ColorSpace(mcolors);
					colorPixels.setColor(x, y, ofColor((mcolors.x) * 255, (mcolors.y) * 255, (mcolors.z) * 255));

				}
				else {
					colorPixels.setColor(x, y, ofColor(0, 0, 0));
				}
				hp.intialize();

		}
	}
	//clock_t end = clock();
	double finish = omp_get_wtime();
	double elapsed = finish - start;
	//double elapsed = double(end - start) / CLOCKS_PER_SEC;
	printf("Time measured: %.3f seconds.\n", elapsed);
	texColor.allocate(colorPixels);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
	//ofSetHexColor(0xffffff);
	texColor.draw(0, 0, w, h);//draws the texture at a given point (x,y), with a given width (w) and height (h). Draws the texture at the x, y and w, h.
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
