/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "Matrix3.h"
#include "Matrix2.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Quaternion.h"

Matrix3::Matrix3(void)	{
	for (int i = 0; i < 9; ++i) {
		values[i] = 0.0f;
	}
	values[0] = 1.0f;
	values[4] = 1.0f;
	values[8] = 1.0f;
}

Matrix3::Matrix3(float elements[9]) {
	values[0] = elements[0];
	values[1] = elements[1];
	values[2] = elements[2];

	values[3] = elements[4];
	values[4] = elements[5];
	values[5] = elements[6];

	values[6] = elements[8];
	values[7] = elements[9];
	values[8] = elements[10];
}

Matrix3::Matrix3(const Matrix4 &m4) {
	values[0] = m4.values[0];
	values[1] = m4.values[1];
	values[2] = m4.values[2];

	values[3] = m4.values[4];
	values[4] = m4.values[5];
	values[5] = m4.values[6];

	values[6] = m4.values[8];
	values[7] = m4.values[9];
	values[8] = m4.values[10];
}

Matrix3::Matrix3(const Matrix2 &m2) {
	values[0] = m2.values[0];
	values[1] = m2.values[1];
	values[2] = 0;

	values[3] = m2.values[2];
	values[4] = m2.values[3];
	values[5] = 0.0f;

	values[6] = 0.0f;
	values[7] = 0.0f;
	values[8] = 1.0f;
}

Matrix3::Matrix3(const Quaternion &quat) {
	float yy = quat.y * quat.y;
	float zz = quat.z * quat.z;
	float xy = quat.x * quat.y;
	float zw = quat.z * quat.w;
	float xz = quat.x * quat.z;
	float yw = quat.y * quat.w;
	float xx = quat.x * quat.x;
	float yz = quat.y * quat.z;
	float xw = quat.x * quat.w;

	values[0] = 1 - 2 * yy - 2 * zz;
	values[1] = 2 * xy + 2 * zw;
	values[2] = 2 * xz - 2 * yw;

	values[3] = 2 * xy - 2 * zw;
	values[4] = 1 - 2 * xx - 2 * zz;
	values[5] = 2 * yz + 2 * xw;

	values[6] = 2 * xz + 2 * yw;
	values[7] = 2 * yz - 2 * xw;
	values[8] = 1 - 2 * xx - 2 * yy;
}


Matrix3::~Matrix3(void)	{

}

Matrix3 Matrix3::Rotation(float degrees, const Vector3 &inaxis)	 {
	Matrix3 m;

	Vector3 axis = inaxis;

	axis.Normalise();

	float c = cos(DegToRad(degrees));
	float s = sin(DegToRad(degrees));

	m.values[0]  = (axis.x * axis.x) * (1.0f - c) + c;
	m.values[1]  = (axis.y * axis.x) * (1.0f - c) + (axis.z * s);
	m.values[2]  = (axis.z * axis.x) * (1.0f - c) - (axis.y * s);

	m.values[3]  = (axis.x * axis.y) * (1.0f - c) - (axis.z * s);
	m.values[4]  = (axis.y * axis.y) * (1.0f - c) + c;
	m.values[5]  = (axis.z * axis.y) * (1.0f - c) + (axis.x * s);

	m.values[6]  = (axis.x * axis.z) * (1.0f - c) + (axis.y * s);
	m.values[7]  = (axis.y * axis.z) * (1.0f - c) - (axis.x * s);
	m.values[8]  = (axis.z * axis.z) * (1.0f - c) + c;

	return m;
}

Matrix3 Matrix3::Scale( const Vector3 &scale )	{
	Matrix3 m;

	m.values[0]  = scale.x;
	m.values[4]  = scale.y;
	m.values[8]  = scale.z;	

	return m;
}

void	Matrix3::ToZero()	{
	for(int i = 0; i < 9; ++i) {
		values[0] = 0.0f;
	}
}

//http://staff.city.ac.uk/~sbbh653/publications/euler.pdf
Vector3 Matrix3::ToEuler() const {
	float testVal = abs(values[2]) + 0.00001f;

	if (testVal < 1.0f) {
		float theta1 = -asin(values[2]);
		float theta2 = PI - theta1;

		float cost1 = cos(theta1);
		//float cost2 = cos(theta2);

		float psi1 = RadToDeg(atan2(values[5] / cost1, values[8] / cost1));
		//float psi2 = Maths::RadiansToDegrees(atan2(array[5] / cost2, array[8] / cost2));

		float phi1 = RadToDeg(atan2(values[1] / cost1, values[0] / cost1));
		//float phi2 = Maths::RadiansToDegrees(atan2(array[1] / cost2, array[0] / cost2));

		theta1 = RadToDeg(theta1);
		//theta2 = Maths::RadiansToDegrees(theta2);

		return Vector3(psi1, theta1, phi1);
	}
	else {
		float phi	= 0.0f;	//x


		float theta = 0.0f;	//y
		float psi	= 0.0f;	//z

		float delta = atan2(values[3], values[6]);

		if (values[2] < 0.0f) {
			theta = PI / 2.0f;
			psi = phi + delta;
		}
		else {
			theta = -PI / 2.0f;
			psi = phi + delta;
		}

		return Vector3(RadToDeg(psi), RadToDeg(theta), RadToDeg(phi));
	}
}

Matrix3 Matrix3::FromEuler(const Vector3 &euler) {
	Matrix3 m;

	float heading	= DegToRad(euler.y);
	float attitude	= DegToRad(euler.x);
	float bank		= DegToRad(euler.z);

	float ch = cos(heading);
	float sh = sin(heading);
	float ca = cos(attitude);
	float sa = sin(attitude);
	float cb = cos(bank);
	float sb = sin(bank);

	m.values[0] = ch * ca;
	m.values[3] = sh*sb - ch*sa*cb;
	m.values[6] = ch*sa*sb + sh*cb;
	m.values[1] = sa;
	m.values[4] = ca*cb;
	m.values[7] = -ca*sb;
	m.values[2] = -sh*ca;
	m.values[5] = sh*sa*cb + ch*sb;
	m.values[8] = -sh*sa*sb + ch*cb;

	return m;
}

Vector3 Matrix3::GetRow(unsigned int row) const {
	assert(row < 3);
	int start = row;
	return Vector3(
		values[start],
		values[start + 3],
		values[start + 6]
	);
}

void Matrix3::SetRow(unsigned int row, const Vector3 &val) {
	assert(row < 3);

	int start = 3 * row;

	values[start] = val.x;
	values[start + 3] = val.y;
	values[start + 6] = val.z;
}

Vector3 Matrix3::GetColumn(unsigned int column) const {
	assert(column < 3);
	int start = 3 * column;
	return Vector3(
		values[start],
		values[start + 1],
		values[start + 2]
	);
}

void Matrix3::SetColumn(unsigned int column, const Vector3 &val) {
	assert(column < 3);
	int start = 3 * column;
	values[start] = val.x;
	values[start + 1] = val.y;
	values[start + 2] = val.z;
}

Vector3 Matrix3::GetDiagonal() const {
	return Vector3(values[0], values[4], values[8]);
}

void	Matrix3::SetDiagonal(const Vector3 &in) {
	values[0] = in.x;
	values[4] = in.y;
	values[8] = in.z;
}

Vector3 Matrix3::operator*(const Vector3 &v) const {
	Vector3 vec;

	vec.x = v.x*values[0] + v.y*values[3] + v.z*values[6];
	vec.y = v.x*values[1] + v.y*values[4] + v.z*values[7];
	vec.z = v.x*values[2] + v.y*values[5] + v.z*values[8];

	return vec;
};