#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "str_queue.h"
#include "player.h"
using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: client <host> <mp3>" << std::endl;
      return 1;
    }
    StreamQueue *que = new StreamQueue;
    Player player(que);
    player.openFile(argv);
	
	player.wrap();
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(argv[1], "5555");//daytime
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);
while(que->size()>0)
    {
	boost::system::error_code error;
	boost::asio::write(socket, boost::asio::buffer(que->front().getFrame(), que->front().getFrameSize()), error);
	player.play(que->front());
	que->pop();
	
	std::cout<<"sending..."<<" error: "<<error<<std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
