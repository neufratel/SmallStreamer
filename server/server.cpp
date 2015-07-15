//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at ntkohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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
//	player.openFile(arg);	

	bool connection_is_open=false;
	int waiting=0;;
	while(true){
		boost::asio::io_service io_service;

    		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 5555));
		std::cout<<"new"<<std::endl;
    		tcp::socket socket(io_service);
		bool connection_is_open=false;
		int waiting=0;;

		boost::system::error_code ec;
		acceptor.listen(boost::asio::socket_base::max_connections, ec);
		Logger::getInstance().msg(std::string("Waiting for new connection..."));
		acceptor.accept(socket);     
		Logger::getInstance().msg(std::string("Connected"));
		

		connection_is_open=true;
		waiting=0;
		while(connection_is_open)
		{

			bool message=true;
			while(socket.available()<Stream::header&&waiting<2000){
				if(!acceptor.is_open()){
					std::cout<<"Stream broke..."<<std::endl;
				}
				if(message==true){
					std::cout<<"Waiting for head..."<<std::endl;
					message=false;
				}
				++waiting;
				std::this_thread::sleep_for(milisec(1));
			}
			message=true;
			if(waiting>=2000){
				connection_is_open=false;
				std::cout<<"Connection is lost"<<std::endl;
				Logger::getInstance().msg(std::string("Connection probably lost."));
				break;
			}else{
				waiting=0;
			}
	
	
			unsigned char* var= new unsigned char[Stream::header];
			   
			boost::system::error_code ignored_error;
			size_t len=boost::asio::read(socket,boost::asio::buffer(var, Stream::header), ignored_error);
			Stream frame(var);
			std::cout<<"C: "<<frame.channels<<" E: "<<frame.encoding
					<<" R: "<<frame.rate<<" Error:"<<ignored_error<<std::endl;

			while(socket.available()<frame.buffer_size&&waiting<2000){
				if(!acceptor.is_open()){
					std::cout<<"Stream broke..."<<std::endl;
				}
				if(message==true){
					std::cout<<"Waiting for data..."<<std::endl;
					message=false;
				}
				std::this_thread::sleep_for(milisec(1));
			}
			message=true;
			if(waiting>=2000){
				connection_is_open=false;
				std::cout<<"Connection is lost"<<std::endl;
				Logger::getInstance().msg(std::string("Connection probably lost."));
				break;
			}else{
				waiting=0;
			}

			unsigned char *data = new unsigned char[frame.buffer_size];
			len=boost::asio::read(socket,boost::asio::buffer(data, frame.buffer_size), ignored_error);
			frame.setData(data);
			frame.print();
				que->push(frame);
			std::cout<<socket.available()<<"\t"<<len<<"\t"<<ignored_error<<std::endl;
		}
	}	
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
	std::string exp(e.what());
    Logger::getInstance().msg(std::string("Exeption occur :")+exp);
  }

  return 0;
}
