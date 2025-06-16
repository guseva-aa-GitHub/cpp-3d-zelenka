// file matrix4x4.h
// 4x4 matrix structure, including operators when compiling in C++

#pragma once

namespace ag
{

template<typename Treal> class mat3t;
template<typename Treal> class vec3t;

// ---------------------------------------------------------------------
// Represents a row-major 4x4 matrix, use this for homogeneous coordinates.
//
// There's much confusion about matrix layouts (column vs. row order).
// This is *always* a row-major matrix.

template<typename Treal>
class mat4t
{
    friend class mat3t<Treal>;
    friend class vec3t<Treal>;
public:
// set to identity
	mat4t();
// construction from single values
	mat4t(Treal _a1, Treal _a2, Treal _a3, Treal _a4,
            Treal _b1, Treal _b2, Treal _b3, Treal _b4,
			Treal _c1, Treal _c2, Treal _c3, Treal _c4,
			Treal _d1, Treal _d2, Treal _d3, Treal _d4);
// construction from 4x4 matrix
	mat4t(const mat4t<Treal>& m);
// construction from 3x3 matrix, remaining elements are set to identity
	mat4t(const mat3t<Treal>& m);

public:
    mat4t<Treal>& operator=(const mat4t<Treal>& m);

// matrix multiplication.
	mat4t<Treal>& operator *= (const mat4t<Treal>& m);
	mat4t<Treal> operator * (const mat4t<Treal>& m) const;
// matrix manipulation.
	mat4t<Treal>& operator += (const mat4t<Treal>& m);
	mat4t<Treal> operator + (const mat4t<Treal>& m) const;
    mat4t<Treal>& operator -= (const mat4t<Treal>& m);
	mat4t<Treal> operator - (const mat4t<Treal>& m) const;
// Transformation of a vector by a 4x4 matrix
    vec3t<Treal> operator * (const vec3t<Treal>& v);

public:
// -------------------------------------------------------------------
// brief Transpose the matrix
	mat4t<Treal>& transpose();
// -------------------------------------------------------------------
// brief Invert the matrix.
// If the matrix is not invertible all elements are set to qnan.
//  Beware, use (f != f) to check whether a Treal f is qnan.
	mat4t<Treal>& inverse();
	Treal determinant() const;
// -------------------------------------------------------------------
// Creates matrix from a set of euler angles
// param x Rotation angle for the x-axis, in degrees
// param y Rotation angle for the y-axis, in degrees
// param z Rotation angle for the z-axis, in degrees
	mat4t<Treal>& fromEulerAnglesXYZ(Treal x, Treal y, Treal z);
	mat4t<Treal>& fromEulerAnglesXYZ(const vec3t<Treal>& blubb);
public:
// -------------------------------------------------------------------
// brief Returns a rotation matrix for a rotation around the x axis
// param m the input matrix
// param a Rotation angle, in degrees
// return Reference to the output matrix
	static mat4t<Treal> rotationX(const mat4t<Treal>& m, Treal a);
// -------------------------------------------------------------------
// brief Returns a rotation matrix for a rotation around the y axis
// param m the input matrix
// param a Rotation angle, in degrees
// return Reference to the output matrix
	static mat4t<Treal> rotationY(const mat4t<Treal>& m, Treal a);
// -------------------------------------------------------------------
// brief Returns a rotation matrix for a rotation around the z axis
// param m the input matrix
// param a Rotation angle, in degrees
// return Reference to the output matrix
	static mat4t<Treal> rotationZ(const mat4t<Treal>& m, Treal a);
// -------------------------------------------------------------------
// Returns a rotation matrix for a rotation around an arbitrary axis.
// param m the input matrix
// param a Rotation angle, in degrees
// param axis Rotation axis, should be a normalized vector.
// return Reference to the output matrix
	static mat4t<Treal> rotation(const mat4t<Treal>& m, Treal a, const vec3t<Treal>& axis);
// -------------------------------------------------------------------
// Returns a translation matrix
// param m the input matrix
// param v Translation vector
// return Reference to the output matrix
	static mat4t<Treal> translation(const mat4t<Treal>& m, const vec3t<Treal>& v);
// -------------------------------------------------------------------
// Returns a scaling matrix
// param m the input matrix
// param v Scaling vector
// return Reference to the output matrix
	static mat4t<Treal> scaling(const mat4t<Treal>& m, const vec3t<Treal>& v);

// View matrix
	static const mat4t<Treal> lookAt(const vec3t<Treal>& eye, const vec3t<Treal>& target,
                                const vec3t<Treal>& up);
// Projection matrix
    static const mat4t<Treal> orthographic(Treal left, Treal right,
                                                    Treal bottom, Treal top, Treal zNear, Treal zFar);
    // fov in degrees
    static const mat4t<Treal> perspective(Treal fov, Treal aspect, Treal zNear, Treal zFar);
// Frustum
    static const mat4t<Treal> frustum(Treal left, Treal right,
                                                Treal bottom, Treal top, Treal zNear, Treal zFar);

// get ptr
    const Treal* ptr() const { return &buf[0]; }
    Treal& at(unsigned int i, unsigned int j);
    Treal at(unsigned int i, unsigned int j) const;

private:
    Treal buf[16];

};

typedef mat4t<float> mat4;
}

#include "matrix4x4.inl"
