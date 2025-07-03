#include "main_window.h"

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

    _scene = new QtScene3D(); 
    setCentralWidget(_scene);

    this->setWindowTitle(tr("3D Zelenka")); 
    this->setFocus();

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

    _selectCamera_RB = new QRadioButton("Select camera", this);
    _selectCamera_RB->setChecked(true);
    _selectModels_RB = new QRadioButton("Select models", this);

    QDockWidget *dockWidget = new QDockWidget(tr("INFO"), this);
    QWidget *dockWidgetContents = new QWidget();
    QWidget *gridLayoutWidget = new QWidget(dockWidgetContents);
    

    QGridLayout *layout = new QGridLayout(gridLayoutWidget);

    layout->addWidget(_selectCamera_RB, 0, 0);
    layout->addWidget(new QLabel(tr(" x = ")), 	1, 0);
	layout->addWidget(_pos_camera.xLE, 	1, 1);
    layout->addWidget(new QLabel(tr(" y = ")), 	2, 0);
	layout->addWidget(_pos_camera.yLE, 	2, 1);
    layout->addWidget(new QLabel(tr(" z = ")), 	3, 0);
	layout->addWidget(_pos_camera.zLE, 	3, 1);

    layout->addWidget(new QLabel(tr(" ")), 4, 0);

    layout->addWidget(_selectModels_RB, 5, 0);
    layout->addWidget(_models_CB, 6, 0);

    layout->addWidget(new QLabel(tr(" x = ")), 	7, 0);
	layout->addWidget(_pos_model.xLE, 	7, 1);
    layout->addWidget(new QLabel(tr(" y = ")), 	8, 0);
	layout->addWidget(_pos_model.yLE, 	8, 1);
    layout->addWidget(new QLabel(tr(" z = ")), 	9, 0);
	layout->addWidget(_pos_model.zLE, 	9, 1);
    

    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
    dockWidget->setWidget(dockWidgetContents);
    dockWidget->setMinimumSize(QSize(256, 512));
}

void MainWindow::UpdateInfo() {
    _pos_camera.Set(_scene->GetPosCamera());

    _3d_models = _scene->GetModels();

    _models_CB->clear();
    for (const auto& model : *_3d_models) {
        _models_CB->addItem(model->GetFile().c_str());
    }    
    models_CBChange(0);
}

void MainWindow::UpdateInfoKey() {
    if (_scene->IsTranformCamera()) {
        _pos_camera.Set(_scene->GetPosCamera());
    } else {
        if (!_3d_models && _3d_models->empty())
        return;

        int item = _models_CB->currentIndex();
        _pos_model.Set(_3d_models->at(item)->GetPosition());
    }
}

void MainWindow::ControlCenterActions() {
    connect(_models_CB, SIGNAL(currentIndexChanged(int)), this, SLOT(models_CBChange(int)));
    connect(_selectCamera_RB, SIGNAL(clicked()), this, SLOT(camera_RBClicked()));
    connect(_selectModels_RB, SIGNAL(clicked()), this, SLOT(model_RBClicked()));
}

void MainWindow::models_CBChange(int item) {
    if (_3d_models && item < _3d_models->size()) {
        _pos_model.Set(_3d_models->at(item)->GetPosition());
    }
}

void MainWindow::camera_RBClicked() {
    _scene->SelectCamera();
}

void MainWindow::model_RBClicked() {
    _scene->SelectModel(_models_CB->currentIndex());
}

void MainWindow::CreateMenus() {
    _file_menu = menuBar()->addMenu(tr("File")); 
    _file_menu->addAction(_openSceneAct); 
    _file_menu->addAction(_saveSceneAsAct); 
    _file_menu->addAction(_exitFileAct); 

    _help_menu = menuBar()->addMenu(tr("Help"));
    _help_menu->addAction(_aboutHelpAct);
}

void MainWindow::keyPressEvent(QKeyEvent* pe) {
    switch (pe->key())
    {
    case Qt::Key_O:
        _selectCamera_RB->setChecked(true);
        break;
    case Qt::Key_P:
        _selectCamera_RB->setChecked(true);
        break;
    case Qt::Key_M:
        _selectModels_RB->setChecked(true);
        break;
    }
    _scene->keyPressEvent(pe);
    UpdateInfoKey();
}

MainWindow::Pos_LE::Pos_LE()
: xLE(new QLineEdit("0.0")), yLE(new QLineEdit("0.0")), zLE(new QLineEdit("0.0")) { }

void MainWindow::Pos_LE::Set(const ag::vec3& pos) {
    xLE->setText(std::to_string(pos.x()).c_str());
    yLE->setText(std::to_string(pos.y()).c_str());
    zLE->setText(std::to_string(pos.z()).c_str());
}
