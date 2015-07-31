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
	


		PlayList::PlayList(){
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
/*		Stream* PlayList::getCurrentStream(){
			Stream* s=nullptr;
			if(current_file<list.size()){
				if(current_position<list[current_file]->size()){
					s= list[current_file]->getStream(current_position);
					showCurrentAudioInfo();
					current_position++;
				}else{
					list[current_file]->release();
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
		}*/
/*
		Stream* PlayList::getCurrentStream(){
			Stream* stream=nullptr;
			while(stream==nullptr){
				if(play_file==true){
					if(current_file<list.size()){
						if(curtrent_position<list[current_file]->size()){
							stream=list[current_file]->getStream(current_position);
							current_position++;
						}else if(auto_play){
							current_file++;
							current_position=0;
						}else{
						//	std::cerr<<"Waiting after file ended"<<std::endl;
							play_file=false;
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}else if(auto_play){
						current_file=0;
					}else{
						std::cerr<<"playlist ended"<<std::endl;
					}
				}else{
	//				cerr<<"PLAYFILE:play_file==false"<<std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			}
			return stream;
		}
*/




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
/*
		void PlayList::showCurrentAudioInfo(){
			std::string name_=list[current_file]->getName();
			while(name_.length()<20) name_+="_";
			std::string tile=name_.substr(0,19);
			std::stringstream ss;
			ss<<"\rTrack :";
			if(current_file>99){
				ss<<current_file;
			}else if(current_file>9){
				ss<<" "<<current_file;
			}else{
				ss<<"  "<<current_file;
			}
			ss<<"    "<<tile<<"  " <<list[current_file]->getCurrentPosition(current_position)
				  <<"/"<<list[current_file]->getAudioLengthString()<<"				"<<std::flush;
			std::cout<<ss.str()<<std::flush;
		}*/


