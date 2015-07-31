#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include "maincontrolpanel.h"
#include "playlistwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    MainControlPanel panel;
    PlaylistWidget list;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
