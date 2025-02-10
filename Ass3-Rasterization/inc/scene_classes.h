/*
Author: Da Li
Here I set up some global seeting for the scene
*/
#include"vector.h"
#include"matf4x4.h"
class ModelCoor {
public:
	//This is for model coordination setting
	f3Vector origin;
	f3Vector u_basis;
	f3Vector v_basis;
	f3Vector w_basis;
	ModelCoor() {}
	void setup(f3Vector e, f3Vector u_b, f3Vector v_b, f3Vector w_b);
	
};
class ArcballCoor {
public:
	//This is for Arcball control coordination setting
	f3Vector origin;
	f3Vector u_basis;
	f3Vector v_basis;
	f3Vector w_basis;
	ArcballCoor(){}
	void setup(f3Vector e, f3Vector u_b, f3Vector v_b, f3Vector w_b);
};
class CameraCor {
public:
	//This is for camera coordination setting
	f3Vector origin;
	f3Vector u_basis;
	f3Vector v_basis;
	f3Vector w_basis;
	f3Vector CameraBox_Low;// The setting for L,B N, left bottom, near plane
	f3Vector CameraBox_High;// The setting for R,T F, right, top, far plane
	CameraCor(){}
	void setup(f3Vector e, f3Vector u_b, f3Vector v_b, f3Vector w_b,f3Vector camBox_low,f3Vector camBox_high);
};
class SCENE {
public:
	//In this scene, I only consider right-hand coordination
	f3Vector ModelCenter; //This is the thing given, for difference model, it has differenct center
	f3Vector Lights;
	ModelCoor Model_Coor;
	ArcballCoor Arc_Coor;
	CameraCor Cameral_Coor;
	void setup(f3Vector model_cen, ModelCoor model_coor, ArcballCoor arc_coor, CameraCor cam_coor, f3Vector light=f3Vector(0,400,0));
};