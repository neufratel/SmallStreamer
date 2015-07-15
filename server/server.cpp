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
#include <thread>
#include <chrono>

typedef std::chrono::duration<int,std::milli> milisec;
using boost::asio::ip::tcp;

std::string make_daytime_string()
{
 std::string ret("z");
	for(int i=0; i<70000; i++) ret+="a"; ret+="x";
 return ret;
}


int main(int ar, char* arg[] )
{
  try
  {
	StreamQueue *que = new StreamQueue;
	Player player(que);
	player.start();
//	player.openFile(arg);	
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 5555));
	std::cout<<"for"<<std::endl;
      tcp::socket socket(io_service);
 	acceptor.accept(socket);     
	//

    while(true)
    {

 	int k=socket.available();
	if(k<20) std::this_thread::sleep_for(milisec(1));
	
	
	unsigned char* var= new unsigned char[20];
	   
	boost::system::error_code ignored_error;
	size_t len=boost::asio::read(socket,boost::asio::buffer(var, 20), ignored_error);
	Stream frame(var);
	k=socket.available();
	if(k<frame.buffer_size) std::this_thread::sleep_for(milisec(1));
	if(k==0)continue;
	unsigned char *data = new unsigned char[frame.buffer_size];
	len=boost::asio::read(socket,boost::asio::buffer(data, frame.buffer_size), ignored_error);
	frame.setData(data);
	frame.print();
		que->push(frame);
	std::cout<<k<<"\t"<<len<<"\t"<<ignored_error<<std::endl;
       	//boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
//boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }	
	
	//std::string sample;
	/*unsigned char sam[73728];
	int iter;
	while(que->size()>0){
	int c=0;
		std::cout<<iter<<" "<<que->front().length<<"  "<<que->size()<<std::endl;
	while(c<73728){
		if(iter>=que->front().length){
			que->pop();
			iter=0;
			continue;
		}
		sam[c]=que->front().buf[iter];
		
			c++;
			++iter;
	}*/
    	//player.play(sam);
	
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
