#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "logger.h"
#include "runnable.h"
#include "playlist.h"
using boost::asio::ip::tcp;

class Client: public Runnable{
	private:
		boost::asio::io_service io_service;
		tcp::resolver resolver;
		tcp::resolver::iterator endpoint_iterator;
		tcp::socket socket;
		
		std::string server;
		std::string port;
		/*variables used for menaging logging*/
		bool log_conn_failed;
		bool play_stream;
		int connection_timeout;
		
		void run();
		bool connectToServer( std::string server, std::string port);
		void sendStream();
	
	public:
		Client( bool play);
		void setServer(std::string s);
		void setPort(std::string p);
		
		
};
