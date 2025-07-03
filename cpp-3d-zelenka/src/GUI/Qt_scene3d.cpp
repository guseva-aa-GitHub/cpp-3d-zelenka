#include "Qt_scene3d.h"

QtScene3D::QtScene3D(QWidget *parent) 
: QGLWidget(parent){ }

QtScene3D::~QtScene3D() { }

void QtScene3D::initializeGL() {
    _scene.GLInitialize();
}

void QtScene3D::resizeGL(int nWidth, int nHeight) {
    _scene.Resize(nWidth,nHeight);
}

void QtScene3D::paintGL() {
    _scene.Paint();
}

// void QtScene3D::mousePressEvent(QMouseEvent *pe) {
//     _mousePosition = pe->pos();
// }

void QtScene3D::keyPressEvent(QKeyEvent *pe) {
    switch (pe->key())
    {
    case Qt::Key_O:
        _scene.SelectOrbitCamera();
        break;
    case Qt::Key_P:
        _scene.SelectCamera();
        break;
    case Qt::Key_M:
        _scene.SelectModel();
        break;
    case Qt::Key_L:
        _scene.SelectLight();
        break;

    case Qt::Key_Plus:
        _scene.SelectedScale(1.1f);
        break;
    case Qt::Key_Minus:
        _scene.SelectedScale(1.0f/1.1f);
        break;

    case Qt::Key_PageUp:
        _scene.SelectedRoll(-1.0f);
        break;
    case Qt::Key_PageDown:
        _scene.SelectedRoll(1.0f);
        break;

    case Qt::Key_Up:
        _scene.SelectedPitch(1.0f);
        break;
    case Qt::Key_Down:
        _scene.SelectedPitch(-1.0f);
        break;

    case Qt::Key_Left:
        _scene.SelectedYaw(1.0f);
        break;
    case Qt::Key_Right:
        _scene.SelectedYaw(-1.0f);
        break;

    case Qt::Key_S:
        _scene.SelectedTranslateY(-0.05f);
        break;
    case Qt::Key_W:
        _scene.SelectedTranslateY(0.05f);
        break;

    case Qt::Key_A:
        _scene.SelectedTranslateX(-0.05f);
        break;
    case Qt::Key_D:
        _scene.SelectedTranslateX(0.05f);
        break;

    case Qt::Key_Q:
        _scene.SelectedTranslateZ(0.05f);
        break;
    case Qt::Key_Z:
        _scene.SelectedTranslateZ(-0.05f);
        break;

    case Qt::Key_Escape:
        this->close();
        break;
    }

    updateGL();
}
