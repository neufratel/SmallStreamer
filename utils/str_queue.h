#ifndef STREAM_QUEUE_H
#define STREAM_QUEUE_H
#include <queue>
#include "stream.h"
#include <mutex>
#include <condition_variable>
class StreamQueue{
	std::queue<Stream> que;
	std::mutex mutex;
	std::condition_variable condition;
	std::condition_variable condition_full;
	static const int MAX_SIZE=2;
	bool not_empty;
	bool not_full;
    public:
	StreamQueue(): not_empty(false), not_full(true) {};
	void push(Stream s){
		std::unique_lock<std::mutex> locker(mutex);
		condition_full.wait(locker, [this]{return this->not_full;});
		que.push(s);
	//	std::cout<<"Quele :"<<que.size()<<std::endl;
		if(que.size()>=MAX_SIZE){
			not_full=false;
		}
		not_empty=true;
		condition.notify_one();
	}
	Stream& front(){
		std::unique_lock<std::mutex> locker(mutex);
		condition.wait(locker, [this]{return this->not_empty;});
		
		Stream &w= que.front();
		
		locker.unlock();
		return w;
	}
	void pop(){
		std::unique_lock<std::mutex> locker(mutex);
		condition.wait(locker, [this]{return this->not_empty;});
		que.pop();
		if(que.size()<=0){
			not_empty=false;
		}
		if(que.size()>=MAX_SIZE){
			not_full=false;
			std::cerr<<"Pop="<<std::endl;
		}else{
			not_full=true;
			condition_full.notify_one();
		
		}
		//std::cout<<"Quele :pop(): "<<que.size()<<std::endl;
	
		locker.unlock();
	}
	size_t size(){
		return que.size();
	}

};
#endif
