#include "controler.h"

Controler Controler::controler;
Controler::Controler():playlist(nullptr), 
				current_file_index(0),
				current_sample_index(0),
				play_file(false),
				auto_play(false){}

void Controler::setPlayList(shared_ptr<PlayList> p){
		playlist=p;
}

void Controler::setPlayList(PlayList *p){
		playlist=shared_ptr<PlayList>(p);

}

unsigned int Controler::size(){ 
			if(playlist.get()!=nullptr){
				return playlist->size();
			}
}
void Controler::play(){ play_file=true;}
void Controler::stop(){ play_file=false;}
void Controler::play(bool p){ play_file=p;}
bool Controler::isPlaying(){ return play_file;}
void Controler::setAutoPlay(bool p){ auto_play=p;}
int  Controler::getPlayListSize(){
		if(playlist.get()!=nullptr){
			return playlist->size();
		}else{
			return 0;
		}
}
void Controler::setCurrentFileIndex(unsigned int w){ current_file_index=w;}
unsigned int Controler::getCurrentFileIndex(){ return current_file_index;}
void Controler::setCurrentSampleIndex(unsigned int w){ current_sample_index=w;}
unsigned int Controler::getCurrentSampleIndex(){ return current_sample_index;}

unsigned int Controler::getAudioSize(unsigned int f){
		unsigned int ret=0;
		if(playlist.get()!=nullptr){
			if(f<playlist->size()){
				ret= playlist->getAudioSize(f);
			}
		}
		return ret;
}

std::string Controler::getAudioFileName(unsigned int i){
		if(playlist.get()!=nullptr){
			return playlist->getAudioFileName(i);
		}
			return "";


}

void Controler::addFile(std::string path){
		if(playlist.get()!=nullptr){
			playlist->addFile(path);
		}
}


Stream* Controler::getCurrentStream(){
			Stream* stream=nullptr;
			while(stream==nullptr){
				if(play_file==true){
					if(current_file_index<playlist->size()){
						if(current_sample_index<playlist->at(current_file_index)->size()){
							stream=playlist->at(current_file_index)->getStream(current_sample_index);
							current_sample_index++;
						}else if(auto_play){
							current_file_index++;
							current_sample_index=0;
						}else{
						//	std::cerr<<"Waiting after file ended"<<std::endl;
							play_file=false;
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}else if(auto_play){
						current_file_index=0;
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



