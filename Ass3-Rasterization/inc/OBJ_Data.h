#include"obj_loader.h"
#include<vector>
#include"vector.h"
struct FACES {// to store faces data
	int num_face_vertices;//the num of vertices of this face
	std::vector<Int3Vector> indices;//each vector contains x(vertex_index),y(normal_index),z(texcoord_index)
};
struct MESH {// to store face data for each obj
	int num_faces;// the num of faces
	std::vector<FACES> faces;//each vector contains x(vertex_index),y(normal_index),z(texcoord_index)
};
typedef struct OBJRes {// to store all the GL resources for one obj data
	std::vector<f3Vector> vectices;//each vectors represent one vertice
	std::vector<f3Vector> model_vectices;//vertices in model coordination
	std::vector<f3Vector> normals;//each vectors represent one normals
	std::vector<f3Vector> normals_vertices;
	std::vector<f2Vector> textures;//each vectors represent one normals  only 2 dimension
	std::vector<MESH> shapes;//each shape contains different meshes. In this situation, I have only one shape for each obj data
	f3Vector OBJ_center;
} OBJResources;
class RasResources {// to store all the GL resources for one obj data
public:
	std::vector<f3Vector> vectices;//each vectors represent one vertice,in global
	std::vector<f3Vector> cam_vectices;//vertices in camera coordination
	std::vector<f3Vector> normals;//each vectors represent one normals for each faces
	std::vector<f3Vector> normals_vertices;
	std::vector<int> normals_vertices_count;// some faces have the same vertices, I need to calculate the average number
	std::vector<f3Vector> colors;// for vertices
	std::vector<int> colors_count;
	std::vector<f2Vector> textures;//each vectors represent one normals  only 2 dimension
	std::vector<MESH> shapes;//each shape contains different meshes. In this situation, I have only one shape for each obj data
	f3Vector OBJ_center;
	RasResources() {

	}
	RasResources(int verticeSize) {
		this->normals_vertices = std::vector<f3Vector >(verticeSize,f3Vector(0,0,0));//Initilize the vertices normals length
		this->colors=std::vector<f3Vector >(verticeSize, f3Vector(0, 0, 0));
		this->normals_vertices_count= std::vector<int >(verticeSize,0);
		this->colors_count= std::vector<int >(verticeSize, 0);
		this->textures= std::vector<f2Vector >(verticeSize, f2Vector(0,0));
	}
} ;
typedef struct FragmentSruct {//each fragment contains the 2D position and color
	Int2Vector pixel;//location of each fragment
	float depth;
	int type;//used for wireframe 0 for edge 1 for surface
	f3Vector colors;//each vectors represent one normals
}Fragment;
#define Z_MIN  -100000  // In my implement, take z in canonical
class FragRes {// to store all the GL resources for one obj data
public:
	std::vector<std::vector<float>> z_Buffer;// 2D array to record the z-distance for each pixel, INITIALIZE as max!!!
	std::vector < Fragment> fragments;
	FragRes() {

	}
	FragRes(int w,int h) {
		this->z_Buffer = std::vector<std::vector<float>>(w, std::vector<float>(h, Z_MIN));
	}
	void info(string title = " ");
};
