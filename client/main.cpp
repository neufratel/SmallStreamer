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
	      std::cerr << "Usage: client <host> <mp3>" << std::endl;
	      return 1;
	    }
	    StreamQueue *que = new StreamQueue;
	    Player player(que);
	    player.openFile(argv);
	
		player.wrap();
	//	socket.close();
	  }
	  catch (std::exception& e)
	  {
		std::cerr << e.what() << std::endl;
	  }

  return 0;
}
