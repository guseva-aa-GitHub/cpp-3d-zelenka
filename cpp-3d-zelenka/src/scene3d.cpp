#include <fstream>
#include <tinyxml.h>

#include "gl_extentions.h"
#include "math/vector3.h"

#include "scene3d.h"
#include "object_type.h"
#include "frame_buffer_image.h"

extern std::ofstream log_file;

std::unique_ptr<ag::BaseModel> createModel(TiXmlElement* model) {
    int type = -1;

    if (model) model->QueryIntAttribute("type", &type);
    switch(type) {
        case AG_TYPE_ASSIMP_MODEL:
            return std::make_unique<ag::ModelAssimp>();
        break;
        case AG_TYPE_SIMPLE_MODEL:
            return std::make_unique<ag::ModelSimple>();
        break;
    }
    return nullptr;
}

Scene3D::Scene3D() 
: _width(0), _height(0), _ready(false) {
    _pTransform = &_camera;
    _pCamera = &_camera;
}

Scene3D::~Scene3D() {
    Clear();
}

void Scene3D::Clear() {
    _models.clear();
    _file.clear();
}

bool Scene3D::Open(const std::string& file) {
    TiXmlDocument doc;
    doc.LoadFile(file);
    if (doc.Error()) {
        log_file<<"Xml format, "<<doc.ErrorDesc()<<std::endl;
        return false;
    }

    Clear();
    _file = file;

    TiXmlNode* scene = doc.FirstChildElement("scene");
    if (!scene)  {
        log_file<<"Uncorrect format scene xml file, ";
        log_file<<"no tag <scene> "<<std::endl;
        return false;
    }

    if (!_light.Read(scene->FirstChildElement("light")))
        log_file<<"Uncorrect scene xml file: light.Read() = false"<<std::endl;

    ReadCamera(scene->FirstChildElement("camera"));

    TiXmlElement* model = scene->FirstChildElement("models")->FirstChildElement("model");
    while(model) {
        if (auto u_ptr = createModel(model); u_ptr->Read(model)) {
            _models.push_back(std::move(u_ptr));
        }

        model = model->NextSiblingElement("model");
    }

    SceneInitialize();
    return true;
}

void Scene3D::GLInitialize() {

    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
       log_file<<"Scene3D::GLInitialize(), error = "<<error<<std::endl;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);      // Разрешить тест глубины
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    ag::opengl_contex_info(log_file);
    bool result = ag::opengl_extension_init();

    if (result)  
        log_file<<"Init GL extension is SUCCESS\n\n";
    else {
        log_file<<"Init GL extension is ERROR\n\n";
        return;
    }

    try  {
        _program.   Init("Shaders",   "_basic",    log_file);
        _program_d. Init("Shaders",   "_depth",    log_file);

        _frame_buffer.init(1024, 1024);
    }
    catch(std::exception &e) {
        log_file<<e.what()<<", openGL init failed"<<std::endl;
    }

    _pTransform = &_camera;
    _pCamera = &_camera;
}

void Scene3D::SceneInitialize(){
    try {
        for(auto& model: _models) 
            model->Init();

        _ready = true;

        log_file<<"Scene Init true"<<std::endl;
    }
    catch(std::exception &e) {
        log_file<<e.what()<<", Scene Init failed"<<std::endl;
    }
}

void Scene3D::Resize(const int nWidth, const int nHeight) {
    _width = static_cast<GLint>(nWidth);
    _height = static_cast<GLint>(nHeight);

    glViewport(0, 0, _width, _height);

    _pCamera->setAspectRatio(_width, _height);
    _light._camera.Viewport(_width, _height);
}

void Scene3D::Paint() {
    _program_d.BeginApply();
    _frame_buffer.begin_apply( _program_d.Id() );
    _light._camera.LocationViewProject( _program_d.Id() );

    for(const auto& model: _models) 
        model->Draw(_program_d.Id());

    _frame_buffer.end_apply();
    _program_d.EndApply();

    // Очистка Экрана и Буфера Глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    _program.BeginApply();
    _frame_buffer.location( _program.Id() );
    _pCamera->begin_apply( _program.Id() );
    _light.BeginApply( _program.Id() );
    _light._camera.LocationBiasViewProject( _program.Id() );

    for(const auto& model: _models) 
        model->Draw(_program.Id());

    _program.EndApply();
}

void Scene3D::SelectCamera() {
    if (_pTransform == &_camera) return;
    _camera.setParam(*_pCamera);
    _pCamera = & _camera;
    _pTransform = & _camera;
}
void Scene3D::SelectOrbitCamera() {
    if (_pTransform == &_orb_camera) return;
    _orb_camera.setParam(*_pCamera);
    _pCamera = & _orb_camera;
    _pTransform = & _orb_camera;
}
void Scene3D::SelectLight() {
    if (_pTransform == &_light) return;
    _pTransform = &_light;
}

void Scene3D::SelectModel(const int id) {
    if (_models.empty() || id >= _models.size()) return;

    std::cout<<"Scene3D::SelectModel() id = "<<id<<std::endl;

    // std::cout<<"_pTransform = "<<_pTransform;
    auto ptr = _models.begin() +id;
    _pTransform = (*ptr).get();
    
    // _pTransform = _models.begin()->get();
    
    // std::cout<<"     = "<<_pTransform<<std::endl;
}

void Scene3D::SelectedScale(const float delta) {
    _pTransform->SetScale(delta);
}
void Scene3D::SelectedYaw(float delta) {
    _pTransform->Yaw(delta);
}
void Scene3D::SelectedPitch(float delta) {
    _pTransform->Pitch(delta);
}
void Scene3D::SelectedRoll(const float delta) {
    _pTransform->Roll(delta);
}
void Scene3D::SelectedTranslateX(const float delta) {
    _pTransform->TranslateX(delta);
}
void Scene3D::SelectedTranslateY(const float delta) {
    _pTransform->TranslateY(delta);
}
void Scene3D::SelectedTranslateZ(const float delta) {
    _pTransform->TranslateZ(delta);
}

const std::vector<std::unique_ptr<ag::BaseModel>>* Scene3D::GetModels() const {
    return &_models;
}

const ag::vec3 Scene3D::GetPosCamera() const {
    return _pCamera != nullptr ? _pCamera->getEye() : ag::vec3(0);
}

bool Scene3D::IsTranformCamera() const noexcept {
    return _pCamera == _pTransform;
}

void Scene3D::ReadCamera(TiXmlElement* xml_camera) {
    if (!xml_camera) return;

    int type = -1;
    if (xml_camera) xml_camera->QueryIntAttribute("type", &type);
    switch(type) {
        case AG_TYPE_CAMERA:
            if (!_camera.Read(xml_camera)) 
                log_file<<"Uncorrect scene xml file: camera.Read() = false"<<std::endl;
        break;
        case AG_TYPE_ORB_CAMERA:
            if (!_orb_camera.Read(xml_camera))
                log_file<<"Uncorrect scene xml file: orb_camera.Read() = false"<<std::endl;
        break;
    }
    ReadCamera(xml_camera->NextSiblingElement("camera"));
}

void Scene3D::Save(const std::string& file) const {
    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
    TiXmlElement *scene = new TiXmlElement( "scene" );

    doc.LinkEndChild(decl);
    doc.LinkEndChild(scene);

    _light.Write(scene);
    _camera.Write(scene);
    _orb_camera.Write(scene);

    TiXmlElement* models = new TiXmlElement( "models" );
    for (const auto& model : _models) {
        model->Write(models);
    }

    scene->LinkEndChild( models );
    if (file.empty())  doc.SaveFile(_file.c_str());
    else              doc.SaveFile(file.c_str());

}
