// file vector3.h
// 3D vector structure, including operators when compiling in C++

#ifndef _VECTOR3D_H_INC
#define _VECTOR3D_H_INC

namespace ag
{

template<typename Treal> class mat3t;
template<typename Treal> class mat4t;
template<typename Treal> class vec4t;
// ---------------------------------------------------------------------------
// Represents a three-dimensional vector
template <typename Treal>
class vec3t
{
    friend class mat4t<Treal>;
    friend class mat3t<Treal>;
    friend class vec4t<Treal>;
public:
	vec3t();
	vec3t(Treal _x, Treal _y, Treal _z);
	explicit vec3t(Treal _v);
	vec3t(const vec3t& o);

public:
    vec3t<Treal>& operator=  (const vec3t<Treal>& o);

// combined operators
	vec3t<Treal>& operator += (const vec3t<Treal>& o);
	vec3t<Treal> operator + (const vec3t<Treal>& o) const;
	vec3t<Treal>& operator -= (const vec3t<Treal>& o);
	vec3t<Treal> operator - (const vec3t<Treal>& o) const;
	vec3t<Treal>& operator *= (const Treal f);
	vec3t<Treal> operator * (const Treal f);
	vec3t<Treal>& operator /= (const Treal f);
	vec3t<Treal> operator / (const Treal f);

public:

// Set the components of a vector
	void set(Treal x, Treal y, Treal z) { buf[0]=x; buf[1]=y; buf[2]=z; };
	void set(Treal x) { buf[0]=buf[1]=buf[2]=x; };

// Get the squared length of the vector
// return Square length
	Treal squareLength() const;

// Get the length of the vector
// return length
	Treal length() const;

// Normalize the vector
       vec3t<Treal>& normalize();
static vec3t<Treal> normalize(const vec3t<Treal>& v);

// Scalar product
static Treal dot(const vec3t<Treal>& v1, const vec3t<Treal>& v2);

// Cross product
static vec3t<Treal> cross( const vec3t<Treal>& v1, const vec3t<Treal>& v2);

    const Treal* ptr() const { return &buf[0]; }

    Treal x() const 	{ return buf[0]; }
    Treal y() const 	{ return buf[1]; }
    Treal z() const 	{ return buf[2]; }


    void setX(Treal x) 	{ buf[0] = x; }
    void setY(Treal y) 	{ buf[1] = y; }
    void setZ(Treal z) 	{ buf[2] = z; }

    void addX(Treal x) 	{ buf[0] += x; }
    void addY(Treal y) 	{ buf[1] += y; }
    void addZ(Treal z) 	{ buf[2] += z; }

private:
	Treal buf[3];
};


typedef vec3t<float> vec3;

}//namespace ag

#include "vector3.inl"

#endif // _VECTOR3D_H_INC
