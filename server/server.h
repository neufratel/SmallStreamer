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
#include "runnable.h"

typedef std::chrono::duration<int,std::milli> milisec;
using boost::asio::ip::tcp;

class Server: public Runnable{
	boost::asio::io_service io_service;
	tcp::acceptor* acceptor;
	tcp::socket* socket;
	boost::system::error_code ec;
	StreamQueue* queue;
	int timeout_time;
		
	public:	
		
	Server(StreamQueue* que): io_service(){
		queue=que;
		acceptor=nullptr;
		timeout_time=20000;
		socket=nullptr;
	}
	~Server(){
		if(acceptor!=nullptr){
			delete acceptor;
		}
		if(socket!=nullptr){
			delete socket;
		}	
	}
	
	void waitConnection(){
		if(acceptor!=nullptr){
			std::cout<<"Clining acceptor..."<<std::endl;
			delete acceptor;
		}
		if(socket!=nullptr){
			delete socket;
			std::cout<<"Clining socket..."<<std::endl;
		}
		std::cout<<"Waiting..."<<std::endl;
		acceptor=new tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), 5555));
		socket = new tcp::socket(io_service);
		acceptor->listen(boost::asio::socket_base::max_connections, ec);
		std::cout<<"Server is waiting for new connection";
		Logger::getInstance().msg(std::string("Waiting for new connection..."));
		acceptor->accept(*socket);     
		Logger::getInstance().msg(std::string("Connected"));
	}
	void recieveStream(){
		int waiting=0;
		bool message;
		while(true)
		{

			message=true;
			while(socket->available()<Stream::header&&waiting<timeout_time){
				if(!acceptor->is_open()){
					std::cout<<"Stream broke..."<<std::endl;
				}
	/*			if(message==true){
					std::cout<<"Waiting for head..."<<std::endl;
					message=false;
				}*/
				++waiting;
				std::this_thread::sleep_for(std::chrono::microseconds(100));
			}
			message=true;
			if(waiting>=timeout_time){
				std::cout<<"Connection is lost"<<std::endl;
				Logger::getInstance().msg(std::string("Connection probably lost before geting a message head."));
				return;
			}else{
				waiting=0;
			}
	
	
			unsigned char* var= new unsigned char[Stream::header];
			   
			boost::system::error_code ignored_error;
			size_t len=boost::asio::read(*socket,boost::asio::buffer(var, Stream::header), ignored_error);
			Stream frame(var);
	/*		std::cout<<"C: "<<frame.channels<<" E: "<<frame.encoding
					<<" R: "<<frame.rate<<" Error:"<<ignored_error<<std::endl;*/

			while(socket->available()<frame.buffer_size&&waiting<timeout_time){
				if(!acceptor->is_open()){
					std::cout<<"Stream broke..."<<std::endl;
				}
			/*	if(message==true){
					std::cout<<"Waiting for data..."<<std::endl;
					message=false;
				}*/
				std::this_thread::sleep_for(std::chrono::microseconds(100));
			}
			message=true;
			if(waiting>=timeout_time){
				std::cout<<"Connection is lost"<<std::endl;
				Logger::getInstance().msg(std::string("Connection probably lost."));
				return;
			}else{
				waiting=0;
			}

			unsigned char *data = new unsigned char[frame.buffer_size];
			len=boost::asio::read(*socket,boost::asio::buffer(data, frame.buffer_size), ignored_error);
			frame.setData(data);
	//		frame.print();
				queue->push(frame);
	//		std::cout<<socket->available()<<"\t"<<len<<"\t"<<ignored_error<<std::endl;
		}
	}
	void run(){
			while(run_){
				waitConnection();
				recieveStream();
			}
	}
};

