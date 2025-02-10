#include "matf4x4.h"
#define SET_ROW(row, v1, v2, v3, v4 )    \
_mat[(row)][0] = (v1); \
_mat[(row)][1] = (v2); \
_mat[(row)][2] = (v3); \
_mat[(row)][3] = (v4);

//I can use inner product to calculate dot product of rth row of A with cth colomn of B
#define INNER_PRODUCT(a,b,r,c) \
((a)._mat[r][0] * (b)._mat[0][c]) \
+((a)._mat[r][1] * (b)._mat[1][c]) \
+((a)._mat[r][2] * (b)._mat[2][c]) \
+((a)._mat[r][3] * (b)._mat[3][c])
matf4x4::matf4x4() {
	_row = 4;
	_col = 4;
	SET_ROW(0, 1, 0, 0, 0)
	SET_ROW(1, 0, 1, 0, 0)
	SET_ROW(2, 0, 0, 1, 0)
	SET_ROW(3, 0, 0, 0, 1)
}
matf4x4::matf4x4(float matdata[4][4]) {
	_row = 4;
	_col = 4;
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _col; j++) {
			_mat[i][j] = matdata[i][j];
		}
	}
}
void matf4x4::poMult(const matf4x4& other) {
	// more efficient method just use a float[4] for temporary storage.
	float t[4];
	for (int row = 0; row < 4; ++row)
	{
		t[0] = INNER_PRODUCT(*this, other, row, 0);//class one , class two, row, col
		t[1] = INNER_PRODUCT(*this, other, row, 1);
		t[2] = INNER_PRODUCT(*this, other, row, 2);
		t[3] = INNER_PRODUCT(*this, other, row, 3);
		SET_ROW(row, t[0], t[1], t[2], t[3])
	}
}
void matf4x4::makeFromMultiplicationOf(const matf4x4& lhs, const matf4x4& rhs)
{
	if (&lhs == this)
	{
		poMult(rhs);//In my opinion, this would never run?
		return;
	}
	// PRECONDITION: We assume neither &lhs nor &rhs == this
	// if it did, use preMult or postMult instead
	_mat[0][0] = INNER_PRODUCT(lhs, rhs, 0, 0);
	_mat[0][1] = INNER_PRODUCT(lhs, rhs, 0, 1);
	_mat[0][2] = INNER_PRODUCT(lhs, rhs, 0, 2);
	_mat[0][3] = INNER_PRODUCT(lhs, rhs, 0, 3);
	_mat[1][0] = INNER_PRODUCT(lhs, rhs, 1, 0);
	_mat[1][1] = INNER_PRODUCT(lhs, rhs, 1, 1);
	_mat[1][2] = INNER_PRODUCT(lhs, rhs, 1, 2);
	_mat[1][3] = INNER_PRODUCT(lhs, rhs, 1, 3);
	_mat[2][0] = INNER_PRODUCT(lhs, rhs, 2, 0);
	_mat[2][1] = INNER_PRODUCT(lhs, rhs, 2, 1);
	_mat[2][2] = INNER_PRODUCT(lhs, rhs, 2, 2);
	_mat[2][3] = INNER_PRODUCT(lhs, rhs, 2, 3);
	_mat[3][0] = INNER_PRODUCT(lhs, rhs, 3, 0);
	_mat[3][1] = INNER_PRODUCT(lhs, rhs, 3, 1);
	_mat[3][2] = INNER_PRODUCT(lhs, rhs, 3, 2);
	_mat[3][3] = INNER_PRODUCT(lhs, rhs, 3, 3);
}
/// \brief Write data with `matrix(row,col)=number`
float& matf4x4::operator()(std::size_t row, std::size_t col) {
	return _mat[row][col];
}
template <class T>
inline T SGL_ABS(T a)
{
	return (a >= 0 ? a : -a);
}

#ifndef SGL_SWAP
#define SGL_SWAP(a,b,temp) ((temp)=(a),(a)=(b),(b)=(temp))
#endif

bool matf4x4::invert_4x4(const matf4x4& src, matf4x4& dst)
{
	if (&src == &dst) {
		matf4x4 tm(src);
		return invert_4x4(tm, dst);
	}

	unsigned int indxc[4], indxr[4], ipiv[4];
	unsigned int i, j, k, l, ll;
	unsigned int icol = 0;
	unsigned int irow = 0;
	double temp, pivinv, dum, big;

	// copy in place this may be unnecessary
	dst = src;

	for (j = 0; j < 4; j++) ipiv[j] = 0;

	for (i = 0; i < 4; i++)
	{
		big = 0.0;
		for (j = 0; j < 4; j++)
			if (ipiv[j] != 1)
				for (k = 0; k < 4; k++)
				{
					if (ipiv[k] == 0)
					{
						if (SGL_ABS(dst(j, k)) >= big)
						{
							big = SGL_ABS(dst(j, k));
							irow = j;
							icol = k;
						}
					}
					else if (ipiv[k] > 1)
						return false;
				}
		++(ipiv[icol]);
		if (irow != icol)
			for (l = 0; l < 4; l++) SGL_SWAP(dst(irow, l),
				dst(icol, l),
				temp);

		indxr[i] = irow;
		indxc[i] = icol;
		if (dst(icol, icol) == 0)
			return false;

		pivinv = 1.0 / dst(icol, icol);
		dst(icol, icol) = 1;
		for (l = 0; l < 4; l++) dst(icol, l) *= pivinv;
		for (ll = 0; ll < 4; ll++)
			if (ll != icol)
			{
				dum = dst(ll, icol);
				dst(ll, icol) = 0;
				for (l = 0; l < 4; l++)dst(ll, l) -= dst(icol, l) * dum;
			}
	}
	for (int lx = 4; lx > 0; --lx)
	{
		if (indxr[lx - 1] != indxc[lx - 1])
			for (k = 0; k < 4; k++) SGL_SWAP(dst(k, indxr[lx - 1]),
				dst(k, indxc[lx - 1]), temp);
	}

	return true;
}
bool matf4x4::makeInvertOf(const matf4x4& rhs) {
	return invert_4x4(rhs, *this);
}
void matf4x4::makeRotateMat(float anglex, float angley, float anglez) {
	float radx = anglex * 3.1415926 / 180;
	float rady = angley * 3.1415926 / 180;
	float radz = anglez * 3.1415926 / 180;
	float CX = cos(radx), SX = sin(radx);
	float CY = cos(rady), SY = sin(rady);
	float CZ = cos(radz), SZ = sin(radz);
	SET_ROW(0, CY*CZ,SX*SY*CZ-CX*SZ, CX*SY*CZ+SX*SZ, 0);
	SET_ROW(1, CY*SZ, SX*SY*SZ+CX*CZ, CX*SY*SZ - SX*CZ, 0);
	SET_ROW(2,-SY, SX*CY,CX*CY, 0);
	SET_ROW(3, 0,0,0,1);
}
void matf4x4::makeTsMat(float x, float y, float z) {
		SET_ROW(0, 1, 0, 0, x)
		SET_ROW(1, 0, 1, 0, y)
		SET_ROW(2, 0, 0, 1, z)
		SET_ROW(3, 0, 0, 0, 1)
}
void matf4x4::makeScaleMat(float Sx, float Sy, float Sz) {
		SET_ROW(0, Sx, 0, 0, 0)
		SET_ROW(1, 0, Sy, 0, 0)
		SET_ROW(2, 0, 0, Sz, 0)
		SET_ROW(3, 0, 0, 0, 1)
}
/*
input: the canonical coordination of frame coordination basis and its origin
make this* matrix becomes a matrix that can transform P_xyz to P_uvw
If I input the coordination of camera, then I can get the transformation from real world to camera
*/
void matf4x4::makeTFtoLocalmat(f3Vector u_basis, f3Vector v_basis, f3Vector w_basis, f3Vector e_origin) {
		SET_ROW(0, u_basis.x, u_basis.y, u_basis.z, -(u_basis* e_origin))
		SET_ROW(1, v_basis.x, v_basis.y, v_basis.z, -(v_basis * e_origin))
		SET_ROW(2, w_basis.x, w_basis.y, w_basis.z, -(w_basis * e_origin))
		SET_ROW(3, 0, 0, 0, 1)
}
/*
input: the canonical coordination of frame coordination basis and its origin
make this* matrix becomes a matrix that can transform P_uvw to P_xyz
*/
void matf4x4::makeTFtoWorldmat(f3Vector u_basis, f3Vector v_basis, f3Vector w_basis, f3Vector e_origin) {
	SET_ROW(0, u_basis.x, v_basis.x, w_basis.x, e_origin.x)
	SET_ROW(1, u_basis.y, v_basis.y, w_basis.y, e_origin.y)
	SET_ROW(2, u_basis.z, v_basis.z, w_basis.z, e_origin.z)
	SET_ROW(3, 0, 0, 0, 1)
}
void matf4x4::makeOrthProjectionmat(f3Vector CamBox_low, f3Vector CamBox_high) {
	float r_l = CamBox_high.x - CamBox_low.x;// r-l
	float r_p_l= CamBox_high.x + CamBox_low.x;//r+l;
	float t_b= CamBox_high.y - CamBox_low.y;// t-b
	float t_p_b = CamBox_high.y + CamBox_low.y;// t+b
	float n_f= CamBox_low.z - CamBox_high.z;// Be care, near plane is bigger than far plane numerically
	float n_p_f= CamBox_low.z + CamBox_high.z;
	SET_ROW(0, 2 / r_l, 0, 0, -(r_p_l / r_l))
	SET_ROW(1, 0, 2 / t_b, 0, -(t_p_b / t_b))
	SET_ROW(2, 0, 0, 2 / n_f, -(n_p_f / n_f))
	SET_ROW(3, 0, 0, 0, 1)
}
void matf4x4::makePerProjectionmat(f3Vector CamBox_low, f3Vector CamBox_high) {
	float r_l = CamBox_high.x - CamBox_low.x;// r-l
	float r_p_l = CamBox_high.x + CamBox_low.x;//r+l;
	float t_b = CamBox_high.y - CamBox_low.y;// t-b
	float t_p_b = CamBox_high.y + CamBox_low.y;// t+b
	float n_f = CamBox_low.z - CamBox_high.z;// n-f,Be care, near plane is bigger than far plane numerically
	float n_p_f = CamBox_low.z + CamBox_high.z;//n+f
	SET_ROW(0, 2* CamBox_low.z/(r_l),0,r_p_l/(-r_l),0)
	SET_ROW(1, 0, 2 * CamBox_low.z/t_b, t_p_b/(-t_b),0)
	SET_ROW(2, 0, 0, n_p_f/n_f,2* CamBox_low.z* CamBox_high.z/(-n_f))
	SET_ROW(3, 0, 0, 1, 0)
}
/*
According to the width and height of the window to make the viewport transform
*/
void matf4x4::makeViewPortmat(int nx, int ny) {
	SET_ROW(0, nx/2, 0, 0, (nx-1)/2)
	SET_ROW(1, 0, -ny/2, 0, (ny-1)/2)
	SET_ROW(2, 0, 0, 1, 0)
	SET_ROW(3, 0, 0, 0, 1)
}
void matf4x4::makeArcballmat(float theta, f3Vector u) {
	float u_xx = u.x * u.x;
	float u_xy = u.x * u.y;
	float u_xz = u.x * u.z;
	float u_yy = u.y * u.y;
	float u_yz = u.y * u.z;
	float u_zz = u.z * u.z;
	SET_ROW(0, u_xx+(1-u_xx)*cos(theta),u_xy*(1-cos(theta))-u.z*sin(theta),u_xz*(1-cos(theta))+u.y*sin(theta),0)
	SET_ROW(1, u_xy*(1-cos(theta))+u.z*sin(theta),u_yy+(1-u_yy)*cos(theta),u_yz*(1-cos(theta))-u.x*sin(theta),0)
	SET_ROW(2, u_xz*(1-cos(theta))-u.y*sin(theta),u_yz*(1-cos(theta))+u.x*sin(theta),u_zz+(1-u_zz)*cos(theta),0)
	SET_ROW(3, 0, 0, 0, 1)
}

void matf4x4::matprint(string title) {
	{
		cout << title << endl;
		for (int i = 0; i < _row; ++i) {
			//#pragma omp parallel for num_threads(40)
			for (int j = 0; j < _col; ++j) {
				std::cout << _mat[i][j] << " ";
			}
			std::cout << std::endl;
		}
		cout << endl;
	}
}