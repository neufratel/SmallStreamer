#ifndef __CONTROLER_H_
#define __CONTROLER_H_
#include <memory>
#include "playlist.h"
#include "stream.h"

	


	using namespace std;
class Controler{
		static Controler controler;
		shared_ptr<PlayList> playlist;
		unsigned int current_file_index;
		unsigned int current_sample_index;
		bool play_file;
		bool auto_play;
		Controler();	
		Controler(const Controler&)=delete;
		Controler& operator=(const Controler&) = delete;
	
	public:
		static Controler& getControl(){
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
		void addFile(std::string path);
		unsigned int size();
		Stream* getCurrentStream();
		void nextFile();
		void prevFile();
};

#endif
