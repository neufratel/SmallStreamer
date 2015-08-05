
#include "playlistmanager.h"
#include <QDebug>
#include "controler.h"
#include "stream.h"
#include <QPoint>
#include <QRect>
#include <QFont>
#include "playlist.h"

PlayListManager::PlayListManager(QWidget *parent) :
    QWidget(parent), list(this), button_add("+",this), button_remove("-", this),
	button_remove_file("-",this), playlist(this)
{
	this->setVisible(true);
	connect(&button_add, SIGNAL (released()), this, SLOT (add_playlist()));
	connect(&button_remove, SIGNAL (released()), this, SLOT(remove_playlist()));
	connect(&button_remove_file, SIGNAL (released()), this, SLOT(remove_file()));
}

void PlayListManager::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	list.setGeometry(QRect(QPoint(0,0), QSize(80, this->height()-40)));
	button_add.setGeometry(QRect(QPoint(0,this->height()-40),QSize(40,40)));
	button_remove.setGeometry(QRect(QPoint(40,this->height()-40),QSize(40,40)));
	playlist.setGeometry(QRect(QPoint(82, 0),QSize(this->width()-82, this->height()-40)));
	button_remove_file.setGeometry(QRect(QPoint(82,playlist.height()),QSize(40,40)));

}

void PlayListManager::remove_playlist(){
		std::cerr<<"To remove:"<<list.current()<<std::endl;
		Controler::getControl().removePlayList(list.currentRow());	
}

void PlayListManager::add_playlist(){
	popup=std::shared_ptr<QWidget>(new QWidget());
	popup->show();
	popup->setVisible(true);
	
	info =new QLabel("Add new Playlist", popup.get());
	playlist_name=new QPlainTextEdit("name", popup.get());
	button_ok=new QPushButton("Dodaj", popup.get());
	info->setFont(QFont( "lucida",12 , QFont::Bold, true ));
	
	info->setGeometry(QRect(QPoint(10, 5), QSize(170,30)));
	playlist_name->setGeometry(QRect(QPoint(5, 35), QSize(170,30)));
	button_ok->setGeometry(QRect(QPoint(5, 140), QSize(170,30)));
	popup->setGeometry(QRect(QPoint(0,0),QSize(180, 180)));	

	info->setVisible(true);
	playlist_name->setVisible(true);
	button_ok->setVisible(true);
	connect(button_ok, SIGNAL (released()), this, SLOT (create_playlist()));
}

void PlayListManager::create_playlist(){
	std::string name=playlist_name->toPlainText().toStdString();
		Controler::getControl().setPlayList(new PlayList(name));
	delete info;
	delete playlist_name;
	delete button_ok;
	popup->close();
}

void PlayListManager::remove_file(){
	Controler::getControl().removeAudioFile(list.currentRow());
}

