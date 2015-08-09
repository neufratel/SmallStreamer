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
		socket=shared_ptr<tcp::socket>(nullptr);
	}
Server::~Server(){
	}
void Server::setPort(std::string p){
		port= std::stoi(p);
		std::cout<<"Server port is: "<<port<<std::endl;
	}
	
void Server::waitConnection(){
		std::cout<<"Waiting..."<<std::endl;
		acceptor.reset();
		socket.reset();

		acceptor=shared_ptr<tcp::acceptor>(new tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), port)));
		std::cout<<"Socket"<<std::endl; 
		socket  =shared_ptr<tcp::socket>( new tcp::socket(io_service), [](tcp::socket* s){s->close(); delete s;});
			std::cout<<"Listen"<<std::endl; 
		acceptor->listen(boost::asio::socket_base::max_connections, error);
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

		while(true)
		{
		/** Reciving header of frame */
			std::shared_ptr<unsigned char> stream_head=std::shared_ptr<unsigned char>(new unsigned char[Stream::header], [](unsigned char *p){ delete[] p; } ); 
			boost::system::error_code error;
			
			boost::asio::read(*socket,boost::asio::buffer(stream_head.get(), Stream::header), error);
			
			if(error){
				std::cerr<<error<<std::endl;
				Logger::getInstance().msg(std::string("Server::recivingStream::error while reading head"));
				return;
			}
			Stream* frame= new Stream(stream_head.get());
			

		/** Reciving data of frame*/
			std::shared_ptr<unsigned char> data=std::shared_ptr<unsigned char>(new unsigned char[frame->getBufferSize()], [](unsigned char *p){ delete[] p; } );
			
			boost::asio::read(*socket,boost::asio::buffer(data.get(), frame->getBufferSize()), error);
			
			if(error){
				std::cerr<<error<<std::endl;
				Logger::getInstance().msg(std::string("Server::recivingStream::error while reading data"));
				return;
			}
			
			frame->setData(data.get());
			frame->print();
			queue->push(frame);
			
		/**Sending back header for syncronization with client */
			boost::asio::write(*socket, boost::asio::buffer(stream_head.get(), Stream::header), error);
			
			if(error){
				std::cerr<<error<<std::endl;
				Logger::getInstance().msg(std::string("Server::recivingStream::error while writing respons"));
				return;
			}
		}
}

void Server::run(){
			while(run_){
				waitConnection();
				recieveStream();
			}
}


