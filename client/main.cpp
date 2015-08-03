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
using boost::asio::ip::tcp;


int main(int arg, char* argv[]){
	QApplication a(arg, argv);
    // ...
		
	/*if(arg<2)
		return -1;
	AudioFile file(argv[1]);
	file.print();
	file.load();
	
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));	
	
	std::cout<<"dur: "<<file.getAudioDuration()<<std::endl;
	std::cout<<"dur: "<<file.getSliceDuration()<<std::endl;*/
//	try{
		PlayList* playlist=new PlayList();
		for(int i=2; i<arg; i++){
			try{
				playlist->addFile(argv[i]);
			}catch(std::string e)
	 		{
			std::cerr << e << std::endl;
	  		}	
		}
		Controler::getControl().setPlayList(playlist);
	//	playlist.addFile(argv[2]);
	/*	playlist.show();
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		playlist.show();
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		playlist.getCurrentStream();
	
	}catch (std::exception& e)
	  {
		std::cerr << e.what() << std::endl;
	  }

	try
	  {
	    if (arg != 3)
	    {
	      std::cerr << "Usage: client <server> <file mp3>" << std::endl;
	      return 1;
	    }*/
		std::string server(argv[1]);
		std::string file(argv[2]);
	    //StreamQueue *que = new StreamQueue;
	    //Player player(que);
	    //player.openFile(file);
	    //player.wrap();
		
	   	Client client( true);
		std::cout<<"Start"<<std::endl;
		client.setServer(server);
		client.start();
		
		ClientControler::getControl().setClient(&client);
		MainWindow w;
		w.show();
	//	client.joinMainThread();

	/*  }
	  catch (std::exception& e)
	  {
		std::cerr << e.what() << std::endl;
	  }*/
  return a.exec();
}

