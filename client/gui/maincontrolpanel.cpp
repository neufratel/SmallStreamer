#include "maincontrolpanel.h"
#include <QDebug>
#include "controler.h"
#include "stream.h"
#include <QPoint>
#include <QRect>
#include <QFont>


MainControlPanel::MainControlPanel(QWidget *parent) :
    QWidget(parent), playing(false), progress_value(0), 
	button_play(this),
	button_stop(this),
	button_next(this),
	button_pierv(this),
	auto_play_box(this),
	progress(Qt::Horizontal,this), 
	volume(Qt::Horizontal,this),
	song_time("00:00/00:00", this),
	song_name("Waiting...", this)

{
   this->setGeometry(QRect(CONTROL_POINT,CONTROL_SIZE));
   this->setFixedHeight(120);
   this->setVisible(true);
   this->setStyleSheet("border-image: url(graphics/clean.png) 0 0 0 0;");
   
	 
    button_play.setGeometry(QRect(B_PLAY_POINT, B_PLAY_SIZE));
    connect(&button_play, SIGNAL (released()), this, SLOT (play()));
    connect(&button_play, SIGNAL (pressed()), this, SLOT (play_pressed()));
    button_play.setVisible(true);
    

    connect(&button_play_timer, SIGNAL (timeout()), this, SLOT (refreshButtonPlay()));
    button_play_timer.start(300);

   
    button_stop.setGeometry(QRect(B_STOP_POINT, B_STOP_SIZE));
    connect(&button_stop, SIGNAL (released()), this, SLOT (stop()));
    connect(&button_stop, SIGNAL (pressed()), this, SLOT (stop_pressed()));
    button_stop.setVisible(true);


    
    button_next.setGeometry(QRect(B_NEXT_POINT, B_NEXT_SIZE));
    connect(&button_next, SIGNAL (released()), this, SLOT (next()));
    connect(&button_next, SIGNAL (pressed()), this, SLOT (next_pressed()));
    button_next.setVisible(true);

  
    button_pierv.setGeometry(QRect(B_PREV_POINT, B_PREV_SIZE));
    connect(&button_pierv, SIGNAL (released()), this, SLOT (pierv()));
    connect(&button_pierv, SIGNAL (pressed()), this, SLOT (pierv_pressed()));
    button_pierv.setVisible(true);

   
    progress.setVisible(true);
    progress.setStyleSheet("QSlider::handle {image: url(graphics/handle.png);};");
    connect(&progress, SIGNAL (sliderMoved(int)), this, SLOT (changeProgress()));
    

    connect(&timer, SIGNAL (timeout()), this, SLOT (setProgress()));
    timer.start(100);

  
 
    connect(&volume, SIGNAL (valueChanged(int)), this, SLOT (setVolume()));
    volume.show();
    volume.setStyleSheet("QSlider::handle {image: url(graphics/handle.png);};");
    
	
	
	auto_play_box.setStyleSheet("QCheckBox::indicator:unchecked {image: url(graphics/check_p.png) 0 0 0 0;} QCheckBox::indicator:checked {image: url(graphics/check_s.png) 0 0 0 0;}");
	auto_play_box.setGeometry(10, 10, 20, 20);
	auto_play_box.setText("AutoPlay");
	auto_play_box.setVisible(true);
	
	
	connect(&auto_play_box, SIGNAL (stateChanged(int)), this, SLOT(autoplay()));
	
	song_name.setFont(QFont( "lucida", 12, QFont::Bold, true ));
	//song_name.setGeometry(QRect(NAME_POINT, NAME_SIZE));
	song_name.setVisible(true);
	
	//song_time = new QLabel("00:00/00:00", this);
	song_time.setFont(QFont( "lucida", 12, QFont::Bold, true ));
	//song_time.setGeometry(QRect(NAME_POINT, NAME_SIZE));
	song_time.setVisible(true);
	button_play.setStyleSheet("border-image: url(graphics/pause.png) 0 0 0 0;");
	button_next.setStyleSheet("border-image: url(graphics/next.png) 0 0 0 0;");
	button_pierv.setStyleSheet("border-image: url(graphics/prev.png) 0 0 0 0;");
	button_stop.setStyleSheet("border-image: url(graphics/stop.png) 0 0 0 0;");
}

void MainControlPanel::autoplay(){
		
	Controler::getControl().setAutoPlay(auto_play_box.isChecked());
}

void MainControlPanel::play(){
	if(playing==true){
		qDebug()<<"Stoping...";
		Controler::getControl().stop();
		button_play.setStyleSheet("border-image: url(graphics/play.png) 0 0 0 0;");
		//button_play.setText("P");
		playing=false;
	}else{
		qDebug()<<"Playing...";
	 	Controler::getControl().play();
		button_play.setStyleSheet("border-image: url(graphics/pause.png) 0 0 0 0;");
		//button_play.setText("S");
		playing=true;
	}
}
void MainControlPanel::play_pressed(){
	if(playing==true){
		button_play.setStyleSheet("border-image: url(graphics/stop_f.png) 0 0 0 0;");
	}else{	 	
		button_play.setStyleSheet("border-image: url(graphics/play_f.png) 0 0 0 0;");
	}
}


void MainControlPanel::next(){
   	button_next.setStyleSheet("border-image: url(graphics/next.png) 0 0 0 0;");
	Controler::getControl().nextFile();
}
void MainControlPanel::next_pressed(){
	button_next.setStyleSheet("border-image: url(graphics/next_f.png) 0 0 0 0;");
}

void MainControlPanel::pierv(){
    	button_pierv.setStyleSheet("border-image: url(graphics/prev.png) 0 0 0 0;");
	Controler::getControl().prevFile();
}
void MainControlPanel::pierv_pressed(){
    	button_pierv.setStyleSheet("border-image: url(graphics/prev_f.png) 0 0 0 0;");
}

void MainControlPanel::stop(){
	button_stop.setStyleSheet("border-image: url(graphics/stop.png) 0 0 0 0;");
	playing=false;

	Controler::getControl().stop();
	Controler::getControl().setCurrentFileIndex(0);	
}
void MainControlPanel::stop_pressed(){
	button_stop.setStyleSheet("border-image: url(graphics/stop_f.png) 0 0 0 0;");
}

void MainControlPanel::setProgress(){
	if(Controler::getControl().size()){
		song_name.setText(Controler::getControl().getAudioFileName(Controler::getControl().getCurrentFileIndex()).c_str());
		song_time.setText((Controler::getControl().getCurrentPositionTime()+"/"+Controler::getControl().getCurrentFileTime()).c_str());
	}
	if(true){
		progress.setMaximum(Controler::getControl().getAudioSize(Controler::getControl().getCurrentFileIndex()));
		try{
			progress_value=Controler::getControl().getCurrentSampleIndex();
	    }catch(std::logic_error e) {

    		std::cerr <<"MainControlPanel"<< e.what();
			progress_value=0;
		}
	}
	
	progress.setValue((int)progress_value);
}
void MainControlPanel::setVolume(){
     Stream::setSampleClassVolume(volume.value());
}


void MainControlPanel::refreshButtonPlay(){
	playing=Controler::getControl().isPlaying();
	if(playing){
	//	button_play.setText("S");
		button_play.setStyleSheet("border-image: url(graphics/pause.png) 0 0 0 0;");
	}else{
	//	button_play.setText("P");
		button_play.setStyleSheet("border-image: url(graphics/play.png) 0 0 0 0;");
	}
}

void MainControlPanel::resizeEvent(QResizeEvent* event)
{
   QWidget::resizeEvent(event);
   // Your code here.
	
	 volume.setGeometry(QRect(QPoint(this->width(), this->height())-VOLUME_POINT,VOLUME_SIZE));
	progress.setGeometry(QRect(PROG_BAR_POINT, QSize(this->width()-80, this->height()-80)));
	song_name.setGeometry(QRect(NAME_POINT, QSize(this->width()-150, 30)));
	song_time.setGeometry(QRect(QPoint(this->width()-125 ,30), S_TIME_SIZE));
	
}

void MainControlPanel::changeProgress(){
	std::cerr<<"Slider moved"<<std::endl;

	Controler::getControl().setCurrentSampleIndex(progress.value());
}






