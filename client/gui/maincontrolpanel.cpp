#include "maincontrolpanel.h"
#include <QDebug>
#include "controler.h"
#include "stream.h"
#include <QPoint>
#include <QRect>
#include <QFont>
#include "button.h"

MainControlPanel::MainControlPanel(QWidget *parent) :
    QWidget(parent), playing(false), progress_value(0), button_play(new Button(this)),
	progress(new QSlider(Qt::Horizontal,this)), 
	volume(new QSlider(Qt::Horizontal,this)),
	song_time(new QLabel("00:00/00:00", this)),
	song_name(new QLabel("Waiting...", this))

{
   this->setGeometry(QRect(CONTROL_POINT,CONTROL_SIZE));
   this->setFixedHeight(120);
 //  this->setFixedWidth(270);
   this->setVisible(true);

//    button_play=new QPushButton("P", this);
	this->setStyleSheet("border-image: url(gui/clean.png) 0 0 0 0;");
    QPoint point(30,30);

  
    button_play->setGeometry(QRect(B_PLAY_POINT, B_PLAY_SIZE));
    connect(button_play, SIGNAL (released()), this, SLOT (play()));
    button_play->setVisible(true);
	button_play_timer= new QTimer(this);
    connect(button_play_timer, SIGNAL (timeout()), this, SLOT (refreshButtonPlay()));
    button_play_timer->start(300);

    button_stop=new QPushButton( this);
    button_stop->setGeometry(QRect(B_STOP_POINT, B_STOP_SIZE));
    //connect(button_next, SIGNAL (released()), this, SLOT (next()));
    button_stop->setVisible(true);


    button_next=new QPushButton(this);
    button_next->setGeometry(QRect(B_NEXT_POINT, B_NEXT_SIZE));
    connect(button_next, SIGNAL (released()), this, SLOT (next()));
    button_next->setVisible(true);

    button_pierv=new QPushButton( this);
    button_pierv->setGeometry(QRect(B_PREV_POINT, B_PREV_SIZE));
    connect(button_pierv, SIGNAL (released()), this, SLOT (pierv()));
    button_pierv->setVisible(true);

   
    progress->setVisible(true);
	progress->setStyleSheet("QSlider::handle {image: url(gui/handle.png);};");
    connect(progress, SIGNAL (sliderMoved(int)), this, SLOT (changeProgress()));
    a=0;
    timer= new QTimer(this);
    connect(timer, SIGNAL (timeout()), this, SLOT (setProgress()));
    timer->start(100);

  
 //   volume->setGeometry(230, 30, 70, 20);
    connect(volume, SIGNAL (valueChanged(int)), this, SLOT (setVolume()));
    volume->show();
	
	auto_play_box = new QCheckBox(this);
	auto_play_box->setStyleSheet("QCheckBox::indicator:unchecked {image: url(gui/check_p.png) 5 5 5 5;} QCheckBox::indicator:checked {image: url(gui/check_s.png) 5 5 5 5;}");
	auto_play_box->setGeometry(10, 10, 30, 30);
	auto_play_box->setText("AutoPlay");
	auto_play_box->setVisible(true);
	
	
	connect(auto_play_box, SIGNAL (stateChanged(int)), this, SLOT(autoplay()));
	
	song_name->setFont(QFont( "lucida", 12, QFont::Bold, true ));
	//song_name->setGeometry(QRect(NAME_POINT, NAME_SIZE));
	song_name->setVisible(true);
	
	//song_time = new QLabel("00:00/00:00", this);
	song_time->setFont(QFont( "lucida", 12, QFont::Bold, true ));
	//song_time->setGeometry(QRect(NAME_POINT, NAME_SIZE));
	song_time->setVisible(true);
	button_next->setStyleSheet("border-image: url(gui/next.png) 0 0 0 0;");
	button_pierv->setStyleSheet("border-image: url(gui/prev.png) 0 0 0 0;");
	button_stop->setStyleSheet("border-image: url(gui/stop.png) 0 0 0 0;");
}

void MainControlPanel::autoplay(){
		
	Controler::getControl().setAutoPlay(auto_play_box->isChecked());
}

void MainControlPanel::play(){
	if(playing==true){
		qDebug()<<"Stoping...";
		Controler::getControl().stop();
		button_play->setStyleSheet("border-image: url(gui/play.png) 0 0 0 0;");
		//button_play->setText("P");
		playing=false;
	}else{
		qDebug()<<"Playing...";
	 	Controler::getControl().play();
		button_play->setStyleSheet("border-image: url(gui/pause.png) 0 0 0 0;");
		//button_play->setText("S");
		playing=true;
	}
}

void MainControlPanel::next(){
    qDebug()<<"Next";
	Controler::getControl().nextFile();
}

void MainControlPanel::pierv(){
    qDebug()<<"Pierv";
	Controler::getControl().prevFile();
}

void MainControlPanel::setProgress(){
	if(Controler::getControl().size()){
		song_name->setText(Controler::getControl().getAudioFileName(Controler::getControl().getCurrentFileIndex()).c_str());
		song_time->setText((Controler::getControl().getCurrentPositionTime()+"/"+Controler::getControl().getCurrentFileTime()).c_str());
	}
	if(playing){
		progress->setMaximum(Controler::getControl().getAudioSize(Controler::getControl().getCurrentFileIndex()));
		try{
			progress_value=Controler::getControl().getCurrentSampleIndex();
	    }catch(std::logic_error e) {

    		std::cerr <<"MainControlPanel"<< e.what();
			progress_value=0;
		}
	}
	
	progress->setValue((int)progress_value);
}
void MainControlPanel::setVolume(){
     Stream::setSampleClassVolume(volume->value());
}


void MainControlPanel::refreshButtonPlay(){
	playing=Controler::getControl().isPlaying();
	if(playing){
	//	button_play->setText("S");
	}else{
	//	button_play->setText("P");
	}
}

void MainControlPanel::resizeEvent(QResizeEvent* event)
{
   QWidget::resizeEvent(event);
   // Your code here.
	
	 volume->setGeometry(QRect(QPoint(this->width(), this->height())-VOLUME_POINT,VOLUME_SIZE));
	progress->setGeometry(QRect(PROG_BAR_POINT, QSize(this->width()-80, this->height()-80)));
	song_name->setGeometry(QRect(NAME_POINT, QSize(this->width()-150, 30)));
	song_time->setGeometry(QRect(QPoint(this->width()-125 ,30), S_TIME_SIZE));
	
}

void MainControlPanel::changeProgress(){
	std::cerr<<"Slider moved"<<std::endl;

	Controler::getControl().setCurrentSampleIndex(progress->value());
}






