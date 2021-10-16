/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <assert.h>
class Matrix2 {
public:
	Matrix2(void);
	Matrix2(float elements[4]);

	Matrix2(const Vector2& a, const Vector2& b);

	~Matrix2(void);

	void ToZero();

	void SetRow(unsigned int row, const Vector2 &val) {
		assert(row < 2);

		int start = 2 * row;

		values[start += 2] = val.x;
		values[start += 2] = val.y;
	}

	void SetColumn(unsigned int column, const Vector2 &val) {
		assert(column < 2);
		memcpy(&values[2 * column], &val, sizeof(Vector2));
	}

	Vector2 GetRow(unsigned int row) const {
		assert(row < 2);
		Vector2 out(0, 0);

		int start = 2 * row;

		out.x = values[start += 2];
		out.y = values[start += 2];
		return out;
	}

	Vector2 GetColumn(unsigned int column) const {
		assert(column < 2);
		Vector2 out(0, 0);

		memcpy(&out, &values[3 * column], sizeof(Vector2));

		return out;
	}

	Vector2 GetDiagonal() const {
		return Vector2(values[0], values[3]);
	}

	void	SetDiagonal(const Vector2 &in) {
		values[0] = in.x;
		values[3] = in.y;
	}

	inline Vector2 operator*(const Vector2 &v) const {
		Vector2 vec;

		vec.x = v.x*values[0] + v.y*values[2];
		vec.y = v.x*values[1] + v.y*values[3];

		return vec;
	};

	void Invert();	
	Matrix2 Inverse() const;

	static Matrix2 Rotation(float degrees);

	//Handy string output for the matrix. Can get a bit messy, but better than nothing!
	inline friend std::ostream& operator<<(std::ostream& o, const Matrix2& m);

	float	values[4];
};

//Handy string output for the matrix. Can get a bit messy, but better than nothing!
inline std::ostream& operator<<(std::ostream& o, const Matrix2& m) {
	o << "Mat2(";
	o << "\t" << m.values[0] << "," << m.values[2] << std::endl;
	o << "\t\t" << m.values[1] << "," << m.values[3] << std::endl;
	return o;
}
