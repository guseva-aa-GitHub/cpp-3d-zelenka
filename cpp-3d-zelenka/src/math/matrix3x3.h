// file matrix3x3.h
// Definition of a 3x3 matrix, including operators when compiling in C++

#pragma once

namespace ag
{

template <typename T> class mat4t;
template <typename T> class vec3t;

// ---------------------------------------------------------------------
// Represents a row-major 3x3 matrix
// There's much confusion about matrix layouts (column vs. row order).
// This is *always* a row-major matrix.

template <typename Treal>
class mat3t
{
    friend class mat4t<Treal>;
    friend class vec3t<Treal>;
public:
// construction
	mat3t();
	mat3t(Treal _a1, Treal _a2, Treal _a3,
			Treal _b1, Treal _b2, Treal _b3,
			Treal _c1, Treal _c2, Treal _c3);

// construction from 3x3 matrix
	mat3t(const mat3t<Treal>& m);
// Construction from a 4x4 matrix. The remaining parts
// of the matrix are ignored.
	mat3t(const mat4t<Treal>& m);

public:

	mat3t<Treal>& operator=  (const mat3t<Treal>& m);
// matrix multiplication.
	mat3t<Treal>& operator *= (const mat3t<Treal>& m);
	mat3t<Treal>  operator  * (const mat3t<Treal>& m) const;
// Transformation of a vector by a 3x3 matrix
    vec3t<Treal> operator * (const vec3t<Treal>& v) const;

public:

// -------------------------------------------------------------------
// Transpose the matrix
	mat3t<Treal>& transpose();

// -------------------------------------------------------------------
// Invert the matrix.
// If the matrix is not invertible all elements are set to qnan.
// Beware, use (f != f) to check whether a Treal f is qnan.
	mat3t<Treal>& inverse();
	Treal determinant() const;

// get ptr
    const Treal* ptr() const { return &buf[0]; }
    Treal& at(unsigned int i, unsigned int j);
    Treal at(unsigned int i, unsigned int j) const;

private:
    Treal buf[9];
};

typedef mat3t<float> mat3;

}//namespace ag

#include "matrix3x3.inl"
