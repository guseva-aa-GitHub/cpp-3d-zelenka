#include "orbit_camera.h"
#include "gl_extentions.h"

#include "math/vector3.h"

#include <fstream>
extern std::ofstream log_file;

using namespace ag;

// void OrbCamera::TranslateX(const float iDistance)
// {
//     /*target ??*/
//     //_eye += ( getRight() * iDistance);
//     _viewNeedsUpdate = true;
// }

// void OrbCamera::TranslateY(const float iDistance)
// {
//     /*target ??*/
// //    _eye += (getUp()*iDistance);
//     _viewNeedsUpdate = true;
// }

// void OrbCamera::TranslateZ(const float iDistance)
// {
// //    glm::vec3 vec = _eye + iDistance * glm::normalize(_target - _eye);
// //    if( vec.z > _target.z )
// //    {
// //        _eye = vec;  _viewNeedsUpdate = true;
// //    }
// }

// rotation local x (тангаж)
void OrbCamera::Pitch(const float iAngle) {
    _orientation.setX(modulusAngle(_orientation.x() + iAngle));
    Rotate(getRight(), iAngle);
}

// rotation local y (рыскание)
void OrbCamera::Yaw(const float iAngle) {
    _orientation.setY(modulusAngle(_orientation.y() + iAngle));
    Rotate(getUp(), iAngle);    
}

// rotation local z (крен)
void OrbCamera::Roll(const float iAngle) {
    _orientation.setZ(modulusAngle(_orientation.z() + iAngle));
    Rotate(_target - _eye, iAngle);
}

void OrbCamera::Rotate(const ag::vec3& iAxis, const float iAngle) {
    _eye = ag::mat4::rotation(ag::mat4(), iAngle, iAxis) * _eye;
    _up  = ag::mat4::rotation(ag::mat4(), iAngle, iAxis) * _up;
    _up.normalize();

    _viewNeedsUpdate = true;
    _rightNeedsUpdate = true;
}
