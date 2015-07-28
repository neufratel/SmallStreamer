#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <ao/ao.h>
#include <mpg123.h>
#include <iostream>
#include "str_queue.h"
#include "stream.h"
#include "logger.h"
#include "runnable.h"
#include  <algorithm>
#include <sstream>
#include <stdio.h>  
const short BITS= 8;
class Player: public Runnable{
	private:
		mpg123_handle *mh;
    		unsigned char *buffer;
	    	size_t buffer_size;
    		size_t done;
	    	int err;
	   	int driver;
    		ao_device *dev;
		bool dev_is_open;
	    	ao_sample_format format;
    		int channels, encoding;
		int8_t byte_rate;
	    	long rate;
		StreamQueue* queue;
		int8_t volume;
	
	
	void play(unsigned char * buff , size_t buff_size);
	
	void run();
	void run_sec();
	public:
		Player(StreamQueue* q);
		Player();
	
	void openFile(std::string file);
	
	void openFile(char *argv[]);
	
	void play(Stream s);
	
	/*It reads open file in chuncs and stores them in StreamQueue.*/
	void wrap();
	
	~Player();
	void setVolume(short vol);

};

 	
#endif
  

   
    
