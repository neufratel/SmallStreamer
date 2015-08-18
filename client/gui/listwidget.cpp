#include "listwidget.h"
#include <QAbstractItemModel>
#include <QStringList>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidgetItem>
#include <QDebug>
#include <QColor>
#include <QtGui>
#include <QDragEnterEvent>
#include "controler.h"
#include "playlistmanager.h"
ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent)
{
    this->setGeometry(0,0, 70, 120);
    this->setVisible(true);


    connect(this, SIGNAL ( itemDoubleClicked(QListWidgetItem*)), this, SLOT (doubleClicked()));
    connect(this, SIGNAL ( itemClicked(QListWidgetItem*)), this, SLOT (clicked()));

	timer= new QTimer(this);
    connect(timer, SIGNAL (timeout()), this, SLOT (selected()));
    timer->start(1000);
	
     
        setAlternatingRowColors(true);
 	    
		
};


void ListWidget::doubleClicked(){
		std::lock_guard<std::recursive_mutex> lock(mutex);

		qDebug()<<"Double Cliced: "<<this->currentRow();
		Controler::getControl().setCurrentPlayListIndex(this->currentRow());

}

void ListWidget::clicked(){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		((PlayListManager*)this->parentWidget())->selected_playlist=this->currentRow();
		qDebug()<<"Clicked: "<<this->currentRow();

}

void ListWidget::loadPlayLists(){
	std::lock_guard<std::recursive_mutex> lock(mutex);

	this->clear();
		for(int i=0; i<Controler::getControl().containerSize(); i++){
			this->insertItem(i,(Controler::getControl()).getPlayListName( i).c_str());
		}
	if(this->count()>0){
		this->item(Controler::getControl().getCurrentPlayListIndex())->setBackgroundColor(Qt::red);
	}
}

void ListWidget::selected(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(old_playlist_size!=Controler::getControl().containerSize()){
		old_playlist_size=Controler::getControl().containerSize();
		loadPlayLists();
	}
	if(this->count()>0){
		this->item(Controler::getControl().getCurrentPlayListIndex())->setBackgroundColor(Qt::red);
	}
//	this->clear();
		/*for(int i=0; i<Controler::getControl().containerSize(); i++){
			this->takeItem(i);
			this->insertItem(i,(Controler::getControl()).getPlayListName(i).c_str());
		}*/
	//	this->item(Controler::getControl().getCurrentPlayListIndex())->setBackgroundColor(Qt::red);
	//	this->setCurrentRow(Controler::getControl().getCurrentPlayListIndex());
}
int ListWidget::current(){
	return this->currentRow();
}
