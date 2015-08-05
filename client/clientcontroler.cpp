
#include "clientcontroler.h"
#include <iterator>
using namespace std;
ClientControler ClientControler::controler;

ClientControler::ClientControler(): client(nullptr),mutex(),
	current_server_index(0) {
	server_list.push_back(shared_ptr<ServerDescription>(new ServerDescription("THIS", "localhost", "5555")));
}

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

void ClientControler::setCurrentServerIndex(unsigned int idx){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(idx<server_list.size()){
		current_server_index=idx;
		//TODO changing server for client 
	}else{
		setCurrentServerIndex(idx-1);
	}
}

unsigned int ClientControler::getCurrentServerIndex(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return current_server_index;
}

void ClientControler::addServerDescription(string name, string ip, string port){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	server_list.push_back(shared_ptr<ServerDescription>(new ServerDescription(name, ip, port)));
}

ServerDescription* ClientControler::getServerDescription(unsigned int idx){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	auto it=server_list.begin();
	if(idx<server_list.size()){
		advance(it, idx);
	}
	return (it->get());
}
	
shared_ptr<ServerDescription> ClientControler::getServerDescriptionSPTR(unsigned int idx){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	auto it=server_list.begin();
	if(idx<server_list.size()){
		advance(it, idx);
	}
	return (*it);
}

int ClientControler::getServerListSize(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return server_list.size();
}
void ClientControler::removeServerDescription(unsigned int idx){
		std::lock_guard<std::recursive_mutex> lock(mutex);
		if(idx>0){
			auto it=server_list.begin();
			advance(it,idx);
			server_list.erase(it);
			setCurrentServerIndex(current_server_index);
		}
}
