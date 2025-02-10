#pragma once
#include "ofMain.h"
#include "obj_loader.h"
#include"OBJ_Data.h"
#include"scene_classes.h"
#include"matf4x4.h"
#define NORMAL_SHADING 0
#define GAURAUD_SHADING 1
#define PHONG_SHADING 2
#define WIREFRAME 3
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
		f3Vector _TestColor;
		int _ShadingTYPE;//0 for normal shading, 1 for Gauraud shading and 2 for PhongShading 
		ofTrueTypeFont StringInfo;
		string _infoString;
		double _Telapsed;
		int		_FPS;
		int		_ModelSelect;//0 for beacon 1 for teapot 2 for teapot with normals
		int     _Texture;//0 none texture, 1 for jpg1
		string ModelName;
		string _basepath;
		string _objname;
		f3Vector _TeapotCen;
		f3Vector _BeaconCen;
		string _filename;
		bool ModelChangeFlage;
		int _ProjectionTYPE = 0;// 0 for orthongal, 1 for perspective

		//about Model loading
		tinyobj::attrib_t attrib;// for vertices normals textures
		std::vector<tinyobj::shape_t> shapes;//for faces information
		std::vector<tinyobj::material_t> materials; //material information
		void ofApp::loadModel(const char* filename, const char* basepath, bool triangulate);
		OBJResources OBJData;
		bool ConvertStruct();
		void InfoOBJ();// for debugging

		//about coordination setting
		f3Vector _axis_x;/*(1,0,0)set up for right hand coor*/ 
		f3Vector _axis_y;/*(0,1,0)set up for right hand coor*/
		f3Vector _axis_z;/*(0,0,1)set up for right hand coor*/
		f3Vector _Model_e, _Arc_e, _Cam_e,_CamBox_low,_CamBox_high;// the e_origin of these three coordination
		ModelCoor ofModelCoor;
		ArcballCoor ofArcCoor;
		CameraCor ofCamCoor;
		SCENE ofScene;//setting up my scene
		float _LightTensity;
		void setupScene(f3Vector model_cen, f3Vector model_e, f3Vector arc_e, f3Vector cam_e, f3Vector camBox_low, f3Vector camBox_high, f3Vector light = f3Vector(0, 400, 0));



		//for arcball function
		matf4x4   _p_AcrTFmat;// this matrix record the transformation of Arcball operation previous, each time update with _p_AcrTFmat=current*_p_AcrTFmat;
		matf4x4   _c_AcrTFmat;//the transform of arcball currently
		bool ArcballFlag;
		f2Vector _startP;
		f2Vector _finishP;
		matf4x4 ArcballMat(float x, float y, float xp, float yp);


		//about pipeline
		matf4x4 _ofGlobalToModelMat;//This is used for transforming the points in real world to model. Take the input in real world
		matf4x4 _ofModelToGlobalMat;  //from model local to global coor. In fact, I don't need this matrix,set it as identical matrix
		matf4x4 _ofGlobalToCamMat;    //from global to camera coor
		matf4x4 _ofCameraToCanonical_Morth; //from camera to canonical through Orthongonal projection
		matf4x4 _ofCameraToCanonical_Mper; //from camera to canonical through Perspective projection
		matf4x4 _ofCanonicaltoScreen_Mvp;		//viewport transform
		void VerticesShader();
		void FragmentShader();
		void FragmentProcess();
		//for shading
		int _PhongExp;
		f3Vector _ambient;
		f3Vector GouraudShading(f3Vector v, f3Vector normVec3/*have to be normalized*/, f3Vector vcolor);
		f3Vector PhongShading(f3Vector v, f3Vector normVec3/*have to be normalized*/, f3Vector vcolor);
		//for rasterization
		RasResources RasterData;// the vertices data after the vertices shader
		FragRes FragData;// the vertices data after the rasterization shader
		std::vector<Fragment> LineRas(f3Vector v1, f3Vector v2, f3Vector aV/*this is for calculate the depth info*/);

		//for texture mapping
		ofTexture _Mytexture;
		ofFloatPixels _Mypixels;
};
