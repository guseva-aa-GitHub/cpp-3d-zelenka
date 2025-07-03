#pragma once

#include <QMainWindow>
#include <QtGui>
#include <QDialog>

#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVector3D>

#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include <QAction>
#include <QMenuBar>

#include "Qt_scene3d.h"

class MainWindow : public QMainWindow {
   Q_OBJECT 

public:
    MainWindow(); 

private:
    QtScene3D* _scene; 
    const std::vector<std::unique_ptr<ag::BaseModel>>* _3d_models = nullptr;

    QMenu* _file_menu; 
    
    QAction* _openSceneAct; 
    QAction* _saveSceneAsAct; 
    QAction* _exitFileAct; 

    QMenu* _help_menu; 
    QAction* _aboutHelpAct; 

    struct Pos_LE {
        Pos_LE();
        QLineEdit *xLE;
        QLineEdit *yLE;
        QLineEdit *zLE;
        void Set(const ag::vec3& pos);
    };    

    QRadioButton* _selectCamera_RB;
    QRadioButton* _selectModels_RB;
    Pos_LE _pos_camera;
    Pos_LE _pos_model;
    QComboBox* _models_CB;

    void CreateActions(); 
    void CreateMenus(); 

    void CreateControlCenter();
    void ControlCenterActions();

    void UpdateInfo();
    void UpdateInfoKey();

private slots:
    void openScene();
    void saveSceneAs();
    void saveScene();
    void exitFile();
    void aboutHelp() { QDialog d; d.exec(); }
    
    void models_CBChange(int item);
    void camera_RBClicked();
    void model_RBClicked();

protected:
void keyPressEvent(QKeyEvent* pe) override;

};
