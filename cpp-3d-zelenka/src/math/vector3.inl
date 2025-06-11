// file  vector3.inl
// Inline implementation of vec3

#ifndef _VECTOR3_INL_INC
#define _VECTOR3_INL_INC

#include "vector3.h"
#include <math.h>

namespace ag
{

template <typename Treal>
vec3t<Treal>::vec3t()
{
    buf[0]=buf[1]=buf[2]=0.0f;
}
template <typename Treal>
vec3t<Treal>::vec3t(Treal _x, Treal _y, Treal _z)
{
    buf[0]=_x; buf[1]=_y; buf[2]=_z;
}
template <typename Treal>
vec3t<Treal>::vec3t(Treal _v)
{
    buf[0]=buf[1]=buf[2]=_v;
}
template <typename Treal>
vec3t<Treal>::vec3t(const vec3t& o)
{
    buf[0]=o.buf[0]; buf[1]=o.buf[1]; buf[2]=o.buf[2];
}

// ---------------------------------------------------------------------
template <typename Treal>
inline vec3t<Treal>& vec3t<Treal>::operator=  (const vec3t<Treal>& o)
{
    buf[0]=o.buf[0]; buf[1]=o.buf[1]; buf[2]=o.buf[2]; return *this;
}

template <typename Treal>
inline vec3t<Treal>& vec3t<Treal>::operator += (const vec3t<Treal>& o)
{
	buf[0] += o.buf[0]; buf[1] += o.buf[1]; buf[2] += o.buf[2]; return *this;
}
// ---------------------------------------------------------------------
template <typename Treal>
inline vec3t<Treal> vec3t<Treal>::operator + (const vec3t<Treal>& o) const
{
	vec3t<Treal> temp(*this);
	temp += o;
	return temp;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline vec3t<Treal>& vec3t<Treal>::operator -= (const vec3t<Treal>& o)
{
	buf[0] -= o.buf[0]; buf[1] -= o.buf[1]; buf[2] -= o.buf[2]; return *this;
}
// ---------------------------------------------------------------------
template <typename Treal>
inline vec3t<Treal> vec3t<Treal>::operator - (const vec3t<Treal>& o) const
{
	vec3t<Treal> temp(*this);
	temp -= o;
	return temp;
}

// ---------------------------------------------------------------------
template <typename Treal>
inline vec3t<Treal>& vec3t<Treal>::operator *= (const Treal f)
{
	buf[0] *= f; buf[1] *= f; buf[2] *= f; return *this;
}
// ---------------------------------------------------------------------
template <typename Treal>
inline vec3t<Treal> vec3t<Treal>::operator * (const Treal f)
{
	return vec3t<Treal>(buf[0]*f, buf[1]*f, buf[2]*f);
}
// ---------------------------------------------------------------------
template <typename Treal>
inline vec3t<Treal>& vec3t<Treal>::operator /= (const Treal f)
{
	buf[0] /= f; buf[1] /= f; buf[2] /= f; return *this;
}
// ---------------------------------------------------------------------
template <typename Treal>
inline vec3t<Treal> vec3t<Treal>::operator / (const Treal f)
{
	return vec3t<Treal>(buf[0]/f, buf[1]/f, buf[2]/f);
}

// ---------------------------------------------------------------------
template <typename Treal>
inline Treal vec3t<Treal>::squareLength() const
{
	return buf[0]*buf[0] + buf[1]*buf[1] + buf[2]*buf[2];
}

// ---------------------------------------------------------------------
template <typename Treal>
inline Treal vec3t<Treal>::length() const
{
	return sqrt(squareLength());
}
// ---------------------------------------------------------------------
template <typename Treal>
inline vec3t<Treal>& vec3t<Treal>::normalize()
{
	*this /= length(); return *this;
}
template <typename Treal>
inline vec3t<Treal> vec3t<Treal>::normalize(const vec3t<Treal>& v)
{
    vec3t<Treal> res(v);
    res /= res.length();
    return res;
}

// ---------------------------------------------------------------------
// scalar product
template <typename Treal>
inline Treal vec3t<Treal>::dot(const vec3t<Treal>& v1, const vec3t<Treal>& v2)
{
	return v1.buf[0]*v2.buf[0] + v1.buf[1]*v2.buf[1] + v1.buf[2]*v2.buf[2];
}
// ---------------------------------------------------------------------
// cross product
template <typename Treal>
inline  vec3t<Treal> vec3t<Treal>::cross( const vec3t<Treal>& v1, const vec3t<Treal>& v2)
{
	return vec3t<Treal>( v1.buf[1]*v2.buf[2] - v1.buf[2]*v2.buf[1], v1.buf[2]*v2.buf[0] - v1.buf[0]*v2.buf[2], v1.buf[0]*v2.buf[1] - v1.buf[1]*v2.buf[0]);
}

}//namespace ag

#endif // _VECTOR3_INL_INC
