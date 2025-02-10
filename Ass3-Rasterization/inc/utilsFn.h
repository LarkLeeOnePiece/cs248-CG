#include"vector.h"
#include"ofMain.h"
#include"obj_loader.h"

f3Vector ofTof3Vec(ofVec3f of3VEC);
void ofMatPrint(ofMatrix4x4 mat4);
void ofMatInfo(ofMatrix4x4 mat1, ofMatrix4x4 mat2, ofMatrix4x4 mat3, ofMatrix4x4 mat4, ofMatrix4x4 mat5, ofMatrix4x4 mat6);
void PrintInfo(const tinyobj::attrib_t& attrib,
    const std::vector<tinyobj::shape_t>& shapes,
    const std::vector<tinyobj::material_t>& materials);
bool TestLoadObj(const char* filename, const char* basepath=NULL,
    bool triangulate=false);

//for triangle rasterization
int min_x(float v0x,float v1x,float v2x);
int max_x(float v0x, float v1x, float v2x);
int min_y(float v0y, float v1y, float v2y);
int max_y(float v0y, float v1y, float v2y);
//for barycentric
float alphaPara(f3Vector v0, f3Vector v1, f3Vector v2,int x,int y);
float betaPara(f3Vector v0, f3Vector v1, f3Vector v2 ,int x, int y);
float gamaPara(f3Vector v0, f3Vector v1, f3Vector v2, int x, int y);

void ColorSpace(f3Vector& color);


