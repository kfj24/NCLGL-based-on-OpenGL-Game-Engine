/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include <algorithm>
#include <iostream>

class Matrix2;
class Matrix4;
class Vector3;
class Quaternion;

class Matrix3
{
public:
	Matrix3(void);
	Matrix3(float elements[9]);
	Matrix3(const Matrix2 &m4);
	Matrix3(const Matrix4 &m4);
	Matrix3(const Quaternion& quat);

	~Matrix3(void);

	//Set all matrix values to zero
	void	ToZero();

	Vector3 GetRow(unsigned int row) const;
	void	SetRow(unsigned int row, const Vector3 &val);

	Vector3 GetColumn(unsigned int column) const;
	void	SetColumn(unsigned int column, const Vector3 &val);

	Vector3 GetDiagonal() const;
	void	SetDiagonal(const Vector3 &in);

	Vector3 ToEuler() const;

	inline Matrix3 Absolute() const {
		Matrix3 m;

		for (int i = 0; i < 9; ++i) {
			m.values[i] = std::abs(values[i]);
		}

		return m;
	}

	inline Matrix3 Transposed() const {
		Matrix3 temp = *this;
		temp.Transpose();
		return temp;
	}

	inline void Transpose() {
		float tempValues[3];

		tempValues[0] = values[3];
		tempValues[1] = values[6];
		tempValues[2] = values[7];

		values[3] = values[1];
		values[6] = values[2];
		values[7] = values[5];

		values[1] = tempValues[0];
		values[2] = tempValues[1];
		values[5] = tempValues[2];
	}

	Vector3 operator*(const Vector3 &v) const;

	inline Matrix3 operator*(const Matrix3 &a) const {
		Matrix3 out;
		//Students! You should be able to think up a really easy way of speeding this up...
		for (unsigned int r = 0; r < 3; ++r) {
			for (unsigned int c = 0; c < 3; ++c) {
				out.values[c + (r * 3)] = 0.0f;
				for (unsigned int i = 0; i < 3; ++i) {
					out.values[c + (r * 3)] += this->values[c + (i * 3)] * a.values[(r * 3) + i];
				}
			}
		}
		return out;
	}

	//Creates a rotation matrix that rotates by 'degrees' around the 'axis'
	//Analogous to glRotatef
	static Matrix3 Rotation(float degrees, const Vector3 &axis);

	//Creates a scaling matrix (puts the 'scale' vector down the diagonal)
	//Analogous to glScalef
	static Matrix3 Scale(const Vector3 &scale);

	static Matrix3 FromEuler(const Vector3 &euler);
public:
	float values[9];
};

//Handy string output for the matrix. Can get a bit messy, but better than nothing!
inline std::ostream& operator<<(std::ostream& o, const Matrix3& m) {
	o << m.values[0] << "," << m.values[1] << "," << m.values[2] << std::endl;
	o << m.values[3] << "," << m.values[4] << "," << m.values[5] << std::endl;
	o << m.values[6] << "," << m.values[7] << "," << m.values[8];
	return o;
}

inline std::istream& operator >> (std::istream& i, Matrix3& m) {
	char ignore;
	i >> std::skipws;
	i >> m.values[0] >> ignore >> m.values[1] >> ignore >> m.values[2];
	i >> m.values[3] >> ignore >> m.values[4] >> ignore >> m.values[5];
	i >> m.values[6] >> ignore >> m.values[7] >> ignore >> m.values[8];

	return i;
}
