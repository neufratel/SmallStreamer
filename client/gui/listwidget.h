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
public:
    explicit ListWidget(QWidget *parent = 0);
	QListWidgetItem* dropHintItem;
	int current();
signals:

public slots:
    void doubleClicked();
	void selected();
};

#endif // __LIST_WIDGET_H_
