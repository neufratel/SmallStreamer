#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), panel(this), list(this)
{
    this->setGeometry(50,50, 330, 500);
}

MainWindow::~MainWindow()
{


}

