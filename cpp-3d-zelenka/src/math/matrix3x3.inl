// file matrix3x3.inl
// Inline implementation of the 3x3 matrix operators

#pragma once

#include "base.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include <limits>
#include <assert.h>

namespace ag
{
// ---------------------------------------------------------------------
// Construction
template <typename Treal>
inline mat3t<Treal>::mat3t()
{
	buf[0]=1.0f; buf[1]=buf[2]=0.0f;
	buf[3]=0.0f, buf[4]=1.0f; buf[5]=0.0f;
	buf[6]=buf[7]=0.0f; buf[8]=1.0f;
}
template <typename Treal>
inline mat3t<Treal>::mat3t(Treal _a1, Treal _a2, Treal _a3,
                            Treal _b1, Treal _b2, Treal _b3,
                            Treal _c1, Treal _c2, Treal _c3)
{
	buf[0] = _a1; buf[1] = _a2; buf[2] = _a3;
	buf[3] = _b1; buf[4] = _b2; buf[5] = _b3;
	buf[6] = _c1; buf[7] = _c2; buf[8] = _c3;
}
// ---------------------------------------------------------------------
// Construction from a 3x3 matrix.
template <typename Treal>
inline mat3t<Treal>::mat3t (const mat3t<Treal>& m)
{
	buf[0] = m.buf[0]; buf[1] = m.buf[1]; buf[2] = m.buf[2];
	buf[3] = m.buf[3]; buf[4] = m.buf[4]; buf[5] = m.buf[5];
	buf[6] = m.buf[6]; buf[7] = m.buf[7]; buf[8] = m.buf[8];
}


// ---------------------------------------------------------------------
// Construction from a 4x4 matrix. The remaining parts of the matrix are ignored.
template <typename Treal>
inline mat3t<Treal>::mat3t( const mat4t<Treal>& m)
{
	buf[0] = m.buf[0]; buf[1] = m.buf[1]; buf[2] = m.buf[2];
	buf[3] = m.buf[4]; buf[4] = m.buf[5]; buf[5] = m.buf[6];
	buf[6] = m.buf[8]; buf[7] = m.buf[9]; buf[8] = m.buf[10];
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat3t<Treal>& mat3t<Treal>::operator= (const mat3t<Treal>& m)
{
	buf[0] = m.buf[0]; buf[1] = m.buf[1]; buf[2] = m.buf[2];
	buf[3] = m.buf[3]; buf[4] = m.buf[4]; buf[5] = m.buf[5];
	buf[6] = m.buf[6]; buf[7] = m.buf[7]; buf[8] = m.buf[8];
	return *this;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat3t<Treal>& mat3t<Treal>::operator *= (const mat3t<Treal>& m)
{
	*this = mat3t<Treal>(m.buf[0] * buf[0] + m.buf[3] * buf[1] + m.buf[6] * buf[2],
		m.buf[1] * buf[0] + m.buf[4] * buf[1] + m.buf[7] * buf[2],
		m.buf[2] * buf[0] + m.buf[5] * buf[1] + m.buf[8] * buf[2],
		m.buf[0] * buf[3] + m.buf[3] * buf[4] + m.buf[6] * buf[5],
		m.buf[1] * buf[3] + m.buf[4] * buf[4] + m.buf[7] * buf[5],
		m.buf[2] * buf[3] + m.buf[5] * buf[4] + m.buf[8] * buf[5],
		m.buf[0] * buf[6] + m.buf[3] * buf[7] + m.buf[6] * buf[8],
		m.buf[1] * buf[6] + m.buf[4] * buf[7] + m.buf[7] * buf[8],
		m.buf[2] * buf[6] + m.buf[5] * buf[7] + m.buf[8] * buf[8]);
	return *this;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat3t<Treal> mat3t<Treal>::operator* (const mat3t<Treal>& m) const
{
	mat3t<Treal> temp( *this);
	temp *= m;
	return temp;
}

// Transformation of a vector by a 4x4 matrix
template <typename Treal>
inline vec3t<Treal> mat3t<Treal>::operator * (const vec3t<Treal>& v) const
{
	vec3t<Treal> res;
	res.buf[0] = buf[0] * v.buf[0] + buf[1] * v.buf[1] + buf[2] * v.buf[2];
	res.buf[1] = buf[3] * v.buf[0] + buf[4] * v.buf[1] + buf[5] * v.buf[2];
	res.buf[2] = buf[6] * v.buf[0] + buf[7] * v.buf[1] + buf[8] * v.buf[2];
	return res;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat3t<Treal>& mat3t<Treal>::transpose()
{
    Treal tmp;
	tmp=buf[1]; buf[1]=buf[3]; buf[3]=tmp;
	tmp=buf[2]; buf[2]=buf[6]; buf[6]=tmp;
	tmp=buf[5]; buf[5]=buf[7]; buf[7]=tmp;
	return *this;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline Treal mat3t<Treal>::determinant() const
{
	return buf[0]*buf[4]*buf[8] - buf[0]*buf[5]*buf[7] + buf[1]*buf[5]*buf[6] - buf[1]*buf[3]*buf[8] + buf[2]*buf[3]*buf[7] - buf[2]*buf[4]*buf[6];
}

// ---------------------------------------------------------------------
template <typename Treal>
inline mat3t<Treal>& mat3t<Treal>::inverse()
{
	// Compute the reciprocal determinant
	Treal det = determinant();
	if(det == static_cast<Treal>(0.0f))
	{
		// Matrix not invertible. Setting all elements to nan is not really
		// correct in a mathematical sense; but at least qnans are easy to
		// spot. We might throw an exception instead, which would
		// be even much better to spot :/.
		const Treal nan = std::numeric_limits<Treal>::quiet_NaN();
		*this = mat3t<Treal>( nan,nan,nan,nan,nan,nan,nan,nan,nan);

		return *this;
	}

	Treal invdet = static_cast<Treal>(1.0f) / det;

	mat3t<Treal> res;
	res.buf[0] = invdet  * (buf[4] * buf[8] - buf[5] * buf[7]);
	res.buf[1] = -invdet * (buf[1] * buf[8] - buf[2] * buf[7]);
	res.buf[2] = invdet  * (buf[1] * buf[5] - buf[2] * buf[4]);
	res.buf[3] = -invdet * (buf[3] * buf[8] - buf[5] * buf[6]);
	res.buf[4] = invdet  * (buf[0] * buf[8] - buf[2] * buf[6]);
	res.buf[5] = -invdet * (buf[0] * buf[5] - buf[2] * buf[3]);
	res.buf[6] = invdet  * (buf[3] * buf[7] - buf[4] * buf[6]);
	res.buf[7] = -invdet * (buf[0] * buf[7] - buf[1] * buf[6]);
	res.buf[8] = invdet  * (buf[0] * buf[4] - buf[1] * buf[3]);
	*this = res;

	return *this;
}

template <typename Treal>
Treal& mat3t<Treal>::at(unsigned int i, unsigned int j)
{
    assert(i<3 && j<4);
    return buf[i*3+j];
}

template <typename Treal>
Treal mat3t<Treal>::at(unsigned int i, unsigned int j) const
{
    assert(i<3 && j<4);
    return buf[i*3+j];
}


} //namespace ag

