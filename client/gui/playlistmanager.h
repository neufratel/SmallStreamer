#ifndef __PLAY_LIST_MANAGER_H__
#define __PLAY_LIST_MANAGER_H__

#include <memory>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QCheckBox>
#include <QTimer>
#include <QSize>
#include <QLabel>
#include <QPlainTextEdit>
#include "listwidget.h"
#include "playlistwidget.h"


class PlayListManager : public QWidget
{
    Q_OBJECT
    QPushButton button_add;
    QPushButton button_remove;
	ListWidget list;
	PlaylistWidget playlist;	

	std::shared_ptr<QWidget> popup;
	QLabel *info;
	QPushButton *button_ok;
	QPlainTextEdit *playlist_name;
	void resizeEvent(QResizeEvent* event);
	

public:
    explicit PlayListManager(QWidget *parent = 0);

signals:

public slots:
	void add_playlist();
	void create_playlist();
	void remove_playlist();

};

#endif // __PLAY_LIST_MANAGER_H_
