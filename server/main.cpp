
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "player.h"
#include "stream.h"
#include "logger.h"
#include <thread>
#include <chrono>
#include "server.h"

typedef std::chrono::duration<int,std::milli> milisec;
using boost::asio::ip::tcp;


int main(int ar, char* arg[] )
{
  try
  {
	Logger::getInstance().msg(std::string("Program started"));
	StreamQueue *que = new StreamQueue;
	Player player(que);
	player.start();
	player.start_sec();

	Server server(que);
		if(ar==2){
			server.setPort(std::string(arg[1]));
		}
	
	server.start();
		
	server.joinMainThread();
	std::cout<<"Server joined"<<std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
	std::string exp(e.what());
    Logger::getInstance().msg(std::string("Exeption occur :")+exp);
  }

  return 0;
}

