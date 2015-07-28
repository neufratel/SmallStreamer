#ifndef __PLAYLIST__H
#define __PLAYLIST__H
#include <string>
#include <chrono>
#include <vector>
#include <list>
#include <sstream>
#include "stream.h"
#include "mpgreader.h"

class AudioFile{
	private:
		std::string name;
		std::string path;
		bool is_loaded;
		bool is_loading;
		std::vector<Stream> file_sample;
		
		MpgReader *reader;
	public:
		AudioFile(std::string fa) :is_loaded(false), is_loading(false) {
			size_t index= fa.find_last_of('/');
			if(index<0 || index > fa.length()) {index=-1;}
			index++;
			name=fa.substr(index);
			path=fa.substr(0, index);
			reader=nullptr;
		}
		int size(){ 
			load();
			return file_sample.size();
		}
		Stream* getStream(int cnt){
			load();
			if(cnt<size()){
				return &file_sample[cnt];
			}else{
				return nullptr;
			}
		}
		void print(){
			if(is_loaded){
				std::cout<<"name: " <<name<<"\t"<<path<<"\t\t"<<getAudioLengthString()<<std::endl;
			}else{
				load();
				std::cout<<"name: " <<name<<"\t"<<path<<"\t\t"<<std::endl;
			}
		}
		void load(){
			if(!is_loaded&&!is_loading){
				is_loading=true;
				reader=new MpgReader();
				reader->asyncReadFile(path+name,&file_sample, &is_loaded);
			}
		}
		bool isLoaded(){
			return is_loaded;
		}
		int getAudioDuration(){
			if(is_loaded){
				return size()*getSliceDuration();
			}else{
				return 0;
			}
		}
		int getSliceDuration(){
			if(is_loaded){
				return file_sample.front().getStreamDuration();
			}else{
				return 0;
			}
		}
		~AudioFile(){
			delete reader;
		}
		std::string getAudioLengthString(){
			std::stringstream ss;
			ss<<(int)getAudioDuration()/60000<<":"<<(getAudioDuration()%60000)/1000;
			ss.flush();
			return std::string(ss.str());
		}
		
};

class PlayList{
	std::vector<AudioFile*> list;
	unsigned int current_file;
	int current_position;
	public:
		PlayList(){
			current_position=0;
			current_file=0;
		}
		void addFile(std::string file){
			list.push_back(new AudioFile(file));
		}
		Stream* getCurrentStream(){
			Stream* s=nullptr;
			if(current_file<list.size()){
				if(current_position<list[current_file]->size()){
					s= list[current_file]->getStream(current_position);
					current_position++;
				}else{
					current_position = 0;
					current_file++;
					show();
					std::cout<<"Selcect song"<<std::endl;
					std::cin>>current_file;
					s= nullptr;
				}
				
			}else{
				current_file=0;
				current_position=0;
	//			std::cout<<"end of playlist"<<std::endl;
				s= nullptr;
			}
				return s;
		}
		void show(){
			for(int i=0; i<list.size(); i++){
				std::cout<<i<<":\t";
				list[i]->print();
			}
		}
		~PlayList(){
			for(AudioFile* file : list){
				delete file;
			}
		}
		int size(){
			return list.size();
		}

};

#endif
