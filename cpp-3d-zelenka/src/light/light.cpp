#include "gl_extentions.h"
#include "light.h"
#include <assert.h>

using namespace ag;

DirectLightCamera::DirectLightCamera()
: _eye(3.0f, 3.0f, 3.0f), _width(0.0), _height(0.0f),
_needUpdate(true) {}

void DirectLightCamera::Viewport(const int width, const int height) {
    _width = width; 
    _height = height; 
    _needUpdate = true; 
}

void DirectLightCamera::LocationViewProject(unsigned int program_id) {
    if (_needUpdate) {
        float left, right, bottom, top, Near, Far;
        float ratio = static_cast<float>(_height)/static_cast<float>(_width);
        float rib = 2.0f*_eye.length();

        Near = -rib;  Far = rib;

        if (_width >= _height) {
            left = -rib/ratio;
            right = rib/ratio;
            bottom = -rib;
            top = rib;
        } else {
            left = -rib;
            right = rib;
            bottom = -rib*ratio;
            top = rib*ratio;
        }

        _projection = ag::mat4::orthographic(left, right, bottom, top, Near, Far);
        _view = ag::mat4::lookAt(_eye, _eye*(-1.0f), ag::vec3(0.0f,1.0f,0.0f));
    }
    _needUpdate = false;

    ag::mat4 viewProjection = _projection * _view;
    int location = glGetUniformLocation(program_id, "transform.viewProjection");
    glUniformMatrix4fv(location, 1, GL_TRUE, viewProjection.ptr());
}

void DirectLightCamera::LocationBiasViewProject(unsigned int program_id) {
    ag::mat4 biasMatrix(
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, 0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.0f, 1.0f);

    ag::mat4 biasViewProjection = biasMatrix * _projection * _view;
    // передадим матрицу источника освещения в шейдерную программу
    int location = glGetUniformLocation(program_id, "transform.light");
    glUniformMatrix4fv(location, 1, GL_TRUE, biasViewProjection.ptr());
}

/**********************************************/

DirectLight::DirectLight() : _type(AG_TYPE_DIRECT_LIGHT) {
    // настроим направленный источник освещения
    _position.set(3.0f, 3.0f, 3.0f);
    _ambient.set(0.1f, 0.1f, 0.1f, 1.0f);
    _diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
    _specular.set(1.0f, 1.0f, 1.0f, 1.0f);
}

void DirectLight::BeginApply(const unsigned int program_id) const {
    glUniform4fv(glGetUniformLocation(program_id, "direct_light.position"), 1,    ag::vec4(_position).ptr());
    glUniform4fv(glGetUniformLocation(program_id, "direct_light.ambient"), 1,     _ambient.ptr());
    glUniform4fv(glGetUniformLocation(program_id, "direct_light.diffuse"), 1,     _diffuse.ptr());
    glUniform4fv(glGetUniformLocation(program_id, "direct_light.specular"), 1,    _specular.ptr());
}

void DirectLight::SetPosition(float x, float y, float z) {
    _position.set(x, y, z);
    _camera.SetEye(_position);
}

void DirectLight::TranslateX(const float iDistance) {
    _position = ag::mat4::translation(ag::mat4(),ag::vec3(iDistance,0.0f,0.0f))*_position;
    _camera.SetEye(_position);
}

void DirectLight::TranslateY(const float iDistance) {
    _position = ag::mat4::translation(ag::mat4(),ag::vec3(0.0f,iDistance,0.0f))*_position;
    _camera.SetEye(_position);
}

void DirectLight::TranslateZ(const float iDistance) {
    _position = ag::mat4::translation(ag::mat4(),ag::vec3(0.0f,0.0f,iDistance))*_position;
    _camera.SetEye(_position);
}

void DirectLight::Pitch(const float iAngle) {
    _position = ag::mat4::rotationX(ag::mat4(), iAngle) * _position;
    _camera.SetEye(_position);
}

void DirectLight::Yaw(const float iAngle) {
    _position = ag::mat4::rotationY(ag::mat4(), iAngle) * _position;
    _camera.SetEye(_position);
}

bool DirectLight::Read(TiXmlElement* light) {
    if (!light) return false;

    if (!ReadString(light->FirstChildElement("name"), _name))
        return false;

    if (!ReadVec3(light->FirstChildElement("position"),     _position))
        return false;
    
    if (!ReadColor(light->FirstChildElement("ambient"),     _ambient))
        return false;
    if (!ReadColor(light->FirstChildElement("diffuse"),     _diffuse))
        return false;
    if (!ReadColor(light->FirstChildElement("specular"),    _specular))
        return false;

    return true;
}

void DirectLight::Write(TiXmlElement* scene) const {
    if(!scene) return;

    TiXmlElement* light = new TiXmlElement( "light" );
    light->SetAttribute( "type", _type);
    light->SetAttribute( "type_name", AG_OBJECT_TYPE_NAME[_type]);

    TiXmlElement* name = new TiXmlElement( "name" );
    name->LinkEndChild( new TiXmlText( _name.c_str()) );
    light->LinkEndChild(name);

    TiXmlElement* position = new TiXmlElement( "position" );
    position->SetDoubleAttribute( "x", _position.x());
    position->SetDoubleAttribute( "y", _position.y());
    position->SetDoubleAttribute( "z", _position.z());
    light->LinkEndChild(position);

    TiXmlElement* ambient = new TiXmlElement( "ambient" );
    ambient->SetDoubleAttribute( "red", _ambient.r());
    ambient->SetDoubleAttribute( "green", _ambient.g());
    ambient->SetDoubleAttribute( "blue", _ambient.b());
    ambient->SetDoubleAttribute( "alpha", _ambient.a());
    light->LinkEndChild(ambient);

    TiXmlElement* diffuse = new TiXmlElement( "diffuse" );
    diffuse->SetDoubleAttribute( "red", _diffuse.r());
    diffuse->SetDoubleAttribute( "green", _diffuse.g());
    diffuse->SetDoubleAttribute( "blue", _diffuse.b());
    diffuse->SetDoubleAttribute( "alpha", _diffuse.a());
    light->LinkEndChild(diffuse);

    TiXmlElement* specular = new TiXmlElement( "specular" );
    specular->SetDoubleAttribute( "red", _specular.r());
    specular->SetDoubleAttribute( "green", _specular.g());
    specular->SetDoubleAttribute( "blue", _specular.b());
    specular->SetDoubleAttribute( "alpha", _specular.a());
    light->LinkEndChild(specular);

    scene->LinkEndChild( light );
}
