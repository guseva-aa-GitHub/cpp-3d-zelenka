#pragma once

#include <QGLWidget>
#include <QtGui>

#include "scene3d.h"

class QtScene3D : public QGLWidget {
public:
    QtScene3D(QWidget* parent = 0);
    ~QtScene3D();

    bool SceneOpen(const std::string& file)  { return _scene.Open(file); }
    void SceneSave(const std::string& file)  { _scene.Save(file); }
    inline bool SceneReady() noexcept        { return _scene.Ready(); }

    const std::vector<std::unique_ptr<ag::BaseModel>>* GetModels() const noexcept { 
                return _scene.GetModels(); }
    const ag::vec3 GetPosCamera() const noexcept { return _scene.GetPosCamera(); }

    inline void SelectCamera() noexcept             { _scene.SelectCamera(); }
    inline void SelectModel(const int id) noexcept  { _scene.SelectModel(id); }
    inline bool IsTranformCamera() const noexcept   { return _scene.IsTranformCamera(); }

    void keyPressEvent(QKeyEvent* pe) override;

private:
    Scene3D _scene;
    QPoint _mousePosition;  
    
protected:
    void initializeGL() override;
    void resizeGL(int nWidth, int nHeight) override;
    void paintGL() override;

    // void mousePressEvent(QMouseEvent* pe) override;
    // void mouseMoveEvent(QMouseEvent* pe) override;
    // void mouseReleaseEvent(QMouseEvent* pe) override;
    // void wheelEvent(QWheelEvent* pe) override;   
};
