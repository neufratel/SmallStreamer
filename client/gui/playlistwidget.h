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
public:
    explicit PlaylistWidget(QWidget *parent = 0);
	QListWidgetItem* dropHintItem;
signals:

public slots:
    void doubleClicked();
	void selected();
	void dropEvent(QDropEvent *event);
//	void dragMoveEvent(QDragMoveEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
};

#endif // PLAYLISTWIDGET_H
