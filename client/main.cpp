#include "client.h"
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "player.h"
using boost::asio::ip::tcp;


int main(int arg, char* argv[]){
	try
	  {
	    if (arg != 3)
	    {
	      std::cerr << "Usage: client <server> <file mp3>" << std::endl;
	      return 1;
	    }
		std::string server(argv[1]);
		std::string file(argv[2]);
	    StreamQueue *que = new StreamQueue;
	    Player player(que);
		player.openFile(file);
	    player.wrap();
	    Client client(que, true);
		std::cout<<"Start"<<std::endl;
		client.start();
		client.joinMainThread();
	  }
	  catch (std::exception& e)
	  {
		std::cerr << e.what() << std::endl;
	  }

  return 0;
}

