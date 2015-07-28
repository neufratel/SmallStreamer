#ifndef __PLAYLIST__H
#define __PLAYLIST__H
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <sstream>
#include <boost/format.hpp>
#include "stream.h"
#include "mpgreader.h"
#include <cstdlib>
class AudioFile{
	private:
		std::string name;
		std::string path;
		std::string type;
		bool is_loaded;
		bool is_loading;
		std::vector<Stream> file_sample;
		
		MpgReader *reader;
	public:
		AudioFile(std::string fa) throw (std::string) :is_loaded(false), is_loading(false) {
			size_t index= fa.find_last_of('/');
			size_t index_dot= fa.find_last_of('.');
			if(index_dot<0|| index_dot>fa.length() ) throw std::string("Unknown File Type")+fa;

			if(index<0 || index > fa.length()) {index=-1;}
			index++;
			name=fa.substr(index);
			path=fa.substr(0, index);
			type=fa.substr(index_dot);
			reader=nullptr;
			if(type.compare(".mp3") && type.compare(".mpga") ) throw std::string("Unknown File type descrition: ")+type;
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
					showCurrentAudioInfo();
					current_position++;
				}else{
					current_position = 0;
					current_file++;
					show();
					std::cout<<"Selcect song"<<std::endl<<":"<<std::flush;
					std::cin>>current_file;
					s= getCurrentStream();
				}
				
			}else{
				current_file=0;
				current_position=0;
				show();
				std::cout<<"Selcect song"<<std::endl<<":"<<std::flush;
				std::cin>>current_file;
				s= getCurrentStream();
			}
				return s;
		}
		void show(){
			system("clear");
			std::cout<<std::endl;
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
		void showCurrentAudioInfo(){
		/*	std::string name_=list[current_position]->getName();
			std::cout<<boost::format("\rTrack:%|3d| \t  %|.10s| %|-s|/%|.5s|" ) 
					%current_file
					%(boost::io::group(std::setfill('_'), std::setw(30),1)a)
					%list[current_file]->getCurrentPosition(current_position)
					%list[current_file]->getAudioLengthString(); 
			std::cout<<std::flush;*/
			//std::cout<<format;
			 //std::cout << boost::format("%|1$1| %|2$3|") % "Hello" % 3 << std::endl;
			std::cout<<"\r Track: "<<current_file<<"\t"<<list[current_file]->getName()<<"\t\t"
						<<list[current_file]->getCurrentPosition(current_position)<<"/"<<list[current_file]->getAudioLengthString()<<std::flush;
		}

};
#endif
