#ifndef __CLIENT_CONTROL_PANEL_H__
#define __CLIENT_CONTROL_PANEL_H__

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QTimer>
#include <QtWidgets/QPushButton>
#include <QSize>
#include <QPoint>
#include <QLabel>
#include <memory>
#include <list>
#include "clientcontroler.h"
class ServerDescriptionItem: public ServerDescription, public QListWidgetItem{
	public:
		ServerDescriptionItem(std::string n, std::string i, std::string p, QListWidget* widget):
			QListWidgetItem(n.c_str(), widget), ServerDescription(n , i ,p)
		{
			
		}
		ServerDescriptionItem(ServerDescription* sd, QListWidget* widget) 
			:QListWidgetItem(sd->name.c_str(), widget),
			 ServerDescription(sd->name, sd->ip, sd->port){
		}
		ServerDescriptionItem(): ServerDescription("default","localhost","5555"){}		

};

class ClientControlPanel : public QWidget
{
    Q_OBJECT
 	QListWidget list;
	QTimer timer;
	QPushButton button_add;
	QPushButton button_remove;
	QPushButton* button_ok;
	std::shared_ptr<QWidget> popup;
	QPlainTextEdit* text_name;
	QPlainTextEdit* text_ip;
	QPlainTextEdit* text_port;
	QLabel *info;
	static const QSize B_ADD_SIZE=QSize(35,35);
	static const QPoint B_ADD_POINT=QPoint(0,37);
	std::list<ServerDescription> server_list;

	virtual void resizeEvent(QResizeEvent* event);
public:
    explicit ClientControlPanel(QWidget *parent = 0);
signals:

public slots:
	void add_server();
	void window_popup();
	void doubleClicked();
 	void selected();
	void remove_server(); 
};

#endif // __CLIENT_CONTROL_PANEL_H__
