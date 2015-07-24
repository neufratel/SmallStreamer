#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "player.h"
#include "logger.h"
#include "runnable.h"
#include "client.h"

using boost::asio::ip::tcp;


	Client::Client(StreamQueue *que, bool pl=false)
		:io_service(), resolver(io_service), socket(io_service), play_stream(pl)
	{
		queue=que;
		log_conn_failed=true;
		Logger::getInstance().msg(std::string("Client: module created"));
	}
		
	bool Client::connectToServer( std::string server, std::string port){

		tcp::resolver::query query(server, port);
		endpoint_iterator = resolver.resolve(query);
		
		try{
			boost::asio::connect(socket, endpoint_iterator);
			log_conn_failed=true;
			Logger::getInstance().msg(std::string("Client: connected to: ")+server+":"+port);
			return true;
		
		}catch (std::exception& e){
			//std::cerr << e.what() << std::endl;
			if(log_conn_failed){
				Logger::getInstance().msg(std::string("Client: connection failed: ")+e.what());
				log_conn_failed=false;
			}
		}

	
	}


	void Client::sendStream(){
		boost::system::error_code error;
		std::cout<<queue->size()<<std::endl;
		while(queue->size()>0)
			{
			
			Stream::setSampleClassVolume(10+queue->size()%90);
			boost::system::error_code error;
			boost::asio::write(socket, boost::asio::buffer(queue->front().getFrame(), queue->front().getFrameSize()), error);
			if(error!=0){
				 Logger::getInstance().msg(std::string("Client: writing error: ")
						+ error.category().name()+ std::to_string(error.value()) );
				 std::cout<<"Streaming stopt"<<std::endl;
				 return;
			}
			if(play_stream){
				player.play(queue->front());
			}
			queue->pop();
		}
		socket.close();
	}
	
void Client::run(){
		std::cout<<"Client run"<<std::endl;
		bool connected=false;
		while(run_){
			connected=connectToServer("localhost","5555");
			if(connected){
				sendStream();
			}
		}			
	}

