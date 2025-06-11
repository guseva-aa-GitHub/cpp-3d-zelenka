#include "main_window.h"

#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include <QAction>
#include <QMenuBar>
#include <iostream>

MainWindow::MainWindow() {
    QDir curret_dir = QDir::current();
    if ( !curret_dir.exists("Data") || !curret_dir.exists("Shaders")) {
        curret_dir.cdUp();
        if ( !curret_dir.exists("Data") || !curret_dir.exists("Shaders")) {
            std::cout<< "QDir::current() not exists (Data || Shaders) "<<std::endl;
        }
        else {
            QDir::setCurrent(curret_dir.path());
        }        
    }

    _scene = new QtScene3D; 
    setCentralWidget(_scene);

    this->setWindowTitle(tr("3D Zelenka")); 
    _scene->setFocus();

    CreateActions(); 
    CreateControlCenter();
    ControlCenterActions();
    CreateMenus(); 
}

void MainWindow::saveScene() {
    if(_scene->SceneReady()) {
        QMessageBox msgBox;
        msgBox.setText("The scene has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Yes)        {
            _scene->SceneSave("");
        }
    }
}

void MainWindow::openScene() {
    QString start_path = QDir::currentPath();
    QString file = QFileDialog::getOpenFileName(this, tr("Open scene"), start_path,"scene (*.xml)");
    if (file.isNull()) return;

    QFileInfo fi(file);
    if(_scene->SceneOpen(file.toUtf8().data())) {
        _scene->update();
        UpdateInfo();
    } else {
        QMessageBox::information(0, tr("Error!"),
                                 tr("    The file ( ") + file + tr( " )\n"
                                 " is incorrect. \n"
                                 "    Read me: 3Dzelenka.log " ) );
    }    
}

void MainWindow::exitFile() {
    close();
}

void MainWindow::saveSceneAs() {
   QString file = QFileDialog::getSaveFileName(
       this,
       tr("Save Scena"),
       QDir::currentPath(),
       tr("Scena (*.xml)") );

   if(file.isNull()) return;

   QFileInfo fi(file);
    _scene->SceneSave(file.toUtf8().data());
}

void MainWindow::CreateActions() {
   _openSceneAct = new QAction(tr("Open..."), this); 
   _saveSceneAsAct = new QAction(tr("Save As..."), this); 

   connect(_openSceneAct, SIGNAL(triggered()), this, SLOT(openScene()));
   connect(_saveSceneAsAct, SIGNAL(triggered()), this, SLOT(saveSceneAs()));

   _exitFileAct = new QAction(tr("Exit"), this); 
   connect(_exitFileAct, SIGNAL(triggered()), this, SLOT(exitFile()));

   _aboutHelpAct = new QAction(tr("About..."), this);
   connect(_aboutHelpAct, SIGNAL(triggered()), this, SLOT(aboutHelp()));
}

void MainWindow::CreateControlCenter() {
    _models_CB = new QComboBox();

    QDockWidget *dockWidget = new QDockWidget(tr("INFO"), this);
    QWidget *dockWidgetContents = new QWidget();
    QWidget *gridLayoutWidget = new QWidget(dockWidgetContents);

    QGridLayout *layout = new QGridLayout(gridLayoutWidget);

    layout->addWidget(new QLabel(tr(" camera ")), 0, 0);
    layout->addWidget(new QLabel(tr(" x = ")), 	1, 0);
	layout->addWidget(_pos_camera.xLE, 	1, 1);
    layout->addWidget(new QLabel(tr(" y = ")), 	2, 0);
	layout->addWidget(_pos_camera.yLE, 	2, 1);
    layout->addWidget(new QLabel(tr(" z = ")), 	3, 0);
	layout->addWidget(_pos_camera.zLE, 	3, 1);

    layout->addWidget(new QLabel(tr(" ")), 4, 0);

    layout->addWidget(new QLabel(tr(" models: ")), 5, 0);
    layout->addWidget(_models_CB, 5, 1);

    layout->addWidget(new QLabel(tr(" x = ")), 	6, 0);
	layout->addWidget(_pos_model.xLE, 	6, 1);
    layout->addWidget(new QLabel(tr(" y = ")), 	7, 0);
	layout->addWidget(_pos_model.yLE, 	7, 1);
    layout->addWidget(new QLabel(tr(" z = ")), 	8, 0);
	layout->addWidget(_pos_model.zLE, 	8, 1);

    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
    dockWidget->setWidget(dockWidgetContents);
    dockWidget->setMinimumSize(QSize(256, 512));
}

void MainWindow::UpdateInfo() {
    
    {   const auto camera_pos = _scene->GetPosCamera();
        _pos_camera.xLE->setText(std::to_string(camera_pos.x()).c_str());
        _pos_camera.yLE->setText(std::to_string(camera_pos.y()).c_str());
        _pos_camera.zLE->setText(std::to_string(camera_pos.z()).c_str());
    }

    _3d_models = _scene->GetNameModels();

    _models_CB->clear();
    for (const auto& model : *_3d_models) {
        _models_CB->addItem(model->GetFile().c_str());
    }    
    models_CBChange(0);
}

void MainWindow::ControlCenterActions() {
    connect(_models_CB, SIGNAL(currentIndexChanged(int)), this, SLOT(models_CBChange(int)));
}

void MainWindow::models_CBChange(int item) {
    if (_3d_models && item < _3d_models->size()) {
        const auto pos = _3d_models->at(item)->GetPosition();
        _pos_model.xLE->setText(std::to_string(pos.x()).c_str());
        _pos_model.yLE->setText(std::to_string(pos.y()).c_str());
        _pos_model.zLE->setText(std::to_string(pos.z()).c_str());
    }
}

void MainWindow::CreateMenus() {
    _file_menu = menuBar()->addMenu(tr("File")); 
    _file_menu->addAction(_openSceneAct); 
    _file_menu->addAction(_saveSceneAsAct); 
    _file_menu->addAction(_exitFileAct); 

    _help_menu = menuBar()->addMenu(tr("Help"));
    _help_menu->addAction(_aboutHelpAct);
}
