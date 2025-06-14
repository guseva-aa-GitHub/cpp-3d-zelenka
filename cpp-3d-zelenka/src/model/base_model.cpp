#include "math/matrix4x4.h"
#include "math/matrix3x3.h"

#include "base_model.h"
#include "material.h"
#include "mesh.h"

#include <filesystem>
#include <fstream>

extern std::ofstream log_file;
using namespace ag;

unsigned int BaseModel::_program_ref;

// void BaseModel::GetBoundingBox(ag::vec3 &min, ag::vec3 &max, 
//                                 ag::vec3 &center, float &scope) const {
//     if (!_ready) return;
//     min = _min;
//     max = _max;
//     center = _center;
//     scope = _scope;
// }

void BaseModel::SetCenterAndScope() {
    _center.set( (_min.x() + _max.x())/2.0f, 
                (_min.y() + _max.y())/2.0f, 
                (_min.z() + _max.z())/2.0f );

    _scope = _max.x() - _min.x();
    _scope = ag::math::maximum(_max.y() - _min.y(), _scope);
    _scope = ag::math::maximum(_max.z() - _min.z(), _scope);
    _scope = 2.0f/_scope;
}

// rotation local x (=тангаж)
void BaseModel::Pitch(const float iAngle) {
    _rotation.setX(modulusAngle(_rotation.x() + iAngle));
}
// rotation local y (=рыскание)
void BaseModel::Yaw(const float iAngle) {
    _rotation.setY(modulusAngle(_rotation.y() + iAngle));
}
// rotation local z (=крен)
void BaseModel::Roll(const float iAngle) {
    _rotation.setZ(modulusAngle(_rotation.z() + iAngle));
}

void BaseModel::set_uniform_location(const unsigned int program_id) const {
    ag::mat4 rot;
    rot = ag::mat4::rotationX(rot, _rotation.x());
    rot = ag::mat4::rotationY(rot, _rotation.y());
    rot = ag::mat4::rotationZ(rot, _rotation.z());

    ag::mat4 model = ag::mat4::translation(ag::mat4(),_position) *
                     ag::mat4::scaling(ag::mat4(),ag::vec3(_scale*_scope)) *
                     rot *
                     ag::mat4::translation(ag::mat4(),(_position + _center)*-1.0f);

    if (_program_ref != program_id) {
        _program_ref = program_id;
        ag::Material::Location(program_id);
        ag::Mesh::Location(program_id);
    }

    int location = glGetUniformLocation(program_id, "transform.model");
    glUniformMatrix4fv(location, 1, GL_TRUE, model.ptr());

    ag::mat3 normal(model);
    normal.inverse();
    normal.transpose();
    location = glGetUniformLocation(program_id, "transform.normal");
    glUniformMatrix3fv(location, 1, GL_TRUE, normal.ptr());
}

bool BaseModel::Read(TiXmlElement* model) {
    if (!model) return false;

    if (!readName(model))
        return false;

    if (!ReadVec3(model->FirstChildElement("position"), _position))
        return false;

    if (!ReadVec3(model->FirstChildElement("rotation"), _rotation))
        return false;    

    if (TiXmlElement* scale = model->FirstChildElement("scale"); scale)
        scale->QueryFloatAttribute("coef", &_scale);
    else return false;

    if (TiXmlElement* transp = model->FirstChildElement("transparency"); transp)
        transp->QueryBoolAttribute("yes", &_transparency);
    else return false;

    return true;
}

void BaseModel::Write(TiXmlElement* models) const {
    if(!models) return;

    TiXmlElement* model = new TiXmlElement( "model" );
    model->SetAttribute( "type", _type);
    model->SetAttribute( "type_name", AG_OBJECT_TYPE_NAME[_type]);

    TiXmlElement* name = new TiXmlElement( "name" );
    name->LinkEndChild( new TiXmlText(_name.c_str()) );
    model->LinkEndChild(name);

    TiXmlElement* path = new TiXmlElement( "path" );
    path->LinkEndChild(new TiXmlText(_path.c_str()));
    model->LinkEndChild(path);

    TiXmlElement* file = new TiXmlElement( "file" );
    file->LinkEndChild(new TiXmlText(_file.c_str()));
    model->LinkEndChild(file);

    TiXmlElement* position = new TiXmlElement( "position" );
    position->SetDoubleAttribute( "x", _position.x());
    position->SetDoubleAttribute( "y", _position.y());
    position->SetDoubleAttribute( "z", _position.z());
    model->LinkEndChild(position);

    TiXmlElement* rotation = new TiXmlElement( "rotation" );
    rotation->SetDoubleAttribute( "x", _rotation.x());
    rotation->SetDoubleAttribute( "y", _rotation.y());
    rotation->SetDoubleAttribute( "z", _rotation.z());
    model->LinkEndChild(rotation);

    TiXmlElement* scale = new TiXmlElement( "scale" );
    scale->SetDoubleAttribute( "coef", _scale);
    model->LinkEndChild(scale);

    TiXmlElement* transparency = new TiXmlElement( "transparency" );
    transparency->SetAttribute( "yes", _transparency);
    model->LinkEndChild(transparency);

    models->LinkEndChild(model);
}

bool BaseModel::readName(TiXmlElement* model) {
    if (   !ReadString(model->FirstChildElement("name"), _name)
        || !ReadString(model->FirstChildElement("path"), _path)
        || !ReadString(model->FirstChildElement("file"), _file))
        return false;

    // bool exist = std::filesystem::exists(filename);
    if (!std::filesystem::exists(_path + "/" + _file)) {
        if (!std::filesystem::exists("../" + _path + "/" + _file)) {
            return false;
        }
        else {
            _path = "../" + _path;
        }        
    }
    return true;
}
