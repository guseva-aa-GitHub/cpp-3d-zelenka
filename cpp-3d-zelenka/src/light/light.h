#pragma once

#include <string>
#include <tinyxml.h>

#include "math/matrix4x4.h"
#include "math/vector3.h"
#include "math/other_vectors.h"

#include "object_type.h"
#include "itransform.h"

namespace ag {
    
class DirectLightCamera {
public:
    DirectLightCamera();

    inline void SetEye(ag::vec3& iPosition) { _eye = iPosition; _needUpdate = true; }
    
    void Viewport(const int width, const int height);
    void LocationViewProject(unsigned int program_id);
    void LocationBiasViewProject(unsigned int program_id);
    
private:    
    ag::mat4 _projection;
    ag::mat4 _view;
    ag::vec3 _eye;

    int _width, _height;
    bool _needUpdate;
};

class DirectLight: public iTransform, public iReadWriteXML {
public:
    DirectLight();
    DirectLightCamera _camera;

    void BeginApply(const unsigned int program_id) const;
	void SetPosition(float x, float y, float z);
	ObjectType GetType() const { return _type; }
    
	void TranslateX(const float iDistance) override;
    void TranslateY(const float iDistance) override;
    void TranslateZ(const float iDistance) override;

    void Pitch(const float iAngle) override;
    void Yaw(const float iAngle) override;
    void Roll(const float ) override {}
    void SetScale(const float ) override {}

    bool Read(TiXmlElement* light) override;
    void Write(TiXmlElement* scene) const override;

private:
    std::string _name;
	ObjectType _type;

    ag::vec3 _position;

    ag::color _ambient;
	ag::color _diffuse;
	ag::color _specular;
};

} //end namespace ag

