#ifndef _ORBIT_CAMERA_H
#define _ORBIT_CAMERA_H

#include "base_camera.h"

namespace ag
{

class OrbCamera: public BaseCamera {
public:
    OrbCamera() { _type = AG_TYPE_ORB_CAMERA; }
    ~OrbCamera() {}

    void TranslateX(const float ) override {}
    void TranslateY(const float ) override {}
    void TranslateZ(const float ) override {}

    void Pitch(const float iAngle) override;
    void Yaw(const float iAngle) override;
    void Roll(const float iAngle) override;

private:
    void Rotate(const ag::vec3& iAxis, const float iAngle);
};

} //end namespace ag

#endif // _ORBIT_CAMERA_H
