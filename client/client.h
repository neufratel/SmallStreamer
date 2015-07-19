#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "player.h"
#include "logger.h"
using boost::asio::ip::tcp;
using Logger;

class Client{
	private:
		boost::asio:io_service io_service;
		tcp::resolver resolver;
		tcp::resolver::query* query;
		tcp::resolver::iterator endpoint_iterator;
		tcp::socket *socket;
	
	public:
		Client(char * argv[]): io_service(){
			resolver(io_service);
			endpoint_iterator=resolver.resolve(query);
			socket(io_service);
			query=nullptr;
			socket=nullptr;
			getInstance().msg(std::string("Client: module created"));
		}
		void connectToServer( std::string server, std::string port){
			if(query!=nullptr){ delete query;}
			query = new tcp::resolver::query(server, port);
			endpoint_iterator = resolver.resolve(*query);

			if(socket!=nullptr){delete socket;}
			socket=new tcp::socket(io_service());
			
			try{
				getInstance().msg(std::string("Client: trying to connect to: ")+server+":"+port);
				boost::asio::connect(*socket, endpoint_iterator);
				getInstance().msg(std::string("Client: connected to: ")+server+":"+port);
			
			}catch (std::exception& e){
				std::cerr << e.what() << std::endl;
				getInstance().msg(std::string("Client: connection failed: ")+e.what());
			}

		
		}


/* 
    
while(que->size()>0)
    {
	boost::system::error_code error;
	boost::asio::write(socket, boost::asio::buffer(que->front().getFrame(), que->front().getFrameSize()), error);
	//player.play(que->front());
	que->pop();
	
	std::cout<<"sending..."<<" error: "<<error<<std::endl;
    }
	socket.close();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;*/
}
