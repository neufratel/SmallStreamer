#include "clientcontrolpanel.h"
#include "clientcontroler.h"
#include <QDebug>


ClientControlPanel::ClientControlPanel(QWidget *parent) :
    QWidget(parent), list(this), button_add("+", this)
{
	this->show();
	 connect(&button_add, SIGNAL (released()), this, SLOT (add_server()));
	 connect(this, SIGNAL ( itemDoubleClicked(QListWidgetItem*)), this, SLOT (doubleClicked()));
}

void ClientControlPanel::resizeEvent(QResizeEvent* event){
	list.setGeometry(QRect(QPoint(5, 10), QSize(this->width()-10, this->height()-20)));
	button_add.setGeometry(QRect(QPoint(0, this->height())-B_ADD_POINT, B_ADD_SIZE));
}

void ClientControlPanel::add_server(){
	
	//ServerDescription server("localhost", "127.0.0.1", "5555");
	//server_list.push_back(server);
	
	
//ClientControler::getControl().setNewServer("localhost", "5555");
	//new QListWidgetItem(server.name.c_str(),&list);
		new ServerDescription("localhost", "127.0.0.1", "5555", &list);
	
}

void ClientControlPanel::doubleClicked(){
    qDebug()<<list.currentIndex()<<list.currentItem();

}
