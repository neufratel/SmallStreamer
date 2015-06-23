#ifndef STRIMER_H
#define STRIMER_H

#include <string>
#include <iostream>
class Stream{
	private:
		int channels;
		int encoding;
		long rate;
		std::string buffer;
	public:
		int getChannels(){return channels;}
		int getEncoding(){return encoding;}
		long getRate(){return rate;}
		char* getBuffer(){return buffer.c_str();}
	Stream(int ch, int enc , int ra, char* buf, size_t size)
		:channels(ch),
		encoding(enc),
		rate(ra),
		buffer(buf, size){
			
		}
	Stream(char * data, int size){
				int tab[3];
				int j=0;
				std::string schan, senc, srate;
				for(int i=0; i<size; i++){
					if(data[i]=='\n'&&j<3){
						tab[j]=i;
						j++;
					}
				}
				channels=std::stoi(data[0]);
				encoding=std::stoi(data[tab[0]+1]);
				rate=std::stol(data[tab[1]+1]);
				buffer=std::string(data[tab[2]+1], size-tab[2]);
	}
	void print(){
		std::cout<<"chanels ="<<channels<<" encoding ="<<encoding<<" rate ="<<rate<<std::endl;
		std::cout<<buffer<<std::endl;
	}
	std::string getStreamDatagram(){
		std::string datagram;
			datagram=std::to_string(channels)+ "\n"+ std::to_string(encoding)+"\n"+std::to_string(rate)+"\n"+buffer;
		return datagram;
	}

};
#endif
