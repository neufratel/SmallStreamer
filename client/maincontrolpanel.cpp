#include "maincontrolpanel.h"
#include <QDebug>
#include "controler.h"
#include "stream.h"


MainControlPanel::MainControlPanel(QWidget *parent) :
    QWidget(parent), playing(false), progress_value(0)
{
   this->setGeometry(30,30, 270, 120);
   this->setFixedHeight(120);
   this->setFixedWidth(270);
   this->setVisible(true);

    button_play=new QPushButton("P", this);
    button_play->setGeometry(30, 30, 50, 30);
    connect(button_play, SIGNAL (released()), this, SLOT (play()));
    button_play->setVisible(true);
	button_play_timer= new QTimer(this);
    connect(button_play_timer, SIGNAL (timeout()), this, SLOT (refreshButtonPlay()));
    button_play_timer->start(300);


    button_next=new QPushButton(">>", this);
    button_next->setGeometry(100, 30, 50, 30);
    connect(button_next, SIGNAL (released()), this, SLOT (next()));
    button_next->setVisible(true);

    button_pierv=new QPushButton("<<", this);
    button_pierv->setGeometry(170, 30, 50, 30);
    connect(button_pierv, SIGNAL (released()), this, SLOT (pierv()));
    button_pierv->setVisible(true);

    progress =new QProgressBar(this);
    progress->setGeometry(30, 70, 190, 30);
    progress->setVisible(true);
    a=0;
    timer= new QTimer(this);
    connect(timer, SIGNAL (timeout()), this, SLOT (setProgress()));
    timer->start(100);

    volume = new QSlider(this);
    volume->setGeometry(230, 30, 20, 70);
    connect(volume, SIGNAL (valueChanged(int)), this, SLOT (setVolume()));
    volume->show();
	
	auto_play_box = new QCheckBox(this);
	auto_play_box->setGeometry(10, 10, 20, 20);
	auto_play_box->setText("AutoPlay");
	auto_play_box->setVisible(true);
	connect(auto_play_box, SIGNAL (stateChanged(int)), this, SLOT(autoplay()));

}

void MainControlPanel::autoplay(){
		
	Controler::getControl().setAutoPlay(auto_play_box->isChecked());
}

void MainControlPanel::play(){
	if(playing==true){
		qDebug()<<"Stoping...";
		Controler::getControl().stop();
		button_play->setText("P");
		playing=false;
	}else{
		qDebug()<<"Playing...";
	 	Controler::getControl().play();
		button_play->setText("S");
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
	if(playing){
		try{
			progress_value=(100*Controler::getControl().getCurrentSampleIndex())/Controler::getControl().getAudioSize(Controler::getControl().getCurrentFileIndex());
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
		button_play->setText("S");
	}else{
		button_play->setText("P");
	}
}



