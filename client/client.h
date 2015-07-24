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
	
	public:
		Client(StreamQueue *que, bool pl=false)
			:io_service(), resolver(io_service), socket(io_service), play_stream(pl)
		{
			queue=que;
			log_conn_failed=true;
			Logger::getInstance().msg(std::string("Client: module created"));
			connection_timeout=0;
		}
		
		bool connectToServer( std::string server, std::string port){

			tcp::resolver::query query(server, port);
			endpoint_iterator = resolver.resolve(query);
			
			try{
				boost::asio::connect(socket, endpoint_iterator);
				log_conn_failed=true;
				Logger::getInstance().msg(std::string("Client: connected to: ")+server+":"+port);
				connection_timeout=0;
				return true;
			
			}catch (std::exception& e){
				std::cerr << e.what() << std::endl;
				if(log_conn_failed){
					Logger::getInstance().msg(std::string("Client: connection failed: ")+e.what());
					log_conn_failed=false;
				}
				if(connection_timeout<100){
					connection_timeout++;
					std::this_thread::sleep_for(std::chrono::microseconds(100000));
					connectToServer(server, port);
				}else{
					this->stop();
					return false;
				}
			}

		
		}
		void sendStream(){
			boost::system::error_code error;

			while(queue->size()>0)
    			{
				boost::system::error_code error;
				boost::asio::write(socket, boost::asio::buffer(queue->front().getFrame(), queue->front().getFrameSize()), error);
				if(error!=0){
					 Logger::getInstance().msg(std::string("Client: writing error: ")
							+ error.category().name()+ std::to_string(error.value()) );
					 std::cout<<"Streaming stopt"<<std::endl;
					 return;
				}
				if(play_stream){
					player.play(queue->front());
				}
				queue->pop();
			}
			socket.close();
		}
		void run(){
			std::cout<<"Client run"<<std::endl;
			bool connected=false;
			while(run_){
				connected=connectToServer("localhost","5555");
				if(connected){
					sendStream();
				}
			}			
		}
};
