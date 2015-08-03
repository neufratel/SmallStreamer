
#include "clientcontroler.h"

ClientControler ClientControler::controler;

ClientControler::ClientControler(): client(nullptr),mutex() {}

void ClientControler::setClient(Client * cl_){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		client=cl_;
}
void ClientControler::setNewServer(std::string server, std::string port){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(client!=nullptr){	
		client->setServer(server);
		client->setPort(port);
		client->setMaintainConnection(false);
	}else{
		std::cerr<<"Client not initialized inside controler try \"setClient(\""<<std::endl;
	}
}
void ClientControler::streamData(bool stream){
	std::lock_guard<std::recursive_mutex> lock(mutex);

}
