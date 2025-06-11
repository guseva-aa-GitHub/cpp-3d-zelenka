#ifndef _QT_SCENE3D_H
#define _QT_SCENE3D_H

#include <QGLWidget>
#include "../scene3d.h"

class QtScene3D : public QGLWidget {
public:
    QtScene3D(QWidget* parent = 0);
    ~QtScene3D();

    bool SceneOpen(const std::string& file)  { return _scene.Open(file); }
    void SceneSave(const std::string& file)  { _scene.Save(file); }
    inline bool SceneReady()                 { return _scene.Ready(); }

    const std::vector<std::unique_ptr<ag::BaseModel>>* GetNameModels() const { 
            return _scene.GetNameModels(); }
    const ag::vec3 GetPosCamera() const { return _scene.GetPosCamera(); }

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
    void keyPressEvent(QKeyEvent* pe) override;
};

#endif // _QT_SCENE3D_H
