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
	static const std::string PLAYLIST;
	static const std::string PLAYLIST_END;
	static const std::string SERVER_NAME;
	static const std::string SERVER_NAME_END;
	static Keeper& getInstance();
	void open();
	void save();
private:
	Keeper() {}
	Keeper(const Keeper& ) = delete;
	Keeper& operator=(const Keeper&) = delete;
	~Keeper() {}
};

#endif //SINGLETON_H
