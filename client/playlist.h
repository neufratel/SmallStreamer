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
#include <memory>
#include "audiofile.h"

/*Singleton*/
class PlayList{
	 std::vector<AudioFile*> list;
	 
		
		
	public:
		PlayList();
		~PlayList();
		void addFile(std::string file);
		//Stream* getCurrentStream();
		void show();
		int size();
		//void showCurrentAudioInfo();
		
		
		unsigned int getCurrentAudioSize();
		unsigned int getAudioSize(unsigned int il);
		std::string getAudioFileName(unsigned int i);
		bool isPlaying();
		void setAutoPlay(bool b);
		AudioFile* at(unsigned int);
};
#endif
