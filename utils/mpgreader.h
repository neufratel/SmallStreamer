#ifndef __MPG_READER__H
#define __MPG_READER__H
#include <mpg123.h>
#include <vector>
#include "stream.h"
#include <thread>
#include <string>
class MpgReader{
	private:
		static const short BITS=8;
		mpg123_handle *mh;
	    	unsigned char *buffer;
	    	size_t buffer_size;
		size_t done;
	    	int err;
		int channels, encoding;
	    	long rate;
		int byte_rate;
		int8_t volume;
		bool is_running;
		bool is_counting;
		std::thread thread;
		std::thread thread_count;


		void threadedRead(std::string file,std::vector<Stream>* vec, bool *fin){
			thread.detach();
			openFile(file);
			readFile(vec);
			*fin=true;
		}
		
		void threadedCount(std::string file, int* size, int *b_size, long *rate_, short* b_rate, bool *fin){
			thread_count.detach();
			openFile(file);
			*rate_=rate;
			*b_rate=(mpg123_encsize(encoding) * BITS);
			countFile(size, b_size);
			*fin=true;
		}
		
	public:
		MpgReader(): is_running(false), is_counting(false){
			/* initializations */
			mpg123_init();
			mh = mpg123_new(NULL, &err);
			mpg123_volume 	( mh,1.0) ;	
	//		buffer_size = mpg123_outblock(mh);
			buffer_size = 32768;
			buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
		}
		void asyncReadFile(std::string file, std::vector<Stream> *vec, bool *finished){
			if(!is_running){
				thread= std::thread(&MpgReader::threadedRead,this,file,vec, finished);
				is_running=true;
			}
		}
		void asyncCountFile(std::string file, int *size_, int *b_size_, long *rate_, short *b_, bool *finished){
			if(!is_counting){
				thread_count= std::thread(&MpgReader::threadedCount, this, file, size_, b_size_, rate_,b_, finished);
				is_counting=true;
			}
		} 
		

		void openFile(std::string file){
			/* open the file and get the decoding format */
			mpg123_open(mh, file.c_str());
			mpg123_getformat(mh, &rate, &channels, &encoding);
			/* set the output format and open the output device */
			
		}
		void readFile(std::vector<Stream> &vec){
			byte_rate=(mpg123_encsize(encoding) * BITS);
			
			while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
				Stream stream(channels, byte_rate, rate, buffer_size, buffer);
				vec.push_back(stream);
			}
		}

		void readFile(std::vector<Stream> *vec){
			byte_rate=(mpg123_encsize(encoding) * BITS);
			
			
			while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
				Stream stream(channels, byte_rate, rate, buffer_size, buffer);
				vec->push_back(stream);
			}
		}
		void countFile(int *size, int* b_size_){
				*size=0;
			while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
				*size=*size+1;
			}
			*b_size_=buffer_size;
		}	
		~MpgReader(){
			/* clean up */
			free(buffer);
			mpg123_close(mh);
			mpg123_delete(mh);
			mpg123_exit();
	}
};

#endif
