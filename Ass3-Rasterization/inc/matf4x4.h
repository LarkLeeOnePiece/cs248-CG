#pragma once
#include<iostream>
#include<vector>
#include"vector.h"
class matf4x4 {
public:
	float _mat[4][4];
	int _row ;
	int _col ;
	matf4x4();
	matf4x4(float data[4][4]);

	//operator for mat
	void makeFromMultiplicationOf(const matf4x4&, const matf4x4&);
	void matf4x4::poMult(const matf4x4& other);//post multiplication *this become (*this)*other
	void makeRotateMat(float anglex, float angley,  float anglez);//I only consider right-hand coordination, and the rotate order is from x to y to z
	void makeTsMat(float x, float y, float z);//make the translate matrix
	void makeScaleMat(float Sx, float Sy, float Sz);//make the Scale matrix
	void makeTFtoWorldmat(f3Vector u_basis,f3Vector v_basis, f3Vector w_basis, f3Vector e_origin);// build the transform matrix, from a coordination system to another 
	void makeTFtoLocalmat(f3Vector u_basis, f3Vector v_basis, f3Vector w_basis, f3Vector e_origin);// build the transform matrix, from a coordination system to another ,this is from world to local
	void makeOrthProjectionmat(f3Vector CamBox_low, f3Vector CamBox_high);
	void makePerProjectionmat(f3Vector CamBox_low, f3Vector CamBox_high);
	void makeViewPortmat(int nx, int ny);
	void makeArcballmat(float theta, f3Vector u);
	/// \brief creates a new matrix from the product of two matrices.
	/// \brief creates a new matrix from the product of two matrices.
	inline matf4x4 operator * (const matf4x4& m) const;
	inline f3Vector operator * (const f3Vector& m) const;
	float& operator()(std::size_t row, std::size_t col);
	/// \brief Matrix becomes the inverse of the provided matrix.
	/// \brief Makes a new matrix which is the inverse of the given matrix.
	bool makeInvertOf(const matf4x4& rhs);

	//operator for vec
	inline f3Vector postMult(const f3Vector& v) const;/// This operation implicitly treats vectors as col-matrices.    M*p
	// 
	bool invert_4x4(const matf4x4& rhs, matf4x4& dst);
	void matprint(string title=" ");

};
inline matf4x4 matf4x4::operator * (const matf4x4& m) const {//Be carefull, the inline member function of a class should be declaration and defined in the same file
	matf4x4 r;
	r.makeFromMultiplicationOf(*this, m);
	return  r;
}

inline f3Vector matf4x4::postMult(const f3Vector& v) const {   
	float d = 1.0f / (_mat[3][0] * v.x + _mat[3][1] * v.y + _mat[3][2] * v.z + _mat[3][3]);
	return f3Vector((_mat[0][0] * v.x + _mat[0][1] * v.y + _mat[0][2] * v.z + _mat[0][3]) * d,
		(_mat[1][0] * v.x + _mat[1][1] * v.y + _mat[1][2] * v.z + _mat[1][3]) * d,
		(_mat[2][0] * v.x + _mat[2][1] * v.y + _mat[2][2] * v.z + _mat[2][3]) * d);
}
inline f3Vector matf4x4:: operator * (const f3Vector& v) const {
	return postMult(v);
}



/*
* mat test code:
	matf4x4 mat;
	mat.matprint();
	float data1[4][4] = {
		2, 1, -1, 3,
		0, -1, 2, -4,
		-2, 3, 1, 0,
		1, 2, 0, -1 };
	matf4x4 RoMat;
	RoMat.makeRotateMat(30, 45, 60);
	RoMat.matprint();
	matf4x4 InRoMat;
	InRoMat.makeInvertOf(RoMat);
	InRoMat.matprint();

*/