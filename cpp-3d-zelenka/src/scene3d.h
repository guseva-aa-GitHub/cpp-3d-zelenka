#pragma once

#include "shader.h"
#include "frame_buffer.h"
#include "light.h"
#include "orbit_camera.h"
#include "camera.h"

#include "model_assimp.h"
#include "model_simple.h"

#include <memory>
#include <vector>

class Scene3D {
public:
    Scene3D();
    ~Scene3D();

    void GLInitialize();
    inline bool Ready() const { return _ready; }

    void Resize(const int nWidth, const int nHeight);
    void Paint();

    bool Open(const std::string& file);
    void Save(const std::string& file) const;

    void SelectOrbitCamera();
    void SelectCamera();
    void SelectModel();
    void SelectLight();

    void SelectedScale(const float delta);
    void SelectedRoll(const float delta);
    void SelectedYaw(const float delta);
    void SelectedPitch(const float delta);
    void SelectedTranslateX(const float delta);
    void SelectedTranslateY(const float delta);
    void SelectedTranslateZ(const float delta);

    const std::vector<std::unique_ptr<ag::BaseModel>>* GetNameModels() const;
    const ag::vec3 GetPosCamera() const;

private:
    std::string _file;
    GLint _width, _height;
    bool _ready;

    ShaderProgram   _program;
    ShaderProgram   _program_d;

    ag::FrameBuffer _frame_buffer;

    ag::DirectLight _light;
    ag::OrbCamera   _orb_camera;
    ag::Camera      _camera;

    std::vector<std::unique_ptr<ag::BaseModel>> _models;

    ag::iTransform* _pTransform = nullptr;
    ag::BaseCamera* _pCamera = nullptr;

    void SceneInitialize();
    void Clear();

    void ReadCamera(TiXmlElement* camera);
};

