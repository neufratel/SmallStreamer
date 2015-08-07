#include "clientcontrolpanel.h"
#include "clientcontroler.h"
#include <QDebug>


ClientControlPanel::ClientControlPanel(QWidget *parent) :
    QWidget(parent), list(this), button_add( this),button_remove(this), timer(this)
{
    this->setStyleSheet("border-image: url(gui/front.png) 5 5 5 5;");	
	this->show();

	button_add.setStyleSheet("border-image: url(gui/server_add.png) 1 1 1 1;");
	button_remove.setStyleSheet("border-image: url(gui/server_rm.png) 1 1 1 1;");

	connect(&button_add, SIGNAL (released()), this, SLOT (window_popup()));
	connect(&button_remove, SIGNAL (released()), this, SLOT (remove_server()));
	connect(&list, SIGNAL ( itemDoubleClicked(QListWidgetItem*)), this, SLOT (doubleClicked()));
   
	connect(&timer, SIGNAL (timeout()), this, SLOT (selected()));
    timer.start(1000);
	
}

void ClientControlPanel::resizeEvent(QResizeEvent* event){
	list.setGeometry(QRect(QPoint(4, 0), QSize(this->width(), this->height()-40)));
	button_add.setGeometry(QRect(QPoint(4, this->height())-B_ADD_POINT, B_ADD_SIZE));
	button_remove.setGeometry(QRect(QPoint(button_add.width(), this->height())-B_ADD_POINT, B_ADD_SIZE));
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
	ClientControler::getControl().addServerDescription(name, server, port);
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
	ClientControler::getControl().setCurrentServerIndex(list.currentRow());
	//ClientControler::getControl().setNewServer(((ServerDescription*)list.currentItem())->ip, ((ServerDescription*)list.currentItem())->port);
}

void ClientControlPanel::selected(){
	//std::lock_guard<std::recursive_mutex> lock(mutex);
	list.clear();
		for(int i=0; i<ClientControler::getControl().getServerListSize(); i++){
			new ServerDescriptionItem(ClientControler::getControl().getServerDescription(i),&list);
		}
			list.item(ClientControler::getControl().getCurrentServerIndex())->setBackgroundColor(Qt::red);
			list.setCurrentRow(ClientControler::getControl().getCurrentServerIndex());

}

void ClientControlPanel::remove_server(){
 	ClientControler::getControl().removeServerDescription(list.currentRow());
}
