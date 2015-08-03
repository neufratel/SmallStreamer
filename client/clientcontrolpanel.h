#ifndef __CLIENT_CONTROL_PANEL_H__
#define __CLIENT_CONTROL_PANEL_H__

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QTimer>
#include <QtWidgets/QPushButton>
#include <QSize>
#include <QPoint>

#include <list>

class ServerDescription{
	public:
		std::string ip;
		std::string port;
		std::string name;
		ServerDescription(std::string n, std::string i, std::string p):
			name(n), ip(i), port(p)		
		{
			
		}
		ServerDescription():name("default"), ip("localhost"), port("5555"){}		

};


class ClientControlPanel : public QWidget
{
    Q_OBJECT
 	QListWidget list;
	QPushButton button_add;
	static const QSize B_ADD_SIZE=QSize(20,20);
	static const QPoint B_ADD_POINT=QPoint(0,20);
	std::list<ServerDescription> server_list;

	virtual void resizeEvent(QResizeEvent* event);
public:
    explicit ClientControlPanel(QWidget *parent = 0);
signals:

public slots:
	void add_server();
	void doubleClicked();
    
};

#endif // __CLIENT_CONTROL_PANEL_H__
