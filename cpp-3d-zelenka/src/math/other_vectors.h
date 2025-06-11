// file other_vectors.h
// vectors structure

#ifndef _OTHER_VECTOR3D_H_INC
#define _OTHER_VECTOR3D_H_INC

namespace ag {
template <typename T> class vec3t;

// ---------------------------------------------------------------------------
// Represents a 4-dimensional vector
template <typename Treal>
class vec4t {
    friend class vec3t<Treal>;
public:
	vec4t(){ buf[0] = buf[1] = buf[2] = buf[3] = Treal(0); }
	vec4t (Treal _x, Treal _y, Treal _z, Treal _w) { 
        buf[0] = _x; buf[1] = _y; buf[2] = _z; buf[3] = _w; 
    }
	explicit vec4t (Treal _v) { 
        buf[0] = buf[1] = buf[2] = buf[3] = _v; 
    }
	vec4t (const vec4t& o) { 
        buf[0] = o.buf[0]; buf[1] = o.buf[1]; buf[2] = o.buf[2]; buf[3] = o.buf[3]; 
    }
	vec4t (const vec3t<Treal>& o) { 
        buf[0] = o.buf[0]; buf[1] = o.buf[1]; buf[2] = o.buf[2]; buf[3] = 1.0f; 
    }

public:
// Set the components of a vector
    void set(Treal _x, Treal _y, Treal _z, Treal _w) { 
        buf[0]=_x; buf[1]=_y; buf[2]=_z; buf[3]=_w; }

    const Treal* ptr() const { return &buf[0]; }
    Treal x(void) const { return buf[0]; }
    Treal y(void) const { return buf[1]; }
    Treal z(void) const { return buf[2]; }
    Treal w(void) const { return buf[3]; }

    void setX(Treal x) { buf[0] = x; }
    void setY(Treal y) { buf[1] = y; }
    void setZ(Treal z) { buf[2] = z; }
    void setW(Treal w) { buf[3] = w; }
private:
	Treal buf[4];
};

typedef vec4t<float> vec4;

// ---------------------------------------------------------------------------
// Represents a two-dimensional vector
template <typename Treal>
class vec2t {
public:
	vec2t() { buf[0]=buf[1]=0.0f; }
	vec2t(Treal _x, Treal _y) { buf[0] = _x; buf[1] = _y; }
	explicit vec2t(Treal _v) { buf[0] = buf[1] = _v; }
	vec2t(const vec2t& o) { buf[0] = o.buf[0]; buf[1] = o.buf[1]; }

public:
// Set the components of a vector
    void set(Treal _x, Treal _y) { buf[0] = _x; buf[1] = _y; }

    const Treal* ptr() const { return &buf[0]; }

    Treal x(void) const { return buf[0]; }
    Treal y(void) const { return buf[1]; }

    void setX(Treal x) { buf[0] = x; }
    void setY(Treal y) { buf[1] = y; }

private:
	Treal buf[2];
};

typedef vec2t<float> vec2;

// ---------------------------------------------------------------------------
// Represents a three-dimensional vector
template <typename Tint>
class ivec3t {
public:
	ivec3t(){ buf[0] = buf[1] = buf[2] = Tint(0); }
	ivec3t(Tint _x, Tint _y, Tint _z) { 
        buf[0]=_x; buf[1]=_y; buf[2]=_z; 
    }
	explicit ivec3t(Tint _v) { 
        buf[0]=buf[1]=buf[2]=_v; 
    }
	ivec3t(const ivec3t& o) { 
        buf[0]=o.buf[0]; buf[1]=o.buf[1]; buf[2]=o.buf[2]; 
    }

public:
    void set(Tint _x, Tint _y, Tint _z) { buf[0]=_x; buf[1]=_y; buf[2]=_z; }

    const Tint* ptr() const { return &buf[0]; }

    Tint x(void) const { return buf[0]; }
    Tint y(void) const { return buf[1]; }
    Tint z(void) const { return buf[2]; }

    void setX(Tint x) { buf[0]=x; }
    void setY(int y) { buf[1]=y; }
    void setZ(Tint z) { buf[2]=z; }

private:
	Tint buf[3];
};

typedef ivec3t<unsigned int> uvec3;

// ---------------------------------------------------------------------------
// Represents a color vector
template <typename Treal>
class colort {
public:
	colort() { buf[0] = buf[1] = buf[2] = buf[3] = Treal(0); }

	colort(Treal _r, Treal _g, Treal _b, Treal _a) {  
        buf[0] = _r; buf[1] = _g; buf[2] = _b; buf[3] = _a; 
    }
	explicit colort(Treal _v) { 
        buf[0] = buf[1] = buf[2] = buf[3] = _v; 
    }
	colort(const colort& o) { 
        buf[0] = o.buf[0]; buf[1] = o.buf[1]; buf[2] = o.buf[2]; buf[3] = o.buf[3]; 
    }

public:
// Set the components of a vector
    void set(Treal _r, Treal _g, Treal _b, Treal _a) { buf[0]=_r; buf[1]=_g; buf[2]=_b; buf[3]=_a; }
    void set(const colort& color) { 
        buf[0] = color.r(); buf[1] = color.g(); buf[2] = color.b(); buf[3] = color.a(); 
    }

    const Treal* ptr() const { return &buf[0]; }

    Treal r(void) const { return buf[0]; }
    Treal g(void) const { return buf[1]; }
    Treal b(void) const { return buf[2]; }
    Treal a(void) const { return buf[3]; }

    void setR(Treal r) { buf[0]=r; }
    void setG(Treal g) { buf[1]=g; }
    void setB(Treal b) { buf[2]=b; }
    void setA(Treal a) { buf[3]=a; }

	bool comparison(const colort& a, float delta) 	{
		if (abs(a.buf[0] - buf[0]) > delta) return false;
		if (abs(a.buf[1] - buf[1]) > delta) return false;
		if (abs(a.buf[2] - buf[2]) > delta) return false;
		else return true;
	}

private:
	Treal buf[4];
};

typedef colort<float> color;

}//namespace ag

#endif // _OTHER_VECTOR3D_H_INC
