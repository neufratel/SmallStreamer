#include "client.h"
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "player.h"
#include "playlist.h"
#include "mainwindow.h"
#include "controler.h"
#include "clientcontroler.h"
#include <QtWidgets/QApplication>
#include "keeper.h"

#include <sys/file.h>
#include <errno.h>
using boost::asio::ip::tcp;


int main(int arg, char* argv[]){

int pid_file = open("/var/run/SmallStreamer.pid", O_CREAT | O_RDWR, 0666);
int rc = flock(pid_file, LOCK_EX | LOCK_NB);
if(rc) {
    if(EWOULDBLOCK == errno)
       std::cout<<"another instance is running"<<std::endl;
}
else {

	QApplication a(arg, argv);
		

		PlayList* playlist=new PlayList();
		for(int i=1; i<arg; i++){
				playlist->addFile(argv[i]);
		}
		Controler::getControl().setPlayList(playlist);
	   	Client client( true);
		std::cout<<"Start"<<std::endl;
		Keeper::getInstance().open();
		client.start();
		ClientControler::getControl().setClient(&client);
		MainWindow w;
		w.show();
  
		a.exec();
		client.stop();
	//	client.joinMainThread(); TODO This not working because client sleep on getSample() from Controler class and later it waits on his own loop 
	Keeper::getInstance().save();
}
	return 0;
}

