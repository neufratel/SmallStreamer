/***
  This file is part of PulseAudio.
  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.
  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
  You should have received a copy of the GNU Lesser General Public License
 ` along with PulseAudio; if not, see <http://www.gnu.org/licenses/>.
***/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pulse/simple.h>
#include <pulse/error.h>

/*****************/
#include <iostream>
#include "str_queue.h"
#include "runnable.h"

class PulsePlayer: public Runnable{
	static const int BUFSIZE = 16384;
	pa_sample_spec ss;
	pa_simple *s;
	int ret;
	int error;

	StreamQueue *queue;
	uint8_t buf[BUFSIZE];
        ssize_t r;
	
	
	

	void createStream(){
		if (!(s = pa_simple_new(NULL, NULL, PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
			std::cout<<"Error while creating new stream"<<std::endl;
			exit(-1);
	    	}
 
	}
	void play(char *bu, size_t ra){
		/* ... and play it */
		for(int i=0; i<BUFSIZE; i++){
			buf[i]=bu[i];
		}
		if(s==nullptr){
			 std::cout<<"Sinc not correct"<<std::endl;
			exit(-1);
		}
		if (pa_simple_write(s, buf, (size_t) ra, &error) < 0) {
		   std::cout<<"Error while playing"<<std::endl;
		}
	}
public:
	PulsePlayer(StreamQueue* q){
		queue=q;
		s=nullptr;
		ss = {
			.format = PA_SAMPLE_S16LE,
			.rate = 44100,
			.channels = 2
		    };
		createStream();
	}
	~PulsePlayer(){
		if (pa_simple_drain(s, &error) < 0) {
			std::cout<<"Error while deleting"<<std::endl;
			exit(-1);
	    	}
	   
		if (s)
			pa_simple_free(s);
	}

	void run(){
	//	Logger::getInstance().msg(std::string("Player: running player "));
	//	Stream stream;	
		while(run_){
			std::shared_ptr<Stream> stream=queue->front();
			queue->pop();

		/*	if(rate!=stream->getRate() || byte_rate!= stream->getByteRate() || channels!=stream->getChannels()){
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
			}*/
        		play(stream->getBuffer(),stream->getBufferSize());
		}		
		
	}
};


