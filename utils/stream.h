#ifndef STREAM_H
#define STREAM_H

#include <iostream>
#include <cstring>
#include <stdint.h>
class Stream{
	public:
		static const size_t header=20;
		static short global_volume;
		int channels, encoding;
		long rate;
		int8_t  volume;
		unsigned char* buf;
		size_t length;
		size_t buffer_size;

		static void setSampleClassVolume(int8_t vol);
		static short getSampleClassVolume();
		void print();
		
		Stream(unsigned char*b, size_t len);
		Stream();
		Stream(unsigned char* init);
		Stream(int chan, int enc, long ra, size_t size, unsigned char * b);
		void setData(unsigned char * b);
		unsigned char* getFrame();
		size_t getFrameSize();
			
};
#endif
