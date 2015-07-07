#include <QApplication>
#include "streamcreator.h"



int main(int argc, char *argv[])
{
   
	 QCoreApplication a(argc, argv);
    if(argc < 2)
        exit(0);

    StreamCreator player;
	player.openFile(argv);
	player.play();

   return a.exec();
}
