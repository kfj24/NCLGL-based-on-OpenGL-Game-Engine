/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "Matrix2.h"
#include "common.h"

Matrix2::Matrix2(void)	{
	values[0] = 1.0f;
	values[1] = 0.0f;
	values[2] = 0.0f;
	values[3] = 1.0f;
}

Matrix2::Matrix2(float elements[4]) {
	values[0] = elements[0];
	values[1] = elements[1];
	values[2] = elements[2];
	values[3] = elements[3];
}

Matrix2::Matrix2(const Vector2& a, const Vector2& b) {
	values[0] = a.x;
	values[1] = b.x;
	values[2] = a.y;
	values[3] = b.y;
};

Matrix2::~Matrix2(void)	{
}

void Matrix2::ToZero() {
	values[0] = 0.0f;
	values[1] = 0.0f;
	values[2] = 0.0f;
	values[3] = 0.0f;
}

Matrix2 Matrix2::Rotation(float degrees)	{
	Matrix2 mat;

	float radians = (float)DegToRad(degrees);
	float s = sin(radians);
	float c = cos(radians);

	mat.values[0] = c;
	mat.values[1] = s;
	mat.values[2] = -s;
	mat.values[3] = c;

	return mat;
}
//This is going to assume that the matrix is actually invertable!
void Matrix2::Invert() {
	float determinant = (values[0] * values[3]) - (values[1] * values[2]);
	float invDet = 1.0f / determinant; //Turn our divides into multiplies!

	values[0] = values[3]  * invDet;
	values[1] = -values[2] * invDet;
	values[2] = -values[1] * invDet;
	values[3] = values[0]  * invDet;
}

Matrix2 Matrix2::Inverse() const {
	Matrix2 newMatrix = *this;
	newMatrix.Invert();
	return newMatrix;
}