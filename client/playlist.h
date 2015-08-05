#ifndef __PLAYLIST__H
#define __PLAYLIST__H
#include <string>
#include <chrono>
#include <list>
#include <set>
#include <sstream>
#include <boost/format.hpp>
#include "stream.h"
#include "mpgreader.h"
#include <cstdlib>
#include <memory>
#include "audiofile.h"

/*Singleton*/
class PlayList{
	 std::list<AudioFile*> list;
	 std::string name; 
		
		
	public:
		PlayList();
		PlayList(std::string name_);
		~PlayList();
		void addFile(std::string file);
		//Stream* getCurrentStream();
		void show();
		int size();
		//void showCurrentAudioInfo();
		
		
		unsigned int getCurrentAudioSize();
		unsigned int getAudioSize(unsigned int il);
		std::string getAudioFileName(unsigned int i);
		std::string getPlayListName();
		bool isPlaying();
		void setAutoPlay(bool b);
		AudioFile* at(unsigned int);
		void removeAudioFile(unsigned int idx);
};
#endif
