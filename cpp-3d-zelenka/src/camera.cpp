#include "camera.h"
#include "gl_extentions.h"

#include "math/vector3.h"
#include "math/matrix4x4.h"

#include <fstream>
extern std::ofstream log_file;

using namespace ag;

void Camera::TranslateX(const float iDistance) {
     ag::vec3 Offset = getRight();
     Offset *= iDistance;

    _eye += Offset;  _target += Offset;

    _viewNeedsUpdate = true;
}

void Camera::TranslateY(const float iDistance) {
    ag::vec3 Offset = ag::vec3::normalize(_up) * iDistance;

    _eye += Offset;  _target += Offset;

    _viewNeedsUpdate = true;
}

void Camera::TranslateZ(const float iDistance) {
    ag::vec3 Offset = ag::vec3::normalize(_target-_eye) * iDistance;

    _target += Offset;  _eye += Offset;

    _viewNeedsUpdate = true;
}

// rotation local x (=тангаж)
void Camera::Pitch(const float iAngle) {
    _orientation.addX(iAngle);
   // if( !limitAngle(_orientation.x, -maxAngleX, maxAngleX) ) return;

    _target -= _eye;

    ag::mat4 Rotate = ag::mat4::rotation(ag::mat4(),iAngle,getRight());
    _up = Rotate * _up;
    _target = Rotate * _target;

    _target += _eye;

    _viewNeedsUpdate = true;
    _rightNeedsUpdate = true;
}

// rotation local y (=рыскание)
void Camera::Yaw(const float iAngle) {
    _orientation.setY(modulusAngle(_orientation.y() + iAngle));
    _target -= _eye;

    ag::mat4 Rotate = ag::mat4::rotation(ag::mat4(),iAngle,_up);
    _target = Rotate * _target;

    _target += _eye;

    _viewNeedsUpdate = true;
    _rightNeedsUpdate = true;
}

// rotation local z (=крен)
void Camera::Roll(const float iAngle) {
    _orientation.setZ(modulusAngle(_orientation.z() + iAngle));

    ag::vec3 direction = ag::vec3::normalize(_target-_eye);
    ag::mat4 Rotate = ag::mat4::rotation(ag::mat4(), iAngle, direction);
    _up = ag::vec3::normalize(Rotate * _up);

    _viewNeedsUpdate = true;
    _rightNeedsUpdate = true;
}
