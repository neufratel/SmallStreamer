#include <iostream>
#include <cstring>
#include <stdint.h>
#include "stream.h"

		
		short Stream::global_volume=0;
		
		static void Stream::setSampleClassVolume(int8_t vol){ 
				global_volume=vol;
		}
		static short Stream::getSampleClassVolume(){ 
				return global_volume;
		}
		void Stream::print(){
			std::cout<<"V: "<<(short)volume<<" CH: "<<channels<<" ENC: "<<encoding<<" R: "<<rate<<" size: "<<buffer_size<<std::endl;
		}
		Stream::Stream(unsigned char*b, size_t len): buf(b), length(len){};
		Stream::Stream(){};
		Stream::Stream(unsigned char* init){
			
			channels=((int)init[0])<<24;
			channels+=((int)init[1])<<16;
			channels+=((int)init[2])<<8;
			channels+=((int)init[3]);

			encoding=((int)init[4])<<24;
			encoding+=((int)init[5])<<16;
			encoding+=((int)init[6])<<8;
			encoding+=((int)init[7]);

			volume=	((int8_t)init[8]);
				(init[9]);
				(init[10]);
				(init[11]);
			rate=	((long)init[12])<<24;
			rate+=	((long)init[13])<<16;
			rate+=	((long)init[14])<<8;
			rate+=	((long)init[15])<<0;
			
			buffer_size=((size_t)init[16])<<24;
			buffer_size+=((size_t)init[17])<<16;
			buffer_size+=((size_t)init[18])<<8;
			buffer_size+=((size_t)init[19])<<0;	
		}
		Stream::Stream(int chan, int enc, long ra, size_t size, unsigned char * b):
			 channels(chan), encoding(enc), rate(ra), buffer_size(size)
		{
				volume=Stream::global_volume;
				buf=new unsigned char[buffer_size];
				memcpy ( buf, b, buffer_size );
		};	
		void Stream::setData(unsigned char * b){buf=b;};
		unsigned char* Stream::getFrame(){
				unsigned char *ret=new unsigned char[header+buffer_size];
				
				ret[0]=(channels>>24) & 0xFF;
				ret[1]=(channels>>16) & 0xFF;
				ret[2]=(channels>>8)  & 0xFF;
				ret[3]=channels 	  & 0xFF;
				
				ret[4]=(encoding>>24) & 0xFF;
				ret[5]=(encoding>>16) & 0xFF;
				ret[6]=(encoding>>8)  & 0xFF;
				ret[7]=encoding 	  & 0xFF;
				
				ret[8]=global_volume;
				ret[9]=0;
				ret[10]=0;
				ret[11]=0;
				ret[12]=(rate>>24) & 0xFF;
				ret[13]=(rate>>16) & 0xFF;
				ret[14]=(rate>>8)  & 0xFF;
				ret[15]=(rate) 	  & 0xFF;
				
				ret[16]=(buffer_size>>24) & 0xFF;
				ret[17]=(buffer_size>>16) & 0xFF;	
				ret[18]=(buffer_size>>8 ) & 0xFF;	
				ret[19]=(buffer_size>>0 ) & 0xFF;
				
				for(int i=0; i<buffer_size; i++)
					ret[20+i]=buf[i];

				return ret;
	}
	size_t Stream::getFrameSize(){ 
			return (header+buffer_size);
	}
			


