#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "logger.h"
#include "runnable.h"
#include "client.h"
#include "playlist.h"

using boost::asio::ip::tcp;


	Client::Client(PlayList *play, bool pl=false)
		:io_service(), resolver(io_service), socket(io_service), play_stream(pl),
			server("192.168.0.5"), port("5555")
	{
		playlist=play;
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
	//		std::cerr << e.what() << std::endl;
			if(log_conn_failed){
				Logger::getInstance().msg(std::string("Client: connection failed: ")+e.what());
				log_conn_failed=false;
			}
		}

	
	}


	void Client::sendStream(){
		boost::system::error_code error;
		while(playlist->size()>0)
			{
			
			Stream::setSampleClassVolume(30);
			boost::system::error_code error;
			Stream* stream=nullptr;
			 	while(stream==nullptr){
					stream =playlist->getCurrentStream();
				}
			boost::asio::write(socket, boost::asio::buffer(stream->getFrame(), stream->getFrameSize()), error);
			if(error!=0){
				 Logger::getInstance().msg(std::string("Client: writing error: ")
						+ error.category().name()+ std::to_string(error.value()) );
				 return;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(stream->getStreamDuration()-2));
		}
		socket.close();
	}
	
void Client::run(){
		std::cout<<"Client run"<<std::endl;
		bool connected=false;
		while(run_){
			connected=connectToServer(server,port);
			if(connected){
				sendStream();
			}
		}			
	}

void Client::setServer(std::string s){ server=s;}
void Client::setPort(std::string p){ port=p;};

