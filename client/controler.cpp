#include "controler.h"

Controler Controler::controler;
std::recursive_mutex Controler::mutex;
Controler::Controler():playlist(nullptr), 
				current_file_index(0),
				current_sample_index(0),
				play_file(false),
				auto_play(false){}

void Controler::setPlayList(shared_ptr<PlayList> p){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		playlist=p;
}

void Controler::setPlayList(PlayList *p){
		playlist=shared_ptr<PlayList>(p);

}

std::string Controler::getAudioFileTime(unsigned int file_, unsigned int sample_){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(current_file_index<playlist->size()){
		

	}else{
		return "00:00/00:00";
	}

}
unsigned int Controler::size(){
			std::lock_guard<std::recursive_mutex> lock(mutex);
			if(playlist.get()!=nullptr){
				return playlist->size();
			}
}
void Controler::play(){
 		std::lock_guard<std::recursive_mutex> lock(mutex);
		play_file=true;

}

void Controler::stop(){ 
		std::lock_guard<std::recursive_mutex> lock(mutex);
		play_file=false;
}

void Controler::play(bool p){ 
		std::lock_guard<std::recursive_mutex> lock(mutex);
		play_file=p;
}

bool Controler::isPlaying(){ 
		std::lock_guard<std::recursive_mutex> lock(mutex);
		return play_file;
}

void Controler::setAutoPlay(bool p){ 
		std::lock_guard<std::recursive_mutex> lock(mutex);
		auto_play=p;
}

int  Controler::getPlayListSize(){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		if(playlist.get()!=nullptr){
			return playlist->size();
		}else{
			return 0;
		}
}

void Controler::setCurrentFileIndex(unsigned int w){ 
			std::lock_guard<std::recursive_mutex> lock(mutex);
			playlist->at(current_file_index)->release();
			current_file_index=w;
			current_sample_index=0;
}

unsigned int Controler::getCurrentFileIndex(){ 
			std::lock_guard<std::recursive_mutex> lock(mutex);
			return current_file_index;
}

void Controler::setCurrentSampleIndex(unsigned int w){ 
			std::lock_guard<std::recursive_mutex> lock(mutex);			
			std::cerr<<"curr: "<<current_sample_index<<" new: "<<w<<std::endl;
			current_sample_index=w;

}

unsigned int Controler::getCurrentSampleIndex(){ 
		std::lock_guard<std::recursive_mutex> lock(mutex);
		return current_sample_index;
}

void Controler::nextFile(){
			std::lock_guard<std::recursive_mutex> lock(mutex);
			if(playlist->size()!=0&&playlist->size()>getCurrentFileIndex()){
				playlist->at(current_file_index)->release();
			
				setCurrentSampleIndex(0);
				current_file_index++;
			
				if(current_file_index>=playlist->size()){
						current_file_index=0;
				}
			}
}
void Controler::prevFile(){
			std::lock_guard<std::recursive_mutex> lock(mutex);
			if(playlist->size()!=0){
				playlist->at(current_file_index)->release();
				current_file_index--;
				current_sample_index=0;
				if(current_file_index>=playlist->size()){
					current_file_index=playlist->size()-1;
				}
			}
}


unsigned int Controler::getAudioSize(unsigned int f){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		unsigned int ret=0;
		if(playlist.get()!=nullptr){
			if(f<playlist->size()){
				ret= playlist->getAudioSize(f);
			}
		}
		return ret;
}	

std::string Controler::getAudioFileName(unsigned int i){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		if(playlist.get()!=nullptr&&playlist->size()>i){
			return playlist->getAudioFileName(i);
		}
			return "";


}

void Controler::addFile(std::string path){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		if(playlist.get()!=nullptr){
			playlist->addFile(path);
		}
}


Stream* Controler::getCurrentStream(){
			Stream* stream=nullptr;
			while(stream==nullptr){
				if(play_file==true){
					if(getCurrentFileIndex()<playlist->size()){
						if(getCurrentSampleIndex()<playlist->at(current_file_index)->size()){
							stream=playlist->at(getCurrentFileIndex())->getStream(getCurrentSampleIndex());
							setCurrentSampleIndex(getCurrentSampleIndex()+1);
						}else if(auto_play){
							std::cerr<<"Next"<<current_sample_index<<" "<<playlist->at(current_file_index)->size()<<std::endl;
							nextFile();
						}else{
							std::cerr<<"Waiting after file ended"<<std::endl;
							play_file=false;
							setCurrentSampleIndex(0);
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}else if(auto_play){
						std::cerr<<"AutoPlay afer playlist ended"<<std::endl;
						setCurrentFileIndex(0);
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

std::string Controler::convertTimeMsToString(int ms){
			std::lock_guard<std::recursive_mutex> lock(mutex);
			std::stringstream ss;
			ss<<(int)ms/60000<<":";
			int var=(ms%60000)/1000;
			if(var<10){ss<<0<<var;}
			else{ ss<<var;}
			ss.flush();
			return std::string(ss.str());
		}
std::string Controler::getCurrentPositionTime(){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		if(current_file_index<playlist->size()){
			return convertTimeMsToString(playlist->at(current_file_index)->getSliceDuration()*current_sample_index);
		}
}
std::string Controler::getCurrentFileTime(){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		if(current_file_index<playlist->size()){
			return convertTimeMsToString(playlist->at(current_file_index)->getSliceDuration()*playlist->at(current_file_index)->size());
		}
		
			return "00:00";
}






