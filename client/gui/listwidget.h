#ifndef __LIST_WIDGET_H__
#define __LIST_WIDGET_H__

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QTimer>
#include <mutex>
class ListWidget : public QListWidget
{
    Q_OBJECT
	QTimer* timer;
	std::recursive_mutex mutex;
	unsigned int old_playlist_size;
public:
    explicit ListWidget(QWidget *parent = 0);
	QListWidgetItem* dropHintItem;
	int current();
    	void loadPlayLists();
signals:

public slots:
    void clicked();
    void doubleClicked();
    void selected();
};

#endif // __LIST_WIDGET_H_
