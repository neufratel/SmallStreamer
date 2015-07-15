#ifndef SINGLETON_H
#define SINGLETON_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdio.h>
class Logger {
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}
	void msg(const std::string& m) {
		++counter_;
		time_t rawtime;
  		struct tm * timeinfo;
  		char buffer[80];

  		std::time (&rawtime);
  		timeinfo = std::localtime(&rawtime);

  		std::strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
  		std::string str(buffer);
		
		std::ofstream logfile;
		logfile.open ("log.txt", std::ios::out | std::ios::app | std::ios::binary); 
		logfile<<str<<":"<<counter_<<"\t"<<m<<std::endl;
		logfile.close();
	}
private:
	Logger() : counter_(0) {}
	Logger(const Logger& ) = delete;
	Logger& operator=(const Logger&) = delete;
	~Logger() {}

	int counter_; //licznik wiadomości
};

#endif //SINGLETON_H
