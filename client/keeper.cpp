
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
#include "clientcontroler.h"
#include "playlist.h"

#include "keeper.h"
const std::string Keeper::PLAYLIST="#PLAYLIST";
const std::string Keeper::PLAYLIST_END="#PLAYLIST_END";
const std::string Keeper::SERVER_NAME="#SERVER_NAME";
const std::string Keeper::SERVER_NAME_END="#SERVER_NAME_END";
	Keeper& Keeper::getInstance() {
		static Keeper instance;
		return instance;
	}
	void Keeper::open(){
		std::ifstream config;
			config.open("client.conf", std::ios::in);
			std::string line;
			std::shared_ptr<PlayList> playlist(nullptr);
			if(config.is_open()){
				while(!config.eof()){
					getline(config,line);
					if(!line.compare(PLAYLIST)){
							getline(config,line);
							playlist=std::shared_ptr<PlayList>(new PlayList(line));
							std::cerr<<line<<std::endl;
					}else if(!line.compare(PLAYLIST_END)){
							if(playlist.get()!=nullptr){
								std::cerr<<line<<std::endl;
								Controler::getControl().setPlayList(playlist);
								playlist=std::shared_ptr<PlayList>(nullptr);
							}
					}else if(!line.compare(SERVER_NAME)){
							std::string name, ip, port;
							getline(config, name);
							getline(config, ip);
							getline(config, port);
							ClientControler::getControl().addServerDescription(name, ip, port);
					}else{
							if(playlist.get()!=nullptr){
								playlist->addFile(line);
							}				
					}
				}
			}

	}
	void Keeper::save(){
		
		std::ofstream config;
		config.open ("client.conf", std::ios::out | std::ofstream::trunc | std::ios::binary); 
		for(int i=1; i<Controler::getControl().containerSize();i++){
			std::shared_ptr<PlayList> pl=std::shared_ptr<PlayList>(Controler::getControl().getPlayList(i));
			config<<PLAYLIST<<std::endl;
			config<<pl->getPlayListName()<<std::endl;
			for(int j=0; j<pl->size(); j++){
				config<<pl->at(j)->getFullPath()<<std::endl;
			}
			config<<PLAYLIST_END<<std::endl;
		}
		for(unsigned int i=1; i<ClientControler::getControl().getServerListSize(); i++){
			std::shared_ptr<ServerDescription> sd =ClientControler::getControl().getServerDescriptionSPTR(i);
			config<<SERVER_NAME<<std::endl;
			config<<sd->name<<std::endl;
			config<<sd->ip<<std::endl;
			config<<sd->port<<std::endl;
		}
		config.close();
	}

