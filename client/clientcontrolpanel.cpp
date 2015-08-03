#include "clientcontrolpanel.h"
#include "clientcontroler.h"
#include <QDebug>


ClientControlPanel::ClientControlPanel(QWidget *parent) :
    QWidget(parent), list(this), button_add("+", this)
{
	this->show();
	connect(&button_add, SIGNAL (released()), this, SLOT (window_popup()));
	connect(&list, SIGNAL ( itemDoubleClicked(QListWidgetItem*)), this, SLOT (doubleClicked()));
}

void ClientControlPanel::resizeEvent(QResizeEvent* event){
	list.setGeometry(QRect(QPoint(0, 0), QSize(this->width(), this->height()-50)));
	button_add.setGeometry(QRect(QPoint(0, this->height())-B_ADD_POINT, B_ADD_SIZE));
}

void ClientControlPanel::window_popup(){
	
	popup=std::shared_ptr<QWidget>(new QWidget());
	popup->show();
	popup->setVisible(true);
	
	info =new QLabel("Addin new server", popup.get());
	text_name=new QPlainTextEdit("name", popup.get());
	text_ip=new QPlainTextEdit("ip", popup.get());
	text_port=new QPlainTextEdit("port", popup.get());
	button_ok=new QPushButton("Dodaj", popup.get());
	
	info->setFont(QFont( "lucida",12 , QFont::Bold, true ));
	text_name->setFont(QFont( "lucida",12 , QFont::Bold, true ));
	text_ip->setFont(QFont( "lucida",12 , QFont::Bold, true ));
	text_port->setFont(QFont( "lucida",12 , QFont::Bold, true ));
	
	info->setGeometry(QRect(QPoint(10, 5), QSize(170,30)));
	text_name->setGeometry(QRect(QPoint(5, 35), QSize(170,30)));
	text_ip->setGeometry(QRect(QPoint(5, 70), QSize(170,30)));
	text_port->setGeometry(QRect(QPoint(5, 105), QSize(170,30)));
	button_ok->setGeometry(QRect(QPoint(5, 140), QSize(170,30)));
	popup->setGeometry(QRect(QPoint(0,0),QSize(180, 180)));	

	info->setVisible(true);
	text_name->setVisible(true);
	text_ip->setVisible(true);
	text_port->setVisible(true);
	button_ok->setVisible(true);
	connect(button_ok, SIGNAL (released()), this, SLOT (add_server()));
	
}
void ClientControlPanel::add_server(){
	std::string name=text_name->toPlainText().toStdString()+": "+text_ip->toPlainText().toStdString()+":"+text_port->toPlainText().toStdString();
	std::string server=text_ip->toPlainText().toStdString();
	std::string port=text_port->toPlainText().toStdString();
	new ServerDescription(name, server, port, &list);
	qDebug()<<text_name->toPlainText();
	
	delete info;
	delete text_name;
	delete text_ip;
	delete text_port;
	delete button_ok;
	popup->close();
}
void ClientControlPanel::doubleClicked(){
    qDebug()<<list.currentIndex()<<list.currentItem()<<" "<<((ServerDescription*)list.currentItem())->name.c_str();
	ClientControler::getControl().setNewServer(((ServerDescription*)list.currentItem())->ip, ((ServerDescription*)list.currentItem())->port);
}
