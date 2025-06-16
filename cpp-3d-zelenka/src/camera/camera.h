#pragma once
#include "base_camera.h"

namespace ag {

class Camera : public BaseCamera {
public:
    Camera() { _type = AG_TYPE_CAMERA; }
    ~Camera(){}

    void TranslateX(const float iDistance) override;
    void TranslateY(const float iDistance) override;
    void TranslateZ(const float iDistance) override;

    void Pitch(const float iAngle) override;
    void Yaw(const float iAngle) override;
    void Roll(const float iAngle) override;
};

} //end namespace ag
