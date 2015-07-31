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


class PlayList{
	 static std::vector<AudioFile*> list;
	 static unsigned int current_file;
	 static unsigned int current_position;
	 static bool auto_play;
	 static bool play_file;
	public:
		PlayList();
		static void addFile(std::string file);
		Stream* getCurrentStream();
		void show();
		~PlayList();
		static int size();
		void showCurrentAudioInfo();
		static unsigned int getCurrentPosition();
		static void setCurrentPosition(int p);
		static unsigned int getCurrentFileIndex();
		static void setCurrentFileIndex(int p);
		static void play();
		static void stop();
		static unsigned int getCurrentAudioSize();
		static std::string getAudioFileName(unsigned int i);
		static bool isPlaying();
		static void setAutoPlay(bool b);
};
#endif
