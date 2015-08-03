#ifndef __CLIENT_CONTROLER_H__
#define __CLIENT_CONTROLER_H__

#include <mutex>
#include "client.h"

using namespace std;
class ClientControler{
		static ClientControler controler;
		std::recursive_mutex mutex;
		Client *client;
		
		ClientControler();	
		ClientControler(const ClientControler&)=delete;
		ClientControler& operator=(const ClientControler&) = delete;

	public:
		static ClientControler& getControl(){
			
			return controler;
		}
		void setClient(Client * cl_);
		void setNewServer(std::string server, std::string port);
		void streamData(bool stream);
};

#endif
