#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include "maincontrolpanel.h"
#include "playlistwidget.h"
#include <QPoint>
#include "clientcontrolpanel.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
	static const QPoint CON_PAN_POINT=QPoint(300,120);
    MainControlPanel panel;
    PlaylistWidget list;
	ClientControlPanel c_panel;

	void resizeEvent(QResizeEvent* event);
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

};

#endif // MAINWINDOW_H
