// file matrix4x4.inl
// Inline implementation of the 4x4 matrix operators

#ifndef MATRIX4x4_INL_INC
#define MATRIX4x4_INL_INC

#include "matrix4x4.h"
#include "matrix3x3.h"
#include "vector3.h"

#include <limits>
#include <math.h>
#include <assert.h>

namespace ag
{

// ---------------------------------------------------------------------
template <typename Treal>
mat4t<Treal>::mat4t()
{
	buf[0]=1.0f; buf[1]=0.0f; buf[2]=0.0f; buf[3]=0.0f;
	buf[4]=0.0f; buf[5]=1.0f; buf[6]=0.0f; buf[7]=0.0f;
	buf[8]=0.0f; buf[9]=0.0f; buf[10]=1.0f; buf[11]=0.0f;
	buf[12]=0.0f; buf[13]=0.0f; buf[14]=0.0f; buf[15]=1.0f;
}

// ---------------------------------------------------------------------
template <typename Treal>
mat4t<Treal>::mat4t(
                    Treal _a1, Treal _a2, Treal _a3, Treal _a4,
                    Treal _b1, Treal _b2, Treal _b3, Treal _b4,
                    Treal _c1, Treal _c2, Treal _c3, Treal _c4,
                    Treal _d1, Treal _d2, Treal _d3, Treal _d4
                    )
{
	buf[0]=_a1; buf[1]=_a2; buf[2]=_a3; buf[3]=_a4;
	buf[4]=_b1; buf[5]=_b2; buf[6]=_b3; buf[7]=_b4;
	buf[8]=_c1; buf[9]=_c2; buf[10]=_c3; buf[11]=_c4;
	buf[12]=_d1; buf[13]=_d2; buf[14]=_d3; buf[15]=_d4;
}

// ---------------------------------------------------------------------
template <typename Treal>
mat4t<Treal>::mat4t(const mat4t<Treal>& m)
{
	buf[0] = m.buf[0]; buf[1] = m.buf[1]; buf[2] = m.buf[2]; buf[3] = m.buf[3];
	buf[4] = m.buf[4]; buf[5] = m.buf[5]; buf[6] = m.buf[6]; buf[7] = m.buf[7];
	buf[8] = m.buf[8]; buf[9] = m.buf[9]; buf[10] = m.buf[10]; buf[11] = m.buf[11];
	buf[12] = m.buf[12]; buf[13] = m.buf[13]; buf[14] = m.buf[14]; buf[15] = m.buf[15];
}

// ---------------------------------------------------------------------
template <typename Treal>
mat4t<Treal>::mat4t(const mat3t<Treal>& m)
{
	buf[0] = m.buf[0]; buf[1] = m.buf[1]; buf[2] = m.buf[2]; buf[3] = static_cast<Treal>(0.0f);
	buf[4] = m.buf[3]; buf[5] = m.buf[4]; buf[6] = m.buf[5]; buf[7] = static_cast<Treal>(0.0f);
	buf[8] = m.buf[6]; buf[9] = m.buf[7]; buf[10] = m.buf[8]; buf[11] = static_cast<Treal>(0.0f);
	buf[12] = static_cast<Treal>(0.0f); buf[13] = static_cast<Treal>(0.0f);
	buf[14] = static_cast<Treal>(0.0f);	buf[15] = static_cast<Treal>(1.0f);
}

template <typename Treal>
mat4t<Treal>& mat4t<Treal>::operator=(const mat4t<Treal>& m)
{
	buf[0] = m.buf[0]; buf[1] = m.buf[1]; buf[2] = m.buf[2]; buf[3] = m.buf[3];
	buf[4] = m.buf[4]; buf[5] = m.buf[5]; buf[6] = m.buf[6]; buf[7] = m.buf[7];
	buf[8] = m.buf[8]; buf[9] = m.buf[9]; buf[10] = m.buf[10]; buf[11] = m.buf[11];
	buf[12] = m.buf[12]; buf[13] = m.buf[13]; buf[14] = m.buf[14]; buf[15] = m.buf[15];
	return *this;
}

// ---------------------------------------------------------------------
template <typename Treal>
mat4t<Treal>& mat4t<Treal>::operator *= (const mat4t<Treal>& m)
{
	*this = mat4t<Treal>(
        m.buf[0] * buf[0] + m.buf[4] * buf[1] + m.buf[8] * buf[2] + m.buf[12] * buf[3],
		m.buf[1] * buf[0] + m.buf[5] * buf[1] + m.buf[9] * buf[2] + m.buf[13] * buf[3],
		m.buf[2] * buf[0] + m.buf[6] * buf[1] + m.buf[10] * buf[2] + m.buf[14] * buf[3],
		m.buf[3] * buf[0] + m.buf[7] * buf[1] + m.buf[11] * buf[2] + m.buf[15] * buf[3],
		m.buf[0] * buf[4] + m.buf[4] * buf[5] + m.buf[8] * buf[6] + m.buf[12] * buf[7],
		m.buf[1] * buf[4] + m.buf[5] * buf[5] + m.buf[9] * buf[6] + m.buf[13] * buf[7],
		m.buf[2] * buf[4] + m.buf[6] * buf[5] + m.buf[10] * buf[6] + m.buf[14] * buf[7],
		m.buf[3] * buf[4] + m.buf[7] * buf[5] + m.buf[11] * buf[6] + m.buf[15] * buf[7],
		m.buf[0] * buf[8] + m.buf[4] * buf[9] + m.buf[8] * buf[10] + m.buf[12] * buf[11],
		m.buf[1] * buf[8] + m.buf[5] * buf[9] + m.buf[9] * buf[10] + m.buf[13] * buf[11],
		m.buf[2] * buf[8] + m.buf[6] * buf[9] + m.buf[10] * buf[10] + m.buf[14] * buf[11],
		m.buf[3] * buf[8] + m.buf[7] * buf[9] + m.buf[11] * buf[10] + m.buf[15] * buf[11],
		m.buf[0] * buf[12] + m.buf[4] * buf[13] + m.buf[8] * buf[14] + m.buf[12] * buf[15],
		m.buf[1] * buf[12] + m.buf[5] * buf[13] + m.buf[9] * buf[14] + m.buf[13] * buf[15],
		m.buf[2] * buf[12] + m.buf[6] * buf[13] + m.buf[10] * buf[14] + m.buf[14] * buf[15],
		m.buf[3] * buf[12] + m.buf[7] * buf[13] + m.buf[11] * buf[14] + m.buf[15] * buf[15]
	);
	return *this;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat4t<Treal> mat4t<Treal>::operator* (const mat4t<Treal>& m) const
{
    mat4t<Treal> temp(*this);
	temp *= m;
	return temp;
}

template <typename Treal>
inline mat4t<Treal>& mat4t<Treal>::operator += (const mat4t<Treal>& m)
{
	*this = mat4t<Treal>(
        buf[0] += m.buf[0], buf[1] += m.buf[1], buf[2] += m.buf[2], buf[3] += m.buf[3],
        buf[4] += m.buf[4], buf[5] += m.buf[5], buf[6] += m.buf[6], buf[7] += m.buf[7],
        buf[8] += m.buf[8], buf[9] += m.buf[9], buf[10] += m.buf[10], buf[11] += m.buf[11],
        buf[12] += m.buf[12], buf[13] += m.buf[13], buf[14] += m.buf[14], buf[15] += m.buf[15]
        );
    return *this;
}

template <typename Treal>
inline mat4t<Treal>	mat4t<Treal>::operator + (const mat4t<Treal>& m) const
{
    mat4t<Treal> temp(*this);
	temp += m;
	return temp;
}

template <typename Treal>
inline mat4t<Treal>& mat4t<Treal>::operator -= (const mat4t<Treal>& m)
{
	*this = mat4t<Treal>(
        buf[0] -= m.buf[0], buf[1] -= m.buf[1], buf[2] -= m.buf[2], buf[3] -= m.buf[3],
        buf[4] -= m.buf[4], buf[5] -= m.buf[5], buf[6] -= m.buf[6], buf[7] -= m.buf[7],
        buf[8] -= m.buf[8], buf[9] -= m.buf[9], buf[10] -= m.buf[10], buf[11] -= m.buf[11],
        buf[12] -= m.buf[12], buf[13] -= m.buf[13], buf[14] -= m.buf[14], buf[15] -= m.buf[15]
        );
    return *this;
}

template <typename Treal>
inline mat4t<Treal>	mat4t<Treal>::operator - (const mat4t<Treal>& m) const
{
    mat4t<Treal> temp(*this);
	temp -= m;
	return temp;
}


// Transformation of a vector by a 4x4 matrix
template <typename Treal>
inline vec3t<Treal> mat4t<Treal>::operator * (const vec3t<Treal>& v)
{
	vec3t<Treal> res;
	res.buf[0] = buf[0] * v.buf[0] + buf[1] * v.buf[1] + buf[2] * v.buf[2] + buf[3];
	res.buf[1] = buf[4] * v.buf[0] + buf[5] * v.buf[1] + buf[6] * v.buf[2] + buf[7];
	res.buf[2] = buf[8] * v.buf[0] + buf[9] * v.buf[1] + buf[10] * v.buf[2] + buf[11];
	return res;
}

// ---------------------------------------------------------------------
template <typename Treal>
mat4t<Treal>& mat4t<Treal>::transpose()
{
    Treal tmp;
    tmp=buf[4]; buf[4]=buf[1]; buf[1]=tmp;
    tmp=buf[8]; buf[8]=buf[2]; buf[2]=tmp;
    tmp=buf[9]; buf[9]=buf[6]; buf[6]=tmp;
    tmp=buf[12]; buf[12]=buf[3]; buf[3]=tmp;
    tmp=buf[13]; buf[13]=buf[7]; buf[7]=tmp;
    tmp=buf[14]; buf[14]=buf[11]; buf[11]=tmp;
	return *this;
}


// ---------------------------------------------------------------------
template <typename Treal>
Treal mat4t<Treal>::determinant() const
{
	return
    buf[0]*buf[5]*buf[10]*buf[15] - buf[0]*buf[5]*buf[11]*buf[14] + buf[0]*buf[6]*buf[11]*buf[13] - buf[0]*buf[6]*buf[9]*buf[15]
	+ buf[0]*buf[7]*buf[9]*buf[14] - buf[0]*buf[7]*buf[10]*buf[13] - buf[1]*buf[6]*buf[11]*buf[12] + buf[1]*buf[6]*buf[8]*buf[15]
	- buf[1]*buf[7]*buf[8]*buf[14] + buf[1]*buf[7]*buf[10]*buf[12] - buf[1]*buf[4]*buf[10]*buf[15] + buf[1]*buf[4]*buf[11]*buf[14]
	+ buf[2]*buf[7]*buf[8]*buf[13] - buf[2]*buf[7]*buf[9]*buf[12] + buf[2]*buf[4]*buf[9]*buf[15] - buf[2]*buf[4]*buf[11]*buf[13]
	+ buf[2]*buf[5]*buf[11]*buf[12] - buf[2]*buf[5]*buf[8]*buf[15] - buf[3]*buf[4]*buf[9]*buf[14] + buf[3]*buf[4]*buf[10]*buf[13]
	- buf[3]*buf[5]*buf[10]*buf[12] + buf[3]*buf[5]*buf[8]*buf[14] - buf[3]*buf[6]*buf[8]*buf[13] + buf[3]*buf[6]*buf[9]*buf[12];
}

// ---------------------------------------------------------------------
template <typename Treal>
mat4t<Treal>& mat4t<Treal>::inverse()
{
	// Compute the reciprocal determinant
	const Treal det = determinant();
	if(det == static_cast<Treal>(0.0f))
	{
		// Matrix not invertible. Setting all elements to nan is not really
		// correct in a mathematical sense but it is easy to debug for the
		// programmer.
		const Treal nan = std::numeric_limits<Treal>::quiet_NaN();
		*this = mat4t<Treal>(
			nan,nan,nan,nan,
			nan,nan,nan,nan,
			nan,nan,nan,nan,
			nan,nan,nan,nan);

		return *this;
	}

	const Treal invdet = static_cast<Treal>(1.0f) / det;

	mat4t<Treal> res;
	res.buf[0] = invdet  * (buf[5] * (buf[10] * buf[15] - buf[11] * buf[14]) + buf[6] * (buf[11] * buf[13] - buf[9] * buf[15]) + buf[7] * (buf[9] * buf[14] - buf[10] * buf[13]));
	res.buf[1] = -invdet * (buf[1] * (buf[10] * buf[15] - buf[11] * buf[14]) + buf[2] * (buf[11] * buf[13] - buf[9] * buf[15]) + buf[3] * (buf[9] * buf[14] - buf[10] * buf[13]));
	res.buf[2] = invdet  * (buf[1] * (buf[6] * buf[15] - buf[7] * buf[14]) + buf[2] * (buf[7] * buf[13] - buf[5] * buf[15]) + buf[3] * (buf[5] * buf[14] - buf[6] * buf[13]));
	res.buf[3] = -invdet * (buf[1] * (buf[6] * buf[11] - buf[7] * buf[10]) + buf[2] * (buf[7] * buf[9] - buf[5] * buf[11]) + buf[3] * (buf[5] * buf[10] - buf[6] * buf[9]));
	res.buf[4] = -invdet * (buf[4] * (buf[10] * buf[15] - buf[11] * buf[14]) + buf[6] * (buf[11] * buf[12] - buf[8] * buf[15]) + buf[7] * (buf[8] * buf[14] - buf[10] * buf[12]));
	res.buf[5] = invdet  * (buf[0] * (buf[10] * buf[15] - buf[11] * buf[14]) + buf[2] * (buf[11] * buf[12] - buf[8] * buf[15]) + buf[3] * (buf[8] * buf[14] - buf[10] * buf[12]));
	res.buf[6] = -invdet * (buf[0] * (buf[6] * buf[15] - buf[7] * buf[14]) + buf[2] * (buf[7] * buf[12] - buf[4] * buf[15]) + buf[3] * (buf[4] * buf[14] - buf[6] * buf[12]));
	res.buf[7] = invdet  * (buf[0] * (buf[6] * buf[11] - buf[7] * buf[10]) + buf[2] * (buf[7] * buf[8] - buf[4] * buf[11]) + buf[3] * (buf[4] * buf[10] - buf[6] * buf[8]));
	res.buf[8] = invdet  * (buf[4] * (buf[9] * buf[15] - buf[11] * buf[13]) + buf[5] * (buf[11] * buf[12] - buf[8] * buf[15]) + buf[7] * (buf[8] * buf[13] - buf[9] * buf[12]));
	res.buf[9] = -invdet * (buf[0] * (buf[9] * buf[15] - buf[11] * buf[13]) + buf[1] * (buf[11] * buf[12] - buf[8] * buf[15]) + buf[3] * (buf[8] * buf[13] - buf[9] * buf[12]));
	res.buf[10] = invdet  * (buf[0] * (buf[5] * buf[15] - buf[7] * buf[13]) + buf[1] * (buf[7] * buf[12] - buf[4] * buf[15]) + buf[3] * (buf[4] * buf[13] - buf[5] * buf[12]));
	res.buf[11] = -invdet * (buf[0] * (buf[5] * buf[11] - buf[7] * buf[9]) + buf[1] * (buf[7] * buf[8] - buf[4] * buf[11]) + buf[3] * (buf[4] * buf[9] - buf[5] * buf[8]));
	res.buf[12] = -invdet * (buf[4] * (buf[9] * buf[14] - buf[10] * buf[13]) + buf[5] * (buf[10] * buf[12] - buf[8] * buf[14]) + buf[6] * (buf[8] * buf[13] - buf[9] * buf[12]));
	res.buf[13] = invdet  * (buf[0] * (buf[9] * buf[14] - buf[10] * buf[13]) + buf[1] * (buf[10] * buf[12] - buf[8] * buf[14]) + buf[2] * (buf[8] * buf[13] - buf[9] * buf[12]));
	res.buf[14] = -invdet * (buf[0] * (buf[5] * buf[14] - buf[6] * buf[13]) + buf[1] * (buf[6] * buf[12] - buf[4] * buf[14]) + buf[2] * (buf[4] * buf[13] - buf[5] * buf[12]));
	res.buf[15] = invdet  * (buf[0] * (buf[5] * buf[10] - buf[6] * buf[9]) + buf[1] * (buf[6] * buf[8] - buf[4] * buf[10]) + buf[2] * (buf[4] * buf[9] - buf[5] * buf[8]));
	*this = res;
	return *this;
}


// ---------------------------------------------------------------------
template <typename Treal>
inline mat4t<Treal>& mat4t<Treal>::fromEulerAnglesXYZ(const vec3t<Treal>& blubb)
{
	return FromEulerAnglesXYZ(blubb.x,blubb.y,blubb.z);
}

// ---------------------------------------------------------------------
template <typename Treal>
mat4t<Treal>& mat4t<Treal>::fromEulerAnglesXYZ(Treal x, Treal y, Treal z)
{
    x = ag::math::radians(x);
    y = ag::math::radians(y);
    z = ag::math::radians(z);

	mat4t<Treal>& _this = *this;
	Treal cr = cos( x );
	Treal sr = sin( x );
	Treal cp = cos( y );
	Treal sp = sin( y );
	Treal cy = cos( z );
	Treal sy = sin( z );

    _this.buf[0] = cp*cy ;
	_this.buf[1] = cp*sy;
	_this.buf[2] = -sp ;

    Treal srsp = sr*sp;
	Treal crsp = cr*sp;

	_this.buf[4] = srsp*cy-cr*sy ;
	_this.buf[5] = srsp*sy+cr*cy ;
	_this.buf[6] = sr*cp ;

	_this.buf[8] =  crsp*cy+sr*sy ;
	_this.buf[9] =  crsp*sy-sr*cy ;
	_this.buf[10] = cr*cp ;
	return *this;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat4t<Treal> mat4t<Treal>::rotationX(const mat4t<Treal>& m, Treal a)
{
    a = ag::math::radians(a);
	/*
	     |  1  0       0       0 |
     M = |  0  cos(A) -sin(A)  0 |
         |  0  sin(A)  cos(A)  0 |
         |  0  0       0       1 |	*/
	mat4t<Treal> out;
	out.buf[5] = out.buf[10] = cos(a);
	out.buf[6] = -(out.buf[9] = sin(a));
	return m*out;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat4t<Treal> mat4t<Treal>::rotationY(const mat4t<Treal>& m, Treal a)
{
    a = ag::math::radians(a);
	/*
	     |  cos(A)  0   sin(A)  0 |
     M = |  0       1   0       0 |
         | -sin(A)  0   cos(A)  0 |
         |  0       0   0       1 |
		*/
	mat4t<Treal> out;
	out.buf[0] = out.buf[10] = cos(a);
	out.buf[8] = -(out.buf[2] = sin(a));
	return m*out;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat4t<Treal> mat4t<Treal>::rotationZ(const mat4t<Treal>& m, Treal a)
{
    a = ag::math::radians(a);
	/*
	     |  cos(A)  -sin(A)   0   0 |
     M = |  sin(A)   cos(A)   0   0 |
         |  0        0        1   0 |
         |  0        0        0   1 |	*/
	mat4t<Treal> out;
	out.buf[0] = out.buf[5] = cos(a);
	out.buf[1] = -(out.buf[4] = sin(a));
	return m*out;
}

// ---------------------------------------------------------------------
// Returns a rotation matrix for a rotation around an arbitrary axis.
template <typename Treal>
inline mat4t<Treal> mat4t<Treal>::rotation(const mat4t<Treal>& m, Treal a, const vec3t<Treal>& axis)
{
    a = ag::math::radians(a);
    Treal c = cos(a), s = sin(a), t = 1 - c;
    vec3t<Treal> v(axis); v.normalize();
    Treal x = v.buf[0], y = v.buf[1], z = v.buf[2];

    // Many thanks to MathWorld and Wikipedia
    mat4t<Treal> out;
    out.buf[0] = t*x*x + c;   out.buf[1] = t*x*y - s*z; out.buf[2] = t*x*z + s*y;
    out.buf[4] = t*x*y + s*z; out.buf[5] = t*y*y + c;   out.buf[6] = t*y*z - s*x;
    out.buf[8] = t*x*z - s*y; out.buf[9] = t*y*z + s*x; out.buf[10] = t*z*z + c;

    return m*out;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat4t<Treal> mat4t<Treal>::translation(const mat4t<Treal>& m, const vec3t<Treal>& v)
{
	mat4t<Treal> out;
	out.buf[3] = v.buf[0];
	out.buf[7] = v.buf[1];
	out.buf[11] = v.buf[2];
	return m*out;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat4t<Treal> mat4t<Treal>::scaling(const mat4t<Treal>& m, const vec3t<Treal>& v)
{
	mat4t<Treal> out;
	out.buf[0] = v.buf[0];
	out.buf[5] = v.buf[1];
	out.buf[10] = v.buf[2];
	return m*out;
}

template <typename Treal>
inline const mat4t<Treal> mat4t<Treal>::lookAt(const vec3t<Treal>& eye, const vec3t<Treal>& target,
                                                const vec3t<Treal>& up)
{
	vec3t<Treal> f = vec3t<Treal>::normalize(eye - target);
    vec3t<Treal> s = vec3t<Treal>::normalize(vec3t<Treal>::cross(up, f));
    vec3t<Treal> u = vec3t<Treal>::normalize(vec3t<Treal>::cross(f, s));

	return mat4t<Treal>(s.x(), s.y(), s.z(), -vec3t<Treal>::dot(s, eye),
                        u.x(), u.y(), u.z(), -vec3t<Treal>::dot(u, eye),
                        f.x(), f.y(), f.z(), -vec3t<Treal>::dot(f, eye),
                        0.0f, 0.0f, 0.0f, 1.0f);
}

template <typename Treal>
inline const mat4t<Treal> mat4t<Treal>::orthographic(Treal left, Treal right,
                                                    Treal bottom, Treal top, Treal zNear, Treal zFar)
{
	const Treal tx = - (right + left) / (right - left),
	            ty = - (top + bottom) / (top - bottom),
	            tz = - (zFar + zNear) / (zFar - zNear);

	return mat4t<Treal>(2.0f / (right - left), 0.0f, 0.0f, tx,
                        0.0f, 2.0f / (top - bottom), 0.0f, ty,
                        0.0f, 0.0f, -2.0f / (zFar - zNear), tz,
                        0.0f, 0.0f, 0.0f, 1.0f);
}

template <typename Treal>
inline const mat4t<Treal> mat4t<Treal>::perspective(Treal fov, Treal aspect, Treal zNear, Treal zFar)
{
    fov = ag::math::radians(fov);
	const Treal f = 1.0f / tan(fov / 2.0f),
	            A = (zFar + zNear) / (zNear - zFar),
	            B = (2.0f * zFar * zNear) / (zNear - zFar);

	return mat4t<Treal>(f / aspect, 0.0f, 0.0f, 0.0f,
                        0.0f, f, 0.0f, 0.0f,
                        0.0f, 0.0f, A, B,
                        0.0f, 0.0f, -1.0f, 0.0f);
}

template <typename Treal>
inline const mat4t<Treal> mat4t<Treal>::frustum(Treal left, Treal right,
                                                Treal bottom, Treal top, Treal zNear, Treal zFar)
{
	return mat4t<Treal>((2.0f * zNear) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
                        0.0f, (2.0f * zNear) / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
                        0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -(2.0f * zFar * zNear) / (zFar - zNear),
                        0.0f, 0.0f, -1.0f, 0.0f);
}

template <typename Treal>
Treal& mat4t<Treal>::at(unsigned int i, unsigned int j)
{
    assert(i<4 && j<4);
    return buf[i*4+j];
}

template <typename Treal>
Treal mat4t<Treal>::at(unsigned int i, unsigned int j) const
{
    assert(i<4 && j<4);
    return buf[i*4+j];
}


}//namespace ag

#endif // MATRIX4x4_INL_INC
