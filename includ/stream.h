#ifndef STRIMER_H
#define STRIMER_H

#include <string>
#include <iostream>
class Stream{
	private:
		int channels;
		int encoding;
		long rate;
		unsigned char * chbuf;
		int size;
	public:
		int getChannels(){return channels;}
		int getEncoding(){return encoding;}
		long getRate(){return rate;}
		unsigned char* getBuffer(){return chbuf;}
		int getSize(){return size;}
	~Stream(){
		delete[] chbuf;	
	}
	Stream(int ch, int enc , int ra, unsigned char* buf, size_t si)
		:channels(ch),
		encoding(enc),
		rate(ra),
		size(si){
			
			std::cout<<"Stream(int ch, int enc , int ra, unsigned char* buf, size_t si)"<<std::endl;
			std::cout<<"ch "<<channels<<" e "<<encoding<<" r "<<rate<<" s "<<size<<std::endl;
			
			chbuf=new unsigned char[size];
			for(int i=0; i<size; i++){
					chbuf[i]=buf[i];
					
				}
			
		}
	Stream(unsigned char * data, int si) : size(si){
				
				channels=2;
				encoding=208;
				rate=44100;
				
				chbuf= new unsigned char[size];
				std::cout<<"ch "<<channels<<" e "<<encoding<<" r "<<rate<<" s "<<size<<std::endl;
				for(int i=0; i<size; i++){
					chbuf[i]=data[i];
					
				}
				
	}
	
	void print(){
		std::cout<<"chanels ="<<channels<<" encoding ="<<encoding<<" rate ="<<rate<<std::endl;
		for(int i=0; i<size; i++){
			std::cout<<(int)chbuf[i];
		}
			std::cout<<std::endl;
	}
	unsigned char * getStreamDatagram(){
		unsigned char* ret =new unsigned char[size];
		std::cout<<"Size "<<size<<std::endl;
		for(int i=0; i<size; i++){
			ret[i]=chbuf[i];		
		}
	//	for(int i=0; i<size; i++){
	//		std::cout<<ret[i];
	//	}
	//		std::cout<<std::endl;
		return ret;
	}

};
#endif
