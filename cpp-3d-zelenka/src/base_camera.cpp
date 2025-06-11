#include "base_camera.h"
#include "gl_extentions.h"

#include <fstream>
extern std::ofstream log_file;

using namespace ag;

BaseCamera::BaseCamera()
: _eye(0.0f,0.0f,1.0f), _target(0.0f), _up(0.0f,1.0f,0.0f),
_fov(45.0f), _aspectRatio(4.0f/3.0f), _near(0.1f), _far(100.0f),
_type(AG_TYPE_BASE_CAMERA),
_projectionNeedsUpdate(true), _viewNeedsUpdate(true),
_rightNeedsUpdate(true) {
    updateProjection();
    updateView();
    updateRight();
}

void BaseCamera::setEye(const ag::vec3& iPoint) {
    _eye = iPoint;
    _viewNeedsUpdate = true;
    _rightNeedsUpdate =  true;
}

void BaseCamera::setTarget(const ag::vec3& iPoint) {
    _target = iPoint;
    _viewNeedsUpdate = true;
}

const ag::vec3& BaseCamera::getRight() {
    updateRight();
    return _right;
}

void BaseCamera::setAspectRatio(const int width, const int height) {
    assert(height);
    _aspectRatio = static_cast<float>(width)/static_cast<float>(height);
    _projectionNeedsUpdate = true;
}

void  BaseCamera::setNearClipDistance(const float iNear) {
    assert( iNear > 0.0f );
    _near = iNear;
    _projectionNeedsUpdate = true;
}

void  BaseCamera::setFarClipDistance(const float iFar) {
    assert( iFar > _near );
    _far = iFar;
    _projectionNeedsUpdate = true;
}

void BaseCamera::setFov(const float iFov) {
    assert( iFov > 5.0f && iFov < 90.0f);
    _fov = iFov;
    _projectionNeedsUpdate = true;
}

const ag::mat4& BaseCamera::getProjection() {
    updateProjection();
    return _projection;
}

const ag::mat4& BaseCamera::getView() {
    updateView();
    return _view;
}

void BaseCamera::updateProjection() {
    if(_projectionNeedsUpdate) {
        _projection = ag::mat4::perspective(_fov, _aspectRatio, _near, _far);
        _projectionNeedsUpdate = false;
    }
}

void BaseCamera::updateView() {
    if(_viewNeedsUpdate) {
        _view = ag::mat4::lookAt(_eye, _target, _up);
        _viewNeedsUpdate = false;
    }
}

void BaseCamera::updateRight() {
    if(_rightNeedsUpdate) {
        _right = ag::vec3::normalize(ag::vec3::cross(_target-_eye, _up));
        _rightNeedsUpdate = false;
    }
}

void BaseCamera::setParam(BaseCamera& camera) {
    if (_type == camera._type) return;

    _eye = camera._eye;
    _target = camera._target;
    _up = camera._up;
    _fov = camera._fov;
    _aspectRatio = camera._aspectRatio;
    _near = camera._near;
    _far = camera._far;
    _orientation = camera._orientation * -1.0f;

    _projectionNeedsUpdate = true;
    _viewNeedsUpdate = true;
    _rightNeedsUpdate = true;
}

void BaseCamera::begin_apply(const unsigned int program_id) {
    updateView();
    updateProjection();
    updateRight();

    // расчитаем необходимые матрицы
    ag::mat4 viewProjection = _projection * _view;

    int location = glGetUniformLocation(program_id, "transform.viewProjection");
    glUniformMatrix4fv(location, 1, GL_TRUE, viewProjection.ptr());
    // передаем позицию наблюдателя (камеры) в шейдерную программу
    location = glGetUniformLocation(program_id, "transform.viewPosition");
    glUniform3fv(location, 1, _eye.ptr());
}

bool BaseCamera::Read(TiXmlElement* camera) {
    if (!camera) return false;

    if (!ReadString(camera->FirstChildElement("name"),  _name))
        return false;
    if (!ReadVec3(camera->FirstChildElement("eye"),     _eye))
        return false;
    if (!ReadVec3(camera->FirstChildElement("target"),  _target))
        return false;
    if (!ReadVec3(camera->FirstChildElement("up"),      _up))
        return false;

    _projectionNeedsUpdate = true;
    _viewNeedsUpdate = true;
    _rightNeedsUpdate = true;
    return true;
}

void BaseCamera::Write(TiXmlElement* scene) const {
    if(!scene) return;

    TiXmlElement* camera = new TiXmlElement( "camera" );
    camera->SetAttribute( "type", _type);
    camera->SetAttribute( "type_name", AG_OBJECT_TYPE_NAME[_type]);

    TiXmlElement* name = new TiXmlElement( "name" );
    name->LinkEndChild( new TiXmlText( _name.c_str()) );
    camera->LinkEndChild(name);

    TiXmlElement* eye = new TiXmlElement( "eye" );
    eye->SetDoubleAttribute( "x", _eye.x());
    eye->SetDoubleAttribute( "y", _eye.y());
    eye->SetDoubleAttribute( "z", _eye.z());
    camera->LinkEndChild(eye);

    TiXmlElement* target = new TiXmlElement( "target" );
    target->SetDoubleAttribute( "x", _target.x());
    target->SetDoubleAttribute( "y", _target.y());
    target->SetDoubleAttribute( "z", _target.z());
    camera->LinkEndChild(target);

    TiXmlElement* up = new TiXmlElement( "up" );
    up->SetDoubleAttribute( "x", _up.x());
    up->SetDoubleAttribute( "y", _up.y());
    up->SetDoubleAttribute( "z", _up.z());
    camera->LinkEndChild(up);

    scene->LinkEndChild( camera );
}

//For debug
void BaseCamera::dump(const char* function) {
    log_file<<"\t * "<<function<<"\n";

    print_vec3("_eye \t",      _eye); 
    print_vec3("_target \t",   _target);
    print_vec3("_up \t",       _up);
    print_vec3("_right \t",    _right);
    print_float(" dist \t",    (_eye-_target).length());
//     print_float("_fov", _fov);
//     print_float("_aspectratio", _aspectRatio);
//     print_float("_near", _near);
//     print_float("_far", _far);
    log_file<<"--------------------------------------\n";
}

void print_vec3(const char* name, ag::vec3& v) {
    log_file<<name<<" "<<v.x()<<", "<<v.y()<<", "<<v.z()<<"\n";
}

void print_float(const char* name, float f) {
    log_file<<name<<" "<<f<<"\n";
}
