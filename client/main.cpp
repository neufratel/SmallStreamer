#include "client.h"
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "player.h"
#include "playlist.h"
using boost::asio::ip::tcp;


int main(int arg, char* argv[]){

	
		
	/*if(arg<2)
		return -1;
	AudioFile file(argv[1]);
	file.print();
	file.load();
	
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));	
	
	std::cout<<"dur: "<<file.getAudioDuration()<<std::endl;
	std::cout<<"dur: "<<file.getSliceDuration()<<std::endl;*/
	try{
		PlayList* playlist=new PlayList();
		for(int i=2; i<arg; i++){
			playlist->addFile(argv[i]);
		}
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
	    Client client(playlist, true);
		std::cout<<"Start"<<std::endl;
		client.start();
		client.joinMainThread();
		std::cout<<"Start"<<std::endl;

	  }
	  catch (std::exception& e)
	  {
		std::cerr << e.what() << std::endl;
	  }

  return 0;
}

