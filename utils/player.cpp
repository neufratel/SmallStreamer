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
#include <cstring>
#include <chrono>
#include <thread>
#include "player.h"

	
	
void Player::play(unsigned char * buff , size_t buff_size){
		 /* It play decoded frame (buff) with size (buff_size) if device is
		 correctly configured. If configuration is invalid it will play stream
		 unproperly. */
        		ao_play(dev.get(), buff, buff_size);

	}

void Player::run_sec(){
		short old_vol=volume;
		while(run_sec_){
			if(volume!=old_vol){
				if(volume>old_vol){
					short step=2;
					setVolume(old_vol+step);
					old_vol=old_vol+step;
				}else{
					setVolume(volume);
					old_vol=volume;
				}
		
			}else{
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
	}


	/* Implementation of Runnable interface, this is main functionality of Player,
	witch is playing Stream form StreamQueue*/
void Player::run(){
		Logger::getInstance().msg(std::string("Player: running player "));
	//	Stream stream;	
		while(run_)
		if(true){
			std::shared_ptr<Stream> stream=queue->front();
			queue->pop();

			if(rate!=stream->getRate() || byte_rate!= stream->getByteRate() || channels!=stream->getChannels()){
				rate=stream->getRate();
				byte_rate= stream->getByteRate();
				channels=stream->getChannels();
				
				format.bits = byte_rate;
				format.rate = rate;
				format.channels = channels;
				format.byte_format = AO_FMT_NATIVE;
				format.matrix = 0;
					dev=shared_ptr<ao_device>(ao_open_live(driver,&format, NULL), [](ao_device *d){ ao_close(d);});
			}
			if(stream->getVolume()!=volume){
				volume=stream->getVolume();	
			}
        		play(stream->getBuffer(),stream->getBufferSize());
		}		
		
	}
		
Player::Player(StreamQueue* q) {
			/* initializations */
			ao_initialize();
			driver = ao_default_driver_id();
			
			volume=20;
			//dev_is_open=false;
			queue=q;
			Logger::getInstance().msg(std::string("Player: module created "));
		}

Player::Player(){
			ao_initialize();
			driver = ao_default_driver_id();
			//dev_is_open=false;
			Logger::getInstance().msg(std::string("Player: module created "));
		}
	
	/*If Player instance is correctly created, it connfigure device 
	for data format included in stream and starts playing stream frame.*/
void Player::play(Stream s){
		if(rate!=s.getRate() || byte_rate!= s.getByteRate() || channels!=s.getChannels()){
				rate=s.getRate();
				byte_rate=s.getByteRate();
				channels=s.getChannels();
				//if(dev_is_open){
				//	ao_close(dev);			
				//}
				format.bits = byte_rate;
				format.rate = rate;
				format.channels = channels;
				format.byte_format = AO_FMT_NATIVE;
				format.matrix = 0;
				dev=shared_ptr<ao_device>(ao_open_live(driver,&format, NULL), [](ao_device *d){ ao_close(d);});
				//dev = ao_open_live(driver, &format, NULL);
				//dev_is_open=true;
			}
		if(s.getVolume()!=volume){
			volume=s.getVolume();	
		}
        		std::cout<<"player::play"<<std::endl;
			play(s.getBuffer(),s.getBufferSize());
	}
	
	
	
Player::~Player(){
			/* clean up */
			//ao_close(dev);
			ao_shutdown();
	}

void Player::setVolume(short vol){
		FILE *in=nullptr;
		std::stringstream comand;
		comand<<"amixer -D pulse sset Master "<<vol<<"%";
		if(!(in = popen(comand.str().c_str(), "r"))){
			std::cout<<"cant volume";
		}
		pclose(in);
}

  

   
    
