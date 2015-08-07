#include "button.h"

Button::Button( QWidget *parent = 0):QPushButton(parent){
		//QPixmap pixmap("gui/green.png");
		this->setStyleSheet("border-image: url(gui/play.png) 0 0 0 0;");// border-top: 10px transparent; border-bottom: 10px transparent;  border-right: 10px transparent;   border-left: 10px transparent; ");
	//	QIcon ButtonIcon(pixmap);
	//	this->setIcon(ButtonIcon);
	}

