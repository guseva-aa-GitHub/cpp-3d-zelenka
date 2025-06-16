#pragma once

#include <string>
#include <list>

#include <tinyxml.h>

#include "gl_extentions.h"
#include "imodel.h"
#include "itransform.h"
#include "object_type.h"

namespace ag {

class BaseModel : public iModel, public iTransform, public iReadWriteXML {
protected:
    static unsigned int _program_ref;
    ag::vec3 _position, _rotation;

    ag::vec3 _min, _max, _center;

    float _scope, _scale;  
    std::string _name, _program_name, _path, _file;

    bool _ready, _transparency;
    ObjectType _type;

    void SetCenterAndScope() override;
    void set_uniform_location(const unsigned int program_id) const;

    bool readName(TiXmlElement* model);

public:
    BaseModel()
    : _scope(1.0f), _scale(1.0f), _ready(false), _transparency(false),
    _type(AG_TYPE_BASE_MODEL) {}

    virtual ~BaseModel() {}

    inline void set_model_file(const char* path, const char* file) { _path = path; _file = file; }
    inline void set_model_name(const char* name)        { _name = name; }
    inline void set_model_program(const char* program)  { _program_name = program; }

    inline bool IsReady() const         { return _ready; }
    inline ObjectType GetType() const   { return _type; }
    inline const std::string& GetFile() const { return _file; }
    inline const ag::vec3& GetPosition() const { return _position; }

    inline void TranslateX(const float iDistance) override { _position.addX(iDistance); }
    inline void TranslateY(const float iDistance) override { _position.addY(iDistance); }
    inline void TranslateZ(const float iDistance) override { _position.addZ(iDistance); }

    void Pitch(const float iAngle) override;
    void Yaw(const float iAngle) override;
    void Roll(const float iAngle) override;

    inline void SetScale(const float iScale) override { _scale *= iScale; }

    inline void setPosition(ag::vec3 iPosition)    { _position = iPosition; }
    inline void setRotation(ag::vec3 iRotation)    { _rotation = iRotation; }

    bool Read(TiXmlElement* model) override;
    void Write(TiXmlElement* models) const override;
};

} 
