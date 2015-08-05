#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "str_queue.h"
#include "player.h"
#include "stream.h"
#include "logger.h"
#include <thread>
#include <string>
#include <chrono>
#include "runnable.h"
#include <memory>
#include "server.h"
typedef std::chrono::duration<int,std::milli> milisec;
using boost::asio::ip::tcp;
using namespace std;
	
		
Server::Server(StreamQueue* que): io_service(), port(5555) {
		queue=que;
		acceptor=shared_ptr<tcp::acceptor>(nullptr);
		timeout_time=20000;
		socket=shared_ptr<tcp::socket>(nullptr);
	}
Server::~Server(){
	}
void Server::setPort(std::string p){
		port= std::stoi(p);
		std::cerr<<port<<" "<<p<<std::endl;
	}
	
void Server::waitConnection(){
		std::cout<<"Waiting..."<<std::endl;
		acceptor=shared_ptr<tcp::acceptor>(nullptr);

		acceptor=shared_ptr<tcp::acceptor>(new tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), 5555)));

		std::cout<<"Accepted.."<<std::endl;
		socket  =shared_ptr<tcp::socket>( new tcp::socket(io_service));
		acceptor->listen(boost::asio::socket_base::max_connections, ec);
		std::cout<<"Server is waiting for new connection"<<std::endl;
		Logger::getInstance().msg(std::string("Waiting for new connection..."));
		acceptor->accept(*socket);     
		Logger::getInstance().msg(std::string("Connected"));

		boost::asio::ip::tcp::endpoint remote_ep = socket->remote_endpoint();
		boost::asio::ip::address remote_ad = remote_ep.address();
		std::string s = remote_ad.to_string();
		std::cout<<"Connection from: "<<s<<std::endl;
	}
void Server::recieveStream(){
		int waiting=0;
		bool message;
		while(true)
		{

			message=true;
			while(socket->available()<Stream::header&&waiting<timeout_time){
				if(!acceptor->is_open()){
					std::cout<<"Stream broke..."<<std::endl;
				}
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
	
	
			std::shared_ptr<unsigned char> stream_head=std::shared_ptr<unsigned char>(new unsigned char[Stream::header], [](unsigned char *p){ delete[] p; } ); 
			boost::system::error_code ignored_error;
			size_t len=boost::asio::read(*socket,boost::asio::buffer(stream_head.get(), Stream::header), ignored_error);
			Stream frame(stream_head.get());
	/*		std::cout<<"C: "<<frame.channels<<" E: "<<frame.encoding
					<<" R: "<<frame.rate<<" Error:"<<ignored_error<<std::endl;*/

			while(socket->available()<frame.getBufferSize()&&waiting<timeout_time){
	
				if(!acceptor->is_open()){
					std::cout<<"Stream broke..."<<std::endl;
				}
				
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
			
			std::shared_ptr<unsigned char> data=std::shared_ptr<unsigned char>(new unsigned char[frame.getBufferSize()], [](unsigned char *p){ delete[] p; } );
			
			len=boost::asio::read(*socket,boost::asio::buffer(data.get(), frame.getBufferSize()), ignored_error);
			
			frame.setData(data.get());
			frame.print();
			queue->push(frame);
		}
}

void Server::run(){
			while(run_){
				waitConnection();
				recieveStream();
			}
}


