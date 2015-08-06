#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <ao/ao.h>
#include <iostream>
#include "str_queue.h"
#include "stream.h"
#include "logger.h"
#include "runnable.h"
#include  <algorithm>
#include <sstream>
#include <stdio.h> 
#include <memory>

using namespace std;

class Player: public Runnable{
	private:
			StreamQueue* queue;

			shared_ptr<ao_device> dev;
	    	ao_sample_format format;
			
			long rate;
			size_t buffer_size;
    		int driver, channels, encoding;
			int8_t byte_rate;
			int8_t volume;
	
	
		void play(unsigned char * buff , size_t buff_size);
		void run();
		void run_sec();
	public:
		Player(StreamQueue* q);
		Player();
		~Player();
		void setVolume(short vol);
		void play(Stream s);
};

 	
#endif
  

   
    
