#ifndef KEEPER_H
#define KEEPER_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdio.h>
#include <memory>

#include "controler.h"
class Keeper {
public:
	static Keeper& getInstance() {
		static Keeper instance;
		return instance;
	}
	void save(){
		
		std::ofstream config;
		config.open ("client.conf", std::ios::out | std::ios::app | std::ios::binary); 
		for(int i=0; i<Controler::getControl().containerSize();i++){
			std::shared_ptr<PlayList> pl=std::shared_ptr<PlayList>(Controler::getControl().getPlayList(i));
			config<<"#"<<pl->getPlayListName()<<std::endl;
			for(int j=0; j<pl->size(); j++){
				config<<pl->at(j)->getFullPath()<<std::endl;
			}
		}
		config.close();
	}
private:
	Keeper() {}
	Keeper(const Keeper& ) = delete;
	Keeper& operator=(const Keeper&) = delete;
	~Keeper() {}
};

#endif //SINGLETON_H
