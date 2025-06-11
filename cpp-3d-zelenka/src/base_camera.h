#ifndef _BASE_CAMERA_H
#define _BASE_CAMERA_H

#include <string>
#include <tinyxml.h>

#include "math/vector3.h"
#include "math/matrix4x4.h"

#include "itransform.h"
#include "object_type.h"

void print_vec3(const char* name,ag::vec3& v);
void print_float(const char* name,float f);

namespace ag {

const float maxAngleX = 85.0f;
const float maxAngleY = 180.0f;
const float maxAngleZ = 180.0f;

class BaseCamera: public iTransform, public iReadWriteXML {
public:
    void dump(const char* function);
    void setEye(const ag::vec3& iPoint);
    void setTarget(const ag::vec3& iPoint);

    void setAspectRatio(const int width, const int height);
    void setNearClipDistance(const float iNear);
    void setFarClipDistance(const float iFar);
    void setFov(const float iFov);

    inline float getAspectRatio() const       { return _aspectRatio; }
    inline float getNearClipDistance() const  { return _near; }
    inline float getFarClipDistance() const   { return _far; }
    inline float getFov() const               { return _fov; }

    const ag::vec3& getRight();
    inline const ag::vec3& getUp() const    { return _up; }
    inline const ag::vec3& getEye() const   { return _eye; }

    const ag::mat4& getProjection();
    const ag::mat4& getView();

    BaseCamera();
    ~BaseCamera() {}

    void SetScale(const float) override {}

    void setParam(BaseCamera &camera);
    ObjectType getType( void ) const { return _type; }

    void begin_apply(const unsigned int program_id);

    bool Read(TiXmlElement* camera) override;
    void Write(TiXmlElement* scene) const override;

private:
    // update are const in order to have const getter methods
    void updateProjection();
    void updateView();
    void updateRight();

protected:
    std::string _name;

    ag::vec3 _eye;      // The eye's position in 3d space
    ag::vec3 _target;   // What the eye is looking at
    ag::vec3 _up;       // The eye's orientation vector (which way is up)
    ag::vec3 _orientation; //for angle

    float _fov;
    float _aspectRatio;
    float _near;
    float _far;
    
    ObjectType _type;

    bool _projectionNeedsUpdate;
    bool _viewNeedsUpdate;
    bool _rightNeedsUpdate;

private:
    ag::vec3 _right;
    ag::mat4 _projection;
    ag::mat4 _view;
};

} //end namespace ag

#endif // _ORBIT_CAMERA_H
