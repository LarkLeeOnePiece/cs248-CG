#include"utils.h"
f3Vector ofTof3Vec(ofVec3f of3VEC) {
	f3Vector newf3Vec;
	newf3Vec.x = of3VEC.x;
	newf3Vec.y = of3VEC.y;
	newf3Vec.z = of3VEC.z;
	return newf3Vec;
}

void ofMatPrint(ofMatrix4x4 mat4) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << mat4._mat[i][j] << " "; //_mat[column][row] 打印出来的每一行为正常矩阵的列
		}
		cout << endl;
	}
}
void ofMatInfo(ofMatrix4x4 mat1, ofMatrix4x4 mat2, ofMatrix4x4 mat3, ofMatrix4x4 mat4, ofMatrix4x4 mat5, ofMatrix4x4 mat6) {
	cout << "TsTolocal:" << endl;
	ofMatPrint(mat1);
	cout << "LocalRoMat:" << endl;
	ofMatPrint(mat2);
	cout << "RoToWorld:" << endl;
	ofMatPrint(mat3);
	cout << "TsToWorld:" << endl;
	ofMatPrint(mat4);
	cout << "Prematrix:" << endl;
	ofMatPrint(mat5);
	cout << "Postmatrix:" << endl;
	ofMatPrint(mat6);
}