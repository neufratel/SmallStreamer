#ifndef STREAM_QUEUE_H
#define STREAM_QUEUE_H
#include <queue>
#include "stream.h"
#include <mutex>
class StreamQueue{
	std::queue<Stream> que;
	std::mutex mutex;
    public:
	StreamQueue():mutex(){};
	void push(Stream s){
		mutex.lock();
		que.push(s);
		mutex.unlock();
	}
	Stream& front(){
		mutex.lock();
		Stream &w= que.front();
		mutex.unlock();
		return w;
	}
	void pop(){
		mutex.lock();
		que.pop();
		mutex.unlock();
	}
	size_t size(){
		return que.size();
	}

};
#endif
