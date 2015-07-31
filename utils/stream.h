#ifndef STREAM_H
#define STREAM_H

#include <iostream>
#include <cstring>
#include <stdint.h>
#include <memory>
class Stream{
	private:
		const short static BITS=8;
		int8_t channels;
		int8_t byte_rate;
		long rate;
		int8_t  volume;
		std::shared_ptr<char>  buf;
		size_t buffer_size;
	public:
		static const size_t header=20;
		static short global_volume;

		static void setSampleClassVolume(int8_t vol);
		static short getSampleClassVolume();
		void print();
		
		Stream(unsigned char* init);
		Stream(int chan, int byte, long ra, size_t size, unsigned char * b);
		void setData(unsigned char * b);
		std::shared_ptr<char> getFrame();
		size_t getFrameSize();
		int8_t getChannels(){ return channels;};
		long getRate(){ return rate;}
		char* getBuffer(){ return buf.get();}
		size_t getBufferSize(){ return buffer_size;}
		int8_t getByteRate(){ return byte_rate;}
		int8_t getVolume(){return volume;}
		long long getStreamDuration();
		long long getStreamDurationUS();	
		~Stream();
};
#endif
