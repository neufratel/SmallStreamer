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
	bool not_empty;
    public:
	StreamQueue(): not_empty(false){};
	void push(Stream s){
		std::unique_lock<std::mutex> locker(mutex);
		que.push(s);
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
		locker.unlock();
	}
	size_t size(){
		return que.size();
	}

};
#endif
