#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), panel(this), c_panel(this), playlist_manager(this)
{
    this->setGeometry(50,50, 330, 500);
}

MainWindow::~MainWindow()
{


}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QWidget::resizeEvent(event);
   // Your code here.
	
	// panel->setGeometry(QRect(QPoint(this->width(), this->height())-VOLUME_POINT,VOLUME_SIZE));
	
	panel.setGeometry(QRect(QPoint(0,this->height())-QPoint( 0, panel.height()), QSize(this->width(), panel.height())));
	playlist_manager.setGeometry(QRect(QPoint(0,0), QSize(this->width()*0.7, this->height()-panel.height())));
	c_panel.setGeometry(QRect(QPoint(this->width()*0.7,0),QSize(this->width()*0.3,this->height()-panel.height())));
//	 panel.move(QPoint(this->width(), this->height())-CON_PAN_POINT);
	
}

