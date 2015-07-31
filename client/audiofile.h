#ifndef __AUDIOFILE__H
#define __AUDIOFILE__H
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <sstream>
#include <boost/format.hpp>
#include "stream.h"
#include "mpgreader.h"
#include <cstdlib>
#include <memory>
class AudioFile{
	private:
		std::string name;
		std::string path;
		std::string type;
		long  rate;
		int buffer_size;
		short byte_rate;
		bool is_loaded;
		bool is_loading;
		bool is_counted;
		bool is_counting;
		long long sample_duration_us;
		int  file_sample_size;
		std::vector<Stream> file_sample;
		
		std::shared_ptr<MpgReader> reader;
		std::shared_ptr<MpgReader> counter;

	public:
		AudioFile(std::string fa) throw (std::string) :is_loaded(false), is_loading(false),
			is_counted(false), is_counting(false), reader(nullptr), counter(nullptr){

			size_t index= fa.find_last_of('/');
			size_t index_dot= fa.find_last_of('.');
			if(index<0 || index > fa.length()){index=0;}
			if(index_dot<0|| index_dot>fa.length() || index>index_dot){
					 std::cerr<<index<<"  "<<index_dot<<"  "<<fa.length()<<std::endl;
					 throw std::string("Unknown File Type ")+fa;
			}
			
			name=fa.substr(index, index_dot-index);
			path=fa.substr(0, index);
			type=fa.substr(index_dot);
			reader=nullptr;
			if(type.compare(".mp3") && type.compare(".mpga") ) throw std::string("Unknown File type descrition: ")+type;

			count();
		}
		int size(){ 
			if(is_loaded){
				//std::cerr<<"size"<<file_sample.size()<<std::endl;
				return file_sample.size();
			}else if(is_counted){
			//std::cerr<<"size"<<file_sample_size<<std::endl;
				return file_sample_size;
			}else{
				return 0;
			}
		}
		Stream* getStream(int cnt){
			load();
			while(file_sample.size()==0){
					std::cout<<"loading :"<<name<<std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(10));;
			}
			if(cnt<file_sample.size()){
				return &file_sample[cnt];
			}else{
				return nullptr;
			}
		}
		void print(){
			std::string name_t(name);
			std::string path_t(path);
			while(name_t.length()<30) name_t+=" ";
			while(path_t.length()<20) path_t+=" ";
			if(is_counting||is_counted){
				std::cout<<"name: " <<name_t.substr(0,30)<<"\t"<<path_t.substr(0,20)<<"\t\t"<<getAudioLengthString()<<std::endl;
			}else{
				count();
				std::cout<<"name: " <<name_t.substr(0,30)<<"\t"<<path_t.substr(0,20)<<"\t\t"<<std::endl;
			}
		}
		void load(){
			if(!is_loaded&&!is_loading){
				is_loading=true;
				reader=std::shared_ptr<MpgReader>(new MpgReader());
				reader->asyncReadFile(path+name+type,&file_sample, &is_loaded);
			}
		}
		void release(){
			file_sample.clear();
			is_loaded=false;
			is_loading=false;
		}
		void count(){
			if(!is_counted&&!is_counting){
				is_counting=true;
				counter=std::shared_ptr<MpgReader>(new MpgReader());
				counter->asyncCountFile(path+name+type,&file_sample_size,&buffer_size, &rate, &byte_rate, &is_counted);
			}

		}
		bool isLoaded(){
			return is_loaded;
		}
		int getAudioDuration(){
			if(is_counted){
				return size()*getSliceDuration();
			}else{
				return 0;
			}
		}
		int getSliceDuration(){
			if(is_counted){
				long long bytes_per_sample=((16*2)/8);
                long long bytes_in_slice=buffer_size;
                long long rate_ll=rate;
				std::cerr<<"Slice dur "<<bytes_in_slice<<"  "<<bytes_per_sample<<" "<<rate_ll<<(int)is_counted<<(int)is_loaded<<std::endl;
                return (1000*bytes_in_slice/(bytes_per_sample))/rate_ll;
			}else{
				return 0;
			}
		}
		~AudioFile(){
		}
		std::string getAudioLengthString(){
			return convertPositionToString(getAudioDuration());
		}
		std::string convertPositionToString(int ms){
			std::stringstream ss;
			ss<<(int)ms/60000<<":";
			int var=(ms%60000)/1000;
			if(var<10){ss<<0<<var;}
			else{ ss<<var;}
			ss.flush();
			return std::string(ss.str());
		}
		std::string getCurrentPosition(int position){
			return  convertPositionToString(position*getSliceDuration());
		}
		std::string getName(){ return name;}
		
		
};

#endif
