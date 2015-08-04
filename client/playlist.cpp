#include "playlist.h"
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
#include "audiofile.h"
	using namespace std;
	
 
	std::string PlayList::getAudioFileName(unsigned int i){
			if(i<list.size()){
				return list[i]->getName();
			}else{
				return "File not exists";
			}
	}
	


		PlayList::PlayList(): name("default"){
		}
		PlayList::PlayList(std::string name_): name(name_){
		}	

		void PlayList::addFile(std::string file){
			list.push_back(new AudioFile(file));
		}
	
	unsigned int PlayList::getAudioSize(unsigned int idx){
			unsigned int size=0;
			if(idx<list.size()){
				size=list[idx]->size();
			}
			return size;
	}
	
	AudioFile* PlayList::at(unsigned int i){ return list.at(i);}
		
void PlayList::show(){
			system("clear");
			std::cout<<std::endl;
			for(int i=0; i<list.size(); i++){
				std::cout<<i<<":\t";
				list[i]->print();
			}
		}
PlayList::~PlayList(){
			for(AudioFile* file : list){
				delete file;
			}
		}

int PlayList::size(){
			return list.size();
}

std::string PlayList::getPlayListName(){
	return name;
}
