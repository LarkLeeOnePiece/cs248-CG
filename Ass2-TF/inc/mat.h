#pragma once
#include<iostream>
#include<vector>
#include"vector.h"
class matf {
public:
	std::vector<std::vector<float>> data;
	int row = 2;
	int col = 2;
	matf(){}
	matf(int m,int n): row(m),col(n){
		data.resize(m, std::vector<float>(n, 0.0));
	}
	matf(int m, int n, std::vector<std::vector<float>> matData) : row(m), col(n) {
		data.resize(m, std::vector<float>(n, 0.0));
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				data[i][j] = matData[i][j];
			}
		}
	}
	//operator for mat
	matf operator + (matf  other);
	matf operator - (matf  other);
	matf operator * (matf  other);
	//operator for vec
	f3Vector operator * (f3Vector  other);
	// ´òÓ¡¾ØÕó
	void print();
};
// translate obj and ray to origin
matf WorldToLocalOrin(f3Vector ObjCen);
//inverse translate obj and ray to origin,back to real world
matf LocalOrinToWorld(f3Vector ObjCen);

//build the rotate matrix according to x,y,z angle
matf Ro_In_Local(f3Vector angles);

//build the rotate matrix according to x,y,z angle from local to real world
matf Ro_To_Real(f3Vector angles);