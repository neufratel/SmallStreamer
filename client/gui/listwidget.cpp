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
ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent)
{
    this->setGeometry(0,0, 70, 120);
    this->setVisible(true);


    connect(this, SIGNAL ( itemDoubleClicked(QListWidgetItem*)), this, SLOT (doubleClicked()));
	
	timer= new QTimer(this);
    connect(timer, SIGNAL (timeout()), this, SLOT (selected()));
    timer->start(1000);
	
     
        setAlternatingRowColors(true);
 	    
		
};


void ListWidget::doubleClicked(){
		std::lock_guard<std::recursive_mutex> lock(mutex);

		qDebug()<<this->currentRow();
		Controler::getControl().setCurrentPlayListIndex(this->currentRow());
		qDebug()<<this->currentRow();

}

void ListWidget::selected(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	this->clear();
		for(int i=0; i<Controler::getControl().containerSize(); i++){
			this->insertItem(i,(Controler::getControl()).getPlayListName(i).c_str());
		}
		this->item(Controler::getControl().getCurrentPlayListIndex())->setBackgroundColor(Qt::red);
		this->setCurrentRow(Controler::getControl().getCurrentPlayListIndex());
}
int ListWidget::current(){
	return this->currentRow();
}
