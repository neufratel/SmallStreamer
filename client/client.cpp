#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "logger.h"
#include "runnable.h"
#include "client.h"
#include "playlist.h"
#include "controler.h"
using boost::asio::ip::tcp;


	Client::Client( bool pl=false)
		:io_service(), resolver(io_service), socket(io_service), play_stream(pl),
			server("localhost"), port("5555"), maintain_connection(false)
	{
		log_conn_failed=true;
		Logger::getInstance().msg(std::string("Client: module created"));
	}
		
	bool Client::connectToServer( std::string server, std::string port){

		//std::cerr<<std::string("Client: connecting to: ")+server+":"+port<<std::endl;
		tcp::resolver::query query(server, port);
		endpoint_iterator = resolver.resolve(query);
		
		try{
			boost::asio::connect(socket, endpoint_iterator);
			log_conn_failed=true;
			Logger::getInstance().msg(std::string("Client: connected to: ")+server+":"+port);
			maintain_connection=true;
			return true;
		
		}catch (std::exception& e){
	//		std::cerr << e.what() << std::endl;
			if(log_conn_failed){
				Logger::getInstance().msg(std::string("Client: connection failed: ")+e.what());
				log_conn_failed=false;
			}
			return false;
		}

	
	}


	void Client::sendStream(){
		boost::system::error_code error;
		while(maintain_connection)
			{
			
			boost::system::error_code error;
			Stream* stream=nullptr;
			 	while(stream==nullptr){
					stream = Controler::getControl().getCurrentStream();
				}
			boost::asio::write(socket, boost::asio::buffer(stream->getFrame().get(), stream->getFrameSize()), error);
			if(error!=0){
				 Logger::getInstance().msg(std::string("Client: writing error: ")
						+ error.category().name()+ std::to_string(error.value()) );
				 return;
			}

			/** Waiting for server response*/
	//*		std::cerr<<"W sockecie"<< socket.available()<<std::endl;
		/*		while(socket.available()<Stream::header){
					std::this_thread::sleep_for(std::chrono::microseconds(100));
					std::cerr<<"IS open "<<socket.is_open()<<std::endl;
				}*/
			std::shared_ptr<unsigned char> stream_head=std::shared_ptr<unsigned char>(new unsigned char[Stream::header], [](unsigned char *p){ delete[] p; } );
            boost::system::error_code ignored_error;
            size_t len=boost::asio::read(socket,boost::asio::buffer(stream_head.get(), Stream::header), ignored_error);
			/*Waiting ended*/


			if(ignored_error!=0){
				 Logger::getInstance().msg(std::string("Client: reading response error: ")
						+ error.category().name()+ std::to_string(error.value()) );
				 return;
			}
	//		std::this_thread::sleep_for(std::chrono::microseconds(stream->getStreamDurationUS()-1000));
		}
		socket.close();
		return;
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

void Client::setServer(std::string s){ 
		std::cerr<<"Client:: zmiana servera:"<<server<<" na "<<s<<std::endl;	
		server=s;
}
void Client::setPort(std::string p){ port=p;};
void Client::setMaintainConnection(bool con){ maintain_connection=con;}

