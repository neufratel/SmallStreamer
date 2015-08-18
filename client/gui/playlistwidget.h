#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QTimer>
#include <mutex>
class PlaylistWidget : public QListWidget
{
    Q_OBJECT
	QTimer* timer;
	std::recursive_mutex mutex;
	unsigned int old_playlist_index;
public:
    explicit PlaylistWidget(QWidget *parent = 0);
    void loadPlaylist(unsigned int idx);
signals:

public slots:
	void clicked();
    	void doubleClicked();
	void selected();
	void dropEvent(QDropEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
};

#endif // PLAYLISTWIDGET_H
