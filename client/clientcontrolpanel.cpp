#include "clientcontrolpanel.h"
#include "clientcontroler.h"
#include <QDebug>


ClientControlPanel::ClientControlPanel(QWidget *parent) :
    QWidget(parent), list(this), button_add("+", this), text_name("name", this),
	text_ip("ip", this), text_port("port", this)
{
	this->show();
	 connect(&button_add, SIGNAL (released()), this, SLOT (add_server()));
	 connect(&list, SIGNAL ( itemDoubleClicked(QListWidgetItem*)), this, SLOT (doubleClicked()));
}

void ClientControlPanel::resizeEvent(QResizeEvent* event){
	list.setGeometry(QRect(QPoint(5, 10), QSize(this->width()-10, this->height()-50)));
	button_add.setGeometry(QRect(QPoint(5, this->height())-B_ADD_POINT, B_ADD_SIZE));
	text_name.setGeometry(QRect(QPoint(50, this->height()-35), QSize(100,30)));
	text_ip.setGeometry(QRect(QPoint(160, this->height()-35), QSize(80,30)));
	text_port.setGeometry(QRect(QPoint(250, this->height()-35), QSize(80,30)));

}

void ClientControlPanel::add_server(){
	
	//ServerDescription server("localhost", "127.0.0.1", "5555");
	//server_list.push_back(server);
	
	
//ClientControler::getControl().setNewServer("localhost", "5555");
	//new QListWidgetItem(server.name.c_str(),&list);
	std::string name=text_name.toPlainText().toStdString()+": "+text_ip.toPlainText().toStdString()+":"+text_port.toPlainText().toStdString();
	std::string server=text_ip.toPlainText().toStdString();
	std::string port=text_port.toPlainText().toStdString();
	new ServerDescription(name, server, port, &list);
	qDebug()<<text_name.toPlainText();
}

void ClientControlPanel::doubleClicked(){
    qDebug()<<list.currentIndex()<<list.currentItem()<<" "<<((ServerDescription*)list.currentItem())->name.c_str();
	ClientControler::getControl().setNewServer(((ServerDescription*)list.currentItem())->ip, ((ServerDescription*)list.currentItem())->port);

}
