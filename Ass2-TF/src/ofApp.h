#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include"mThreads.h"
#include "vector.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		//for rendering
		ofTexture 		texColor;
		int w, h;
		ofPixels colorPixels;

		ofxPanel gui;// creat a panel for paramter
		ofParameter<glm::vec4> CenSlider;// for all center
		ofParameter<glm::vec3> RoSlider;// add for ellipsoid center
		//for camera rtransform
		ofParameter<glm::vec3> viewTsSlider;// for all viewpoint translate
		ofParameter<glm::vec3> CamRoSlider;// for camera rotation
		ofParameter<glm::vec3> LightPos;// for camera rotation
		

		//for parameterupdate
		void RotationSlider(glm::vec3& RoSlider);
		void CenterSlider(glm::vec4& CenSlider);
		void CamTsSliderFn(glm::vec3& viewTsSlider);
		void CamRoSliderFn(glm::vec3& CamRoSlider);
		void LightSliderFn(glm::vec3& LightPos);
		ofTrueTypeFont frame;
		string timeString;
		double elapsed;
		int		FPS;
		int		SELECTED;//record the obj selected
		bool	ANIMATED;

		ofVec3f axis_x;
		ofVec3f axis_y;
		ofVec3f axis_z;
		ofVec3f sceneCen;// scene center

		f3Vector Se_pixelVec;//for global object selected
		f3Vector Se_viewPoint;

		ofMatrix4x4 CameraTsMatrix;// to local matrix
		ofMatrix4x4 CameraRoMatrix;// to real world matrix

};
