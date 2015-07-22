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
    	long rate;
		StreamQueue* queue;
	
	
	
	void play(unsigned char * buff , size_t buff_size){
		 /* It play decoded frame (buff) with size (buff_size) if device is
		 correctly configured. If configuration is invalid it will play stream
		 unproperly. */
   		
        		ao_play(dev, buff, buff_size);

	}
	/* Implementation of Runnable interface, this is main functionality of Player,
	witch is playing Stream form StreamQueue*/
	void run(){
		Logger::getInstance().msg(std::string("Player: running player "));
		while(run_)
		if(queue->size()>0){
			if(rate!=queue->front().rate || encoding!= queue->front().encoding || channels!=queue->front().channels){
				rate=queue->front().rate;
				encoding= queue->front().encoding;
				channels=queue->front().channels;
				if(dev_is_open){
					ao_close(dev);			
				}
				format.bits = mpg123_encsize(encoding) * BITS;
				format.rate = rate;
				format.channels = channels;
				format.byte_format = AO_FMT_NATIVE;
				format.matrix = 0;
				dev = ao_open_live(driver, &format, NULL);
				dev_is_open=true;
			}
        		play(queue->front().buf,queue->front().buffer_size);
			queue->pop();
		}		
		
	}
	public:
		Player(StreamQueue* q){
			/* initializations */
			ao_initialize();
			driver = ao_default_driver_id();
			mpg123_init();
			mh = mpg123_new(NULL, &err);
			buffer_size = mpg123_outblock(mh);
			buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
			dev_is_open=false;
			queue=q;
			Logger::getInstance().msg(std::string("Player: module created "));
		}
		Player(){
			/* initializations */
			ao_initialize();
			driver = ao_default_driver_id();
			mpg123_init();
			mh = mpg123_new(NULL, &err);
			buffer_size = mpg123_outblock(mh);
			buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
			dev_is_open=false;
			Logger::getInstance().msg(std::string("Player: module created "));
		}
	/*If path to file is valid and file format can be decoded with mpg123 library
	it reads the file configuration and prepere the device for suspected output */
	void openFile(std::string file){
			/* open the file and get the decoding format */
			mpg123_open(mh, file.c_str());
			mpg123_getformat(mh, &rate, &channels, &encoding);
			/* set the output format and open the output device */
			format.bits = mpg123_encsize(encoding) * BITS;
			format.rate = rate;
			format.channels = channels;
			format.byte_format = AO_FMT_NATIVE;
			format.matrix = 0;
			dev = ao_open_live(driver, &format, NULL);
			Logger::getInstance().msg(std::string("Player: opening file: ")+file);
	}
	/*If path to file is valid and file format can be decoded with mpg123 library
	it reads the file configuration and prepere the device for suspected output */
	void openFile(char *argv[]){
			/* open the file and get the decoding format */
			mpg123_open(mh, argv[2]);
			mpg123_getformat(mh, &rate, &channels, &encoding);
			/* set the output format and open the output device */
			format.bits = mpg123_encsize(encoding) * BITS;
			format.rate = rate;
			format.channels = channels;
			format.byte_format = AO_FMT_NATIVE;
			format.matrix = 0;
			dev = ao_open_live(driver, &format, NULL);
			Logger::getInstance().msg(std::string("Player: opening file: ")+std::string(argv[2]));
	}
	/*If Player instance is correctly created, it connfigure device 
	for data format included in stream and starts playing stream frame.*/
	void play(Stream s){
		if(rate!=s.rate || encoding!= s.encoding || channels!=s.channels){
				rate=s.rate;
				encoding=s.encoding;
				channels=s.channels;
				if(dev_is_open){
					ao_close(dev);			
				}
				format.bits = mpg123_encsize(encoding) * BITS;
				format.rate = rate;
				format.channels = channels;
				format.byte_format = AO_FMT_NATIVE;
				format.matrix = 0;
				dev = ao_open_live(driver, &format, NULL);
				dev_is_open=true;
			}
        		play(s.buf,s.buffer_size);
	}
	
	
	/*It reads open file in chuncs and stores them in StreamQueue.*/
	void wrap(){
		Logger::getInstance().msg(std::string("Player: wraping file"));
		while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
			std::cout<<buffer_size<<" "<<done<<std::endl;
			Stream stream(channels, encoding, rate, buffer_size, buffer);
			queue->push(stream);
		}
		Logger::getInstance().msg(std::string("Player: finished wrapping"));


	}
	
	~Player(){
			/* clean up */
			free(buffer);
			ao_close(dev);
			mpg123_close(mh);
			mpg123_delete(mh);
			mpg123_exit();
			ao_shutdown();
	}

};

 	
#endif
  

   
    
