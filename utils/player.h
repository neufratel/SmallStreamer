#ifndef PLAYER_H
#define PLAYER_H
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
	char * encod_c;
		char * chane_c;
		char * rate_c;
		char * bufsi_c;

		mpg123_handle *mh;
    	unsigned char *buffer;
    	size_t buffer_size;
    	size_t done;
    	int err;
	unsigned char *sample;

    	int driver;
    	ao_device *dev;
	bool dev_is_open;

    	ao_sample_format format;
    	int channels, encoding;
    	long rate;
	StreamQueue* que;
	public:
		Player(StreamQueue* q){
			/* initializations */
			ao_initialize();
			driver = ao_default_driver_id();
			mpg123_init();
			mh = mpg123_new(NULL, &err);
			buffer_size = mpg123_outblock(mh);
			buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
			encod_c=new char[4];
			chane_c=new char[4];
			rate_c=new char[8];
			bufsi_c=new char[4];
			sample= new unsigned char[128000];
			dev_is_open=false;
			que=q;
			Logger::getInstance().msg(std::string("Player: module created "));
		}
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
	void play(unsigned char * buff , size_t buff_size){
		 /* decode and play */
   		
        		ao_play(dev, buff, buff_size);
//			Stream s(channels, encoding, rate, buffer, buffer_size);
//			sender.sendStream(s);
	}
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
	void run(){
		Logger::getInstance().msg(std::string("Player: running player "));
		while(run_)
		if(que->size()>0){
			if(rate!=que->front().rate || encoding!= que->front().encoding || channels!=que->front().channels){
				rate=que->front().rate;
				encoding= que->front().encoding;
				channels=que->front().channels;
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
        		play(que->front().buf,que->front().buffer_size);
			que->pop();
		}		
		
	}
	
	/*void wrap(std::queue<Stream>* queue){
		chane_c[3]=channels 	  & 0xFF;
		chane_c[2]=(channels>>8)  & 0xFF;
		chane_c[1]=(channels>>16) & 0xFF;
		chane_c[0]=(channels>>24) & 0xFF;
		
		encod_c[3]=encoding 	  & 0xFF;
		encod_c[2]=(encoding>>8)  & 0xFF;
		encod_c[1]=(encoding>>16) & 0xFF;
		encod_c[0]=(encoding>>24) & 0xFF;
		
		rate_c[7]=(rate) 	  & 0xFF;
		rate_c[6]=(rate>>8)  & 0xFF;
		rate_c[5]=(rate>>16) & 0xFF;
		rate_c[4]=(rate>>24) & 0xFF;
		rate_c[3]=(rate>>32) & 0xFF;
		rate_c[2]=(rate>>40) & 0xFF;
		rate_c[1]=(rate>>48) & 0xFF;
		rate_c[0]=(rate>>56) & 0xFF;
		std::string tmp=std::string(chane_c, sizeof(char)*4)+std::string(encod_c,sizeof(char)*4)+std::string(rate_c, sizeof(char)*8);
		 while (mpg123_read(mh, buffer, &buffer_size, &done) == MPG123_OK){
			std::cout<<buffer_size<<" "<<done<<std::endl;
			bufsi_c[3]=(buffer_size>>0 ) & 0xFF;
			bufsi_c[2]=(buffer_size>>8 ) & 0xFF;
			bufsi_c[1]=(buffer_size>>16) & 0xFF;
			bufsi_c[0]=(buffer_size>>24) & 0xFF;
			queue->push(tmp+std::string(bufsi_c, sizeof(char)*4)+std::string(buffer, buffer_size));
        	//	ao_play(dev, buffer, done);
//			Stream s(channels, encoding, rate, buffer, buffer_size);
//			sender.sendStream(s);
		}
	}*/
	void wrap(){
		Logger::getInstance().msg(std::string("Player: wraping file"));
		while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
			std::cout<<buffer_size<<" "<<done<<std::endl;
			bufsi_c[3]=(buffer_size>>0 ) & 0xFF;
			bufsi_c[2]=(buffer_size>>8 ) & 0xFF;
			bufsi_c[1]=(buffer_size>>16) & 0xFF;
			bufsi_c[0]=(buffer_size>>24) & 0xFF;
			Stream stream(channels, encoding, rate, buffer_size, buffer);
			que->push(stream);
			//queue->push(tmp+std::string(bufsi_c, sizeof(char)*4)+std::string(buffer, buffer_size));
        	//	ao_play(dev, buffer, done);
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
			delete[] rate_c;
			delete[] encod_c;
			delete[] chane_c;
			delete[] bufsi_c;
			delete[] sample;
	}

};

 	
#endif
  

   
    
