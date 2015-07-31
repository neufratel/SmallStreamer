#include <iostream>
#include <cstring>
#include <stdint.h>
#include "stream.h"
#include <memory>

	using namespace std;

		Stream::~Stream(){
	/*	if(buf!=nullptr){
				std::cerr<<"~Stream() deleting buf"<<buf<<std::endl;
				delete[] buf;
				buf=nullptr;
			}*/


		}
		short Stream::global_volume=50;
		
		void Stream::setSampleClassVolume(int8_t vol){ 
				global_volume=vol;
		}
		short Stream::getSampleClassVolume(){ 
				return global_volume;
		}
		void Stream::print(){
			std::cout<<"V: "<<(int)volume<<" CH: "<<(int)channels<<" BRT: "<<(int)byte_rate<<" R: "<<(int)rate<<" size: "<<(int)buffer_size<<std::endl;
		}
	//	Stream::Stream(unsigned char*b, size_t len): buf(b), buffer_size(len){};
	//	Stream::Stream(){};
		Stream::Stream(unsigned char* init){
			
			channels=((int8_t)init[0]);
			volume=((int8_t)init[1]);
			byte_rate=((int8_t)init[2]);


			rate=	((long)init[12])<<24;
			rate+=	((long)init[13])<<16;
			rate+=	((long)init[14])<<8;
			rate+=	((long)init[15])<<0;

			
			buffer_size=((size_t)init[16])<<24;
			buffer_size+=((size_t)init[17])<<16;
			buffer_size+=((size_t)init[18])<<8;
			buffer_size+=((size_t)init[19])<<0;	
		}
		Stream::Stream(int chan, int br, long ra, size_t size, unsigned char * b):
			 channels(chan), byte_rate(br), rate(ra), buffer_size(size)
		{
				volume=Stream::global_volume;
				buf=shared_ptr<char>(new char[buffer_size]);
				memcpy ( buf.get(), b, buffer_size );
		};	
		void Stream::setData(unsigned char * b){
			/*	if(buf!=nullptr){
					delete[] buf;
				}*/
			
				buf=shared_ptr<char>(new char[buffer_size]);b;
				memcpy ( buf.get(), b, buffer_size );
			};
		shared_ptr<char> Stream::getFrame(){
				shared_ptr<char> ret(new unsigned char[header+buffer_size]);
				
				ret.get()[0]=channels;
				ret.get()[1]=global_volume;
				ret.get()[2]=byte_rate;
				ret.get()[8]=global_volume;
				ret.get()[12]=(rate>>24) & 0xFF;
				ret.get()[13]=(rate>>16) & 0xFF;
				ret.get()[14]=(rate>>8)  & 0xFF;
				ret.get()[15]=(rate) 	  & 0xFF;
				
				ret.get()[16]=(buffer_size>>24) & 0xFF;
				ret.get()[17]=(buffer_size>>16) & 0xFF;	
				ret.get()[18]=(buffer_size>>8 ) & 0xFF;	
				ret.get()[19]=(buffer_size>>0 ) & 0xFF;
				
				for(int i=0; i<buffer_size; i++)
					ret.get()[20+i]=buf.get()[i];

				return ret;
	}
	size_t Stream::getFrameSize(){ 
			return (header+buffer_size);
	}
	long long Stream::getStreamDuration(){
                                long long bytes_per_sample=((byte_rate*channels)/BITS);
                                long long bytes_in_slice=getBufferSize();
                                long long rate_ll=getRate();
                                return (1000*bytes_in_slice/(bytes_per_sample))/rate_ll;
                }
	long long Stream::getStreamDurationUS(){
                                long long bytes_per_sample=((byte_rate*channels)/BITS);
                                long long bytes_in_slice=getBufferSize();
                                long long rate_ll=getRate();
                                return (1000000*bytes_in_slice/(bytes_per_sample))/rate_ll;
                }			
			


