#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "player.h"
#include "logger.h"
#include "runnable.h"
using boost::asio::ip::tcp;

class Client: public Runnable{
	private:
		bool play;
		Player player;
		boost::asio::io_service io_service;
		tcp::resolver resolver;
		tcp::resolver::iterator endpoint_iterator;
		tcp::socket socket;
		StreamQueue  *queue;
		
		/*variables used for menaging logging*/
		bool log_conn_failed;
		bool play_stream;
		int connection_timeout;
		
		void run();
		bool connectToServer( std::string server, std::string port);
		void sendStream();
	
	public:
		Client(StreamQueue *que, bool pl);
		
		
		
};
