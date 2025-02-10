#include"scene_classes.h"
#include"vector.h"
void ModelCoor::setup(f3Vector e, f3Vector u_b, f3Vector v_b, f3Vector w_b) {
	this->origin = e;
	this->u_basis = u_b;
	this->v_basis = v_b;
	this->w_basis = w_b;
}




void ArcballCoor::setup(f3Vector e, f3Vector u_b, f3Vector v_b, f3Vector w_b) {
	this->origin = e;
	this->u_basis = u_b;
	this->v_basis = v_b;
	this->w_basis = w_b;
}


void CameraCor::setup(f3Vector e, f3Vector u_b, f3Vector v_b, f3Vector w_b, f3Vector camBox_low, f3Vector camBox_high) {
	this->origin = e;
	this->u_basis = u_b;
	this->v_basis = v_b;
	this->w_basis = w_b;
	this->CameraBox_Low = camBox_low;
	this->CameraBox_High = camBox_high;
}

void SCENE::setup(f3Vector model_cen, ModelCoor model_coor, ArcballCoor arc_coor, CameraCor cam_coor, f3Vector light) {
	this->ModelCenter = model_cen;
	this->Model_Coor = model_coor;
	this->Arc_Coor = arc_coor;
	this->Cameral_Coor = cam_coor;
	this->Lights = light;
}