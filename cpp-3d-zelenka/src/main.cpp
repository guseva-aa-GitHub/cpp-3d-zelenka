#include <QApplication>

#include <fstream>
std::ofstream log_file("3Dzelenka.log");

#include "GUI/main_window.h"

int main(int argc, char** argv) {
    log_file.setf( std::ios::fixed );  
    log_file.precision(3); 
    
    QApplication app(argc, argv);
    setlocale(LC_NUMERIC, "C");

    MainWindow mainwindow; 
    mainwindow.resize(500, 500); 
    mainwindow.showMaximized(); 

    return app.exec();
}
