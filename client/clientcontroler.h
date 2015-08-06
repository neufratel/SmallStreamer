#ifndef __CLIENT_CONTROLER_H__
#define __CLIENT_CONTROLER_H__

#include <list>
#include <mutex>
#include <memory>
#include "client.h"

using namespace std;

class ServerDescription{
	public:
		std::string ip;
		std::string port;
		std::string name;
		ServerDescription(std::string n, std::string i, std::string p): name(n), ip(i), port(p){}
		ServerDescription():name("default"), ip("localhost"), port("5555"){}		
		ServerDescription(const ServerDescription& sd): name(sd.name), ip(sd.ip), port(sd.port){}
};

class ClientControler{
		std::list<shared_ptr<ServerDescription>> server_list;
		unsigned int current_server_index;
		static ClientControler controler;
		std::recursive_mutex mutex;
		Client *client;
		
		ClientControler();	
		ClientControler(const ClientControler&)=delete;
		ClientControler& operator=(const ClientControler&) = delete;
		void setNewServer(ServerDescription* sd);

	public:
		static ClientControler& getControl(){
			
			return controler;
		}
		void setClient(Client * cl_);
		void streamData(bool stream);
		void setCurrentServerIndex(unsigned int idx);
		unsigned int getCurrentServerIndex();
		void addServerDescription(string name, string ip, string port);
		ServerDescription* getServerDescription(unsigned int idx);		
		int getServerListSize();	
		shared_ptr<ServerDescription> getServerDescriptionSPTR(unsigned int idx);
		void removeServerDescription(unsigned int idx);

};

#endif
