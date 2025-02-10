#include "mat.h"
#include<iostream>
#include<cmath>
// cpp_compiler_options_openmp.cpp
#include <omp.h>
void matf:: print(){
	{
		for (int i = 0; i < row; ++i) {
//#pragma omp parallel for num_threads(40)
			for (int j = 0; j < col; ++j) {
				std::cout << data[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
}
matf matf::operator+(matf other) {
	if (row != other.row || col != other.col) {
		throw std::invalid_argument("Matrix dimensions do not match for addition.");
	}

	matf result(row, col);
	for (int i = 0; i < row; ++i) {
#pragma omp parallel for num_threads(40)
		for (int j = 0; j < col; ++j) {
			result.data[i][j] = data[i][j] + other.data[i][j];
		}
	}

	return result;
}
matf matf::operator-(matf other) {
	if (row != other.row || col != other.col) {
		throw std::invalid_argument("Matrix dimensions do not match for subtraction.");
	}

	matf result(row, col);
	for (int i = 0; i < row; ++i) {
#pragma omp parallel for num_threads(40)
		for (int j = 0; j < col; ++j) {
			result.data[i][j] = data[i][j] - other.data[i][j];
		}
	}

	return result;
}
matf matf::operator*(matf other) {
	if (col != other.row) {
		throw std::invalid_argument("Matrix dimensions do not allow multiplication.");
	}

	int resultRows = row;
	int resultCols = other.col;
	matf result(resultRows, resultCols);
	
	for (int i = 0; i < resultRows; ++i) {
		#pragma omp parallel for num_threads(40)
		for (int j = 0; j < resultCols; ++j) {
			double sum = 0.0;
			#pragma omp parallel for num_threads(40)
			for (int k = 0; k < col; ++k) {
				sum += data[i][k] * other.data[k][j];
			}
			result.data[i][j] = sum;
		}
	}

	return result;
}
f3Vector matf::operator*(f3Vector coor) {
	// translate coor to mat
	std::vector<std::vector<float>> B = {
		{coor.x}, {coor.y}, {coor.z},{1.0}
			};
	matf other(4, 1, B);
	if (col != other.row) {
		throw std::invalid_argument("Matrix dimensions do not allow multiplication.");
	}

	int resultRows = row;
	int resultCols = other.col;
	matf result(resultRows, resultCols);

	for (int i = 0; i < resultRows; ++i) {
		#pragma omp parallel for num_threads(40)
		for (int j = 0; j < resultCols; ++j) {
			double sum = 0.0;
			for (int k = 0; k < col; ++k) {
				sum += data[i][k] * other.data[k][j];
			}
			result.data[i][j] = sum;
		}
	}
	f3Vector reVec(result.data[0][0],result.data[1][0], result.data[2][0]);
	return reVec;
}
// translate obj  and ray in wrold to local origin
matf WorldToLocalOrin(f3Vector ObjCen) {// according to the center of obj, build the translate mat
	matf T_mat(4, 4);
	/*
	1 0 0 -vx
	0 1 0 -vy
	0 0 1 -vz
	0 0 0 1
	*/
	T_mat.data[0][0] = 1; T_mat.data[1][1] = 1; T_mat.data[2][2] = 1; T_mat.data[3][3] = 1;//diagonal eles
	T_mat.data[0][3] = -1 * ObjCen.x; T_mat.data[1][3] = -1 * ObjCen.y; T_mat.data[2][3] = -1 * ObjCen.z; T_mat.data[3][3] = 1;//diagonal eles
	return T_mat;
}
matf LocalOrinToWorld(f3Vector ObjCen) {// according to the center of obj, build the translate mat
	matf T_mat(4, 4);
	/*
	1 0 0 vx
	0 1 0 vy
	0 0 1 vz
	0 0 0 1
	*/
	T_mat.data[0][0] = 1; T_mat.data[1][1] = 1; T_mat.data[2][2] = 1; T_mat.data[3][3] = 1;//diagonal eles
	T_mat.data[0][3] = ObjCen.x; T_mat.data[1][3] = ObjCen.y; T_mat.data[2][3] = ObjCen.z; T_mat.data[3][3] = 1;//diagonal eles
	return T_mat;
}

//build the rotate matrix according to x,y,z angle
matf Ro_In_Local(f3Vector angs) {
	float radx = angs.x * 3.14159 / 180;
	float rady = angs.y * 3.14159 / 180;
	float radz = angs.z * 3.14159 / 180;
	matf T_matX(4, 4); matf T_matY(4, 4); matf T_matZ(4, 4);
	/*
	T_matX
	1    0    0    0
	0  cosa -sina  0
	0  sina  cosa  0
	0    0    0    1
	*/
	T_matX.data[0][0] = 1.0; T_matX.data[1][1] = cos(radx); T_matX.data[1][2] = -sin(radx);
	T_matX.data[2][1] = sin(radx); T_matX.data[2][2] = cos(radx); T_matX.data[3][3] = 1.0;
	/*
	T_matY
	cosa  0    sina   0
	0     1     0     0
	-sina 0    cosa   0
	0     0     0     1
	*/
	T_matY.data[0][0] = cos(rady); T_matY.data[0][2] = sin(rady); T_matY.data[1][1] = 1.0;
	T_matY.data[2][0] = -sin(rady); T_matY.data[2][2] = cos(rady); T_matY.data[3][3] = 1.0;
	/*
	T_matZ
	cosa -sina    0    0
	sina  cosa    0    0
	0      0      1    0
	0      0      0    1
	*/
	T_matZ.data[0][0] = cos(radz); T_matZ.data[0][1] = -sin(radz); T_matZ.data[1][0] = sin(radz);
	T_matZ.data[1][1] = cos(radz); T_matZ.data[2][2] =1.0; T_matZ.data[3][3] = 1.0;

	matf RoMat = T_matZ * T_matY * T_matX;// from x to y to z
	return RoMat;
}
matf Ro_To_Real(f3Vector angs) {
	float radx = angs.x * 3.14159 / 180*-1;
	float rady = angs.y * 3.14159 / 180*-1;
	float radz = angs.z * 3.14159 / 180*-1;
	matf T_matX(4, 4); matf T_matY(4, 4); matf T_matZ(4, 4);
	/*
	T_matX
	1    0    0    0
	0  cosa -sina  0
	0  sina  cosa  0
	0    0    0    1
	*/
	T_matX.data[0][0] = 1.0; T_matX.data[1][1] = cos(radx); T_matX.data[1][2] = -sin(radx);
	T_matX.data[2][1] = sin(radx); T_matX.data[2][2] = cos(radx); T_matX.data[3][3] = 1.0;
	/*
	T_matY
	cosa  0    sina   0
	0     1     0     0
	-sina 0    cosa   0
	0     0     0     1
	*/
	T_matY.data[0][0] = cos(rady); T_matY.data[0][2] = sin(rady); T_matY.data[1][1] = 1.0;
	T_matY.data[2][0] = -sin(rady); T_matY.data[2][2] = cos(rady); T_matY.data[3][3] = 1.0;
	/*
	T_matZ
	cosa -sina    0    0
	sina  cosa    0    0
	0      0      1    0
	0      0      0    1
	*/
	T_matZ.data[0][0] = cos(radz); T_matZ.data[0][1] = -sin(radz); T_matZ.data[1][0] = sin(radz);
	T_matZ.data[1][1] = cos(radz); T_matZ.data[2][2] = 1.0; T_matZ.data[3][3] = 1.0;

	matf RoMat = T_matX * T_matY* T_matZ;// from z to y to x
	return RoMat;
}