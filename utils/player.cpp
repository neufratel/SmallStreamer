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
#include <cstring>
#include <chrono>
#include <thread>
#include "player.h"

	
	
	void Player::play(unsigned char * buff , size_t buff_size){
		 /* It play decoded frame (buff) with size (buff_size) if device is
		 correctly configured. If configuration is invalid it will play stream
		 unproperly. */
 // 				mpg123_volume_change( mh,0.5) ;
        		ao_play(dev, buff, buff_size);

	}
	void Player::run_sec(){
		short old_vol=volume;
		while(run_sec_){
			if(volume!=old_vol){
				setVolume(volume);
				old_vol=volume;
			}{
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
	}
	/* Implementation of Runnable interface, this is main functionality of Player,
	witch is playing Stream form StreamQueue*/
	void Player::run(){
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
			if(queue->front().volume!=volume){
				volume=queue->front().volume;	
			}
        		play(queue->front().buf,queue->front().buffer_size);
			queue->pop();
		}		
		
	}
		Player::Player(StreamQueue* q){
			/* initializations */
			ao_initialize();
			driver = ao_default_driver_id();
			mpg123_init();
			mh = mpg123_new(NULL, &err);
			mpg123_volume 	( mh,1) ;
			volume=20;
			buffer_size = mpg123_outblock(mh);
			buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
			dev_is_open=false;
			queue=q;
			Logger::getInstance().msg(std::string("Player: module created "));
		}
		Player::Player(){
			/* initializations */
			ao_initialize();
			driver = ao_default_driver_id();
			mpg123_init();
			mh = mpg123_new(NULL, &err);
			mpg123_volume 	( mh,1.0) ;	
			buffer_size = mpg123_outblock(mh);
			buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
			dev_is_open=false;
			Logger::getInstance().msg(std::string("Player: module created "));
		}
	/*If path to file is valid and file format can be decoded with mpg123 library
	it reads the file configuration and prepere the device for suspected output */
	void Player::openFile(std::string file){
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
	void Player::openFile(char *argv[]){
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
	void Player::play(Stream s){
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
		if(s.volume!=volume){
			volume=s.volume;	
		}
        		play(s.buf,s.buffer_size);
	}
	
	
	/*It reads open file in chuncs and stores them in StreamQueue.*/
	void Player::wrap(){
		Logger::getInstance().msg(std::string("Player: wraping file"));
		struct mpg123_frameinfo frameinfo;
		mpg123_info (mh, &frameinfo);
		std::cout<<"frame bitrate "<<frameinfo.bitrate<<std::endl;
		while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
			Stream stream(100,channels, encoding, rate, buffer_size, buffer);
			queue->push(stream);
		}
		Logger::getInstance().msg(std::string("Player: finished wrapping"));


	}
	
	Player::~Player(){
			/* clean up */
			free(buffer);
			ao_close(dev);
			mpg123_close(mh);
			mpg123_delete(mh);
			mpg123_exit();
			ao_shutdown();
	}
	void Player::setVolume(short vol){
		FILE *in;
		char buff[512];
		std::stringstream comand;
		std::cout<<comand.str()<<std::endl;
		comand<<"amixer -D pulse sset Master "<<vol<<"%";
		if(!(in = popen(comand.str().c_str(), "r"))){
			std::cout<<"cant volume";
		}
		//while(fgets(buff, sizeof(buff), in)!=NULL){
		//	std::cout << buff;
		//}
		pclose(in);

	}

  

   
    
