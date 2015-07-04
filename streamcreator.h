#include <ao/ao.h>
#include <mpg123.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "stream.h"
#include "sender.h"
#include <QThread>
#define BITS 8
class StreamCreator{
	private:
		mpg123_handle *mh;
    	unsigned char *buffer;
    	size_t buffer_size;
    	size_t done;
    	int err;

    	int driver;
    	ao_device *dev;

    	ao_sample_format format;
    	int channels, encoding;
    	long rate;
		StreamSender sender;
	public:
		StreamCreator(){
			/* initializations */
			ao_initialize();
			driver = ao_default_driver_id();
			mpg123_init();
			mh = mpg123_new(NULL, &err);
			buffer_size = mpg123_outblock(mh);
			buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
		}
	void openFile( char *argv[]){
			/* open the file and get the decoding format */
			mpg123_open(mh, argv[1]);
			mpg123_getformat(mh, &rate, &channels, &encoding);

			/* set the output format and open the output device */
			format.bits = mpg123_encsize(encoding) * BITS;
			format.rate = rate;
			format.channels = channels;
			format.byte_format = AO_FMT_NATIVE;
			format.matrix = 0;
			dev = ao_open_live(driver, &format, NULL);
	}

	void play(){
		 /* decode and play */
   		 Stream *s;
		 while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
        //		ao_play(dev, buffer, done);
			s = new Stream(channels, encoding, rate, buffer,(buffer_size));
		
			sender.sendStream(s);
		//	play(s);
			QThread::msleep(100);
			delete s;
		}
	}
	void play(Stream *s){
		 /* decode and play */
   			format.bits = mpg123_encsize(s->getEncoding()) * BITS;
			format.rate = s->getRate();
			format.channels = s->getChannels();
			format.byte_format = AO_FMT_NATIVE;
			format.matrix = 0;
			dev = ao_open_live(driver, &format, NULL);
        	ao_play(dev, s->getBuffer(), done);
		
	}
	~StreamCreator(){
			/* clean up */
			free(buffer);
			ao_close(dev);
			mpg123_close(mh);
			mpg123_delete(mh);
			mpg123_exit();
			ao_shutdown();
	}

};

 	

  

   
    
