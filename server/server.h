#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "player.h"
#include "stream.h"
#include "logger.h"
#include <thread>
#include <string>
#include <chrono>
#include "runnable.h"
#include <memory>

typedef std::chrono::duration<int,std::milli> milisec;
using boost::asio::ip::tcp;
using namespace std;

class Server: public Runnable{
	boost::asio::io_service io_service;

	shared_ptr<tcp::acceptor> acceptor;
	shared_ptr<tcp::socket> socket;
	boost::system::error_code error;
	StreamQueue* queue;
	unsigned int port;
	
		
	public:	
		
	Server(StreamQueue* que);
	~Server();
	void setPort(std::string p);
	
	void waitConnection();
	void recieveStream();
	void run();
};

