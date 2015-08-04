#ifndef __CONTROLER_H_
#define __CONTROLER_H_
#include <memory>
#include <mutex>
#include <list>
#include "playlist.h"
#include "stream.h"
	


	using namespace std;
class Controler{
		static Controler controler;
		static std::recursive_mutex mutex;
		shared_ptr<PlayList> playlist;
		list<shared_ptr<PlayList>> playlist_container;
		unsigned int current_file_index;
		unsigned int current_sample_index;
		unsigned int current_playlist_index;
		bool play_file;
		bool auto_play;
		Controler();	
		Controler(const Controler&)=delete;
		Controler& operator=(const Controler&) = delete;
		std::string convertTimeMsToString(int ms);
		shared_ptr<PlayList>& getPlayList();
	
	public:
		static Controler& getControl(){
			std::lock_guard<std::recursive_mutex> lock(mutex);
			return controler;
		}
		void setPlayList(shared_ptr<PlayList> p);
		void setPlayList(PlayList *p);
		void play();
		void stop();
		void play(bool p);
		bool isPlaying();
		void setAutoPlay(bool p);
		int getPlayListSize();
		void setCurrentFileIndex(unsigned int f);
		unsigned int getCurrentFileIndex();
		void setCurrentSampleIndex(unsigned int s);
		unsigned int getCurrentSampleIndex();
		unsigned int getAudioSize(unsigned int f);
		std::string getAudioFileName(unsigned int i);
		std::string getAudioFileTime(unsigned int file_, unsigned int sample_);
		std::string getCurrentPositionTime();
		std::string getCurrentFileTime();
		void addFile(std::string path);
		unsigned int size();
		Stream* getCurrentStream();
		void nextFile();
		void prevFile();
		void setCurrentPlayListIndex(unsigned int idx);
		unsigned int getCurrentPlayListIndex();
};

#endif
