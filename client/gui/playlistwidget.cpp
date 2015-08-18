#include "playlistwidget.h"
#include <QAbstractItemModel>
#include <QStringList>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidgetItem>
#include <QDebug>
#include <QtGui>
#include <QDragEnterEvent>
#include "controler.h"
#include "playlistmanager.h"
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
    connect(this, SIGNAL ( itemClicked(QListWidgetItem*)), this, SLOT (clicked()));
	
	timer= new QTimer(this);
    connect(&(((PlayListManager*)this->parentWidget())->list), SIGNAL( itemClicked(QListWidgetItem*)) , this, SLOT (selected()));
    timer->start(1000);
	
		setAcceptDrops(true);
        setDragEnabled(true);
     
        setSelectionMode(QAbstractItemView::SingleSelection);
        setDropIndicatorShown(true);
        setDragDropMode(QAbstractItemView::InternalMove);
     
        setAlternatingRowColors(true);
     
};


void PlaylistWidget::doubleClicked(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	
    qDebug()<<this->currentIndex()<<this->currentItem();
 	Controler::getControl().setCurrentFileIndex(this->currentRow());
}

void PlaylistWidget::clicked(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	((PlayListManager*)this->parentWidget())->selected_song=this->currentRow();
}

void PlaylistWidget::loadPlaylist(unsigned int idx){
	std::lock_guard<std::recursive_mutex> lock(mutex);

	this->clear();
	if(Controler::getControl().getPlayListSize(idx)>0)
		for(int i=0; i<Controler::getControl().getPlayListSize(idx); i++){
			this->insertItem(i,(Controler::getControl()).getAudioFileName(idx, i).c_str());
		}
	if(this->count()>0){
		this->setCurrentRow(Controler::getControl().getCurrentFileIndex());
	}
}

void PlaylistWidget::selected(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(((PlayListManager*)this->parentWidget())->selected_playlist!=old_playlist_index){
		old_playlist_index=((PlayListManager*)this->parentWidget())->selected_playlist;
		loadPlaylist(((PlayListManager*)this->parentWidget())->selected_playlist);
	}
	if(old_playlist_index==Controler::getControl().getCurrentPlayListIndex() && Controler::getControl().getPlayListSize()>0){
		this->item(Controler::getControl().getCurrentFileIndex())->setBackgroundColor(Qt::red);
		
	}
	/*while(this->count()>0)
		this->takeItem(0);
		for(int i=0; i<Controler::getControl().size(); i++){
			this->insertItem(i,(Controler::getControl()).getAudioFileName(i).c_str());
		}
	if(this->count()>0){
		this->setCurrentRow(Controler::getControl().getCurrentFileIndex());
	}*/
}


    void PlaylistWidget::dragEnterEvent(QDragEnterEvent *event)
    {
			std::lock_guard<std::recursive_mutex> lock(mutex);

        
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
                QUrl url;
                foreach (url,urls)
                {
					std::string file_path=url.toString().toStdString();
					std::string path =file_path.substr(7, file_path.length()-6);
					try{
						std::lock_guard<std::recursive_mutex> lock(mutex);

	                	Controler::getControl().addFile(((PlayListManager*)this->parentWidget())->selected_playlist, path);
							
	//						new QListWidgetItem(Controler::getControl().getAudioFileName(Controler::getControl().size()-1).c_str(),this);
                
                
        		    }catch(std::string e)
            		{
            			std::cerr << e << std::endl;
            		}

                }
		loadPlaylist(old_playlist_index);
            }
            event->acceptProposedAction();
        }
        QListWidget::dropEvent(event);
    }





