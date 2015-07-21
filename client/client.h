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
	
	public:
		Client(StreamQueue *que
):io_service(), resolver(io_service), socket(io_service){
			queue=que;
			Logger::getInstance().msg(std::string("Client: module created"));
		}
		void connectToServer( std::string server, std::string port){
				std::cout<<"break"<<std::endl;

			tcp::resolver::query query(server, port);
			endpoint_iterator = resolver.resolve(query);
			std::cout<<"break"<<std::endl;
			
			try{
				Logger::getInstance().msg(std::string("Client: trying to connect to: ")+server+":"+port);
				boost::asio::connect(socket, endpoint_iterator);
				Logger::getInstance().msg(std::string("Client: connected to: ")+server+":"+port);
			
			}catch (std::exception& e){
				std::cerr << e.what() << std::endl;
				Logger::getInstance().msg(std::string("Client: connection failed: ")+e.what());
			}

		
		}
		void sendStream(){
			std::cout<<queue->size()<<std::endl;
			boost::system::error_code error;

			while(queue->size()>0)
    			{
				boost::system::error_code error;
				std::cout<<"beffore"<<std::endl;
				boost::asio::write(socket, boost::asio::buffer(queue->front().getFrame(), queue->front().getFrameSize()), error);
				player.play(queue->front());
				queue->pop();
	
				std::cout<<queue->size()<<"\t: sending..."<<" error: "<<error<<std::endl;
			}
			socket.close();
		}
		void run(){
			std::cout<<"Client run"<<std::endl;
			while(run_){
				connectToServer("localhost","5555");
				sendStream();
			}			
		}
};
