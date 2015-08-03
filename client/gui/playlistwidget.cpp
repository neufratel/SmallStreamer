#include "playlistwidget.h"
#include <QAbstractItemModel>
#include <QStringList>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidgetItem>
#include <QDebug>
#include <QtGui>
#include <QDragEnterEvent>
#include "controler.h"
PlaylistWidget::PlaylistWidget(QWidget *parent) :
    QListWidget(parent)
{
    this->setGeometry(30,30, 270, 120);
  //  this->setFixedHeight(300);
  //  this->setFixedWidth(270);
    this->setVisible(true);

 //   list = new QListWidget(this);
 //   list->setGeometry(10,10, 250, 280);
 //   list->setVisible(true);

 //   QWidgetItem* button_play=  new QWidget();
 //   button_play->setGeometry(30, 30, 50, 30);
   // connect(button_play, SIGNAL (released()), this, SLOT (play()));
   // button_play.setVisible(true);


    for(int i=0; i<(Controler::getControl()).size(); i++)
    this->insertItem(i,(Controler::getControl()).getAudioFileName(i).c_str());

    connect(this, SIGNAL ( itemDoubleClicked(QListWidgetItem*)), this, SLOT (doubleClicked()));
	
	timer= new QTimer(this);
    connect(timer, SIGNAL (timeout()), this, SLOT (selected()));
    timer->start(300);
	
		setAcceptDrops(true);
        setDragEnabled(true);
     
        setSelectionMode(QAbstractItemView::SingleSelection);
        setDropIndicatorShown(true);
        setDragDropMode(QAbstractItemView::InternalMove);
     
        setAlternatingRowColors(true);
     
        dropHintItem = new QListWidgetItem("Drop Files here...",this);

		
};


void PlaylistWidget::doubleClicked(){
    qDebug()<<this->currentIndex()<<this->currentItem();
 	Controler::getControl().setCurrentFileIndex(this->currentRow());
}

void PlaylistWidget::selected(){
	this->setCurrentRow(Controler::getControl().getCurrentFileIndex());
}


    void PlaylistWidget::dragEnterEvent(QDragEnterEvent *event)
    {
        if (event->mimeData()->hasUrls())
        {
            event->acceptProposedAction();
        } else {
            QListWidget::dragEnterEvent(event);
        }
    }
     
    void PlaylistWidget::dragMoveEvent(QDragMoveEvent *event)
    {
        if (event->mimeData()->hasUrls())
        {
            event->acceptProposedAction();
        } else {
            QListWidget::dragMoveEvent(event);
        }
    }
     
    void PlaylistWidget::dropEvent(QDropEvent *event)
    {
        if (event->mimeData()->hasUrls())
        {
            QList<QUrl> urls = event->mimeData()->urls();
            if (!urls.isEmpty())
            {
                if (dropHintItem)
                {
                    delete dropHintItem;
                    dropHintItem = 0;
                }
                QUrl url;
                foreach (url,urls)
                {
					std::string file_path=url.toString().toStdString();
					std::string path =file_path.substr(7, file_path.length()-6);
					try{
		                	Controler::getControl().addFile(path);
							
							new QListWidgetItem(Controler::getControl().getAudioFileName(Controler::getControl().size()-1).c_str(),this);
                
                
        		    }catch(std::string e)
            		{
            			std::cerr << e << std::endl;
            		}

                }
            }
            event->acceptProposedAction();
        }
        QListWidget::dropEvent(event);
    }





