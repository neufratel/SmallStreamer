#ifndef MAINCONTROLPANEL_H
#define MAINCONTROLPANEL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QCheckBox>
#include <QTimer>

class MainControlPanel : public QWidget
{
    Q_OBJECT
    QPushButton *button_play;
    QPushButton *button_next;
    QPushButton *button_pierv;
    QProgressBar *progress;
    QSlider *volume;
    QTimer *timer;
	QTimer *button_play_timer;
	QCheckBox* auto_play_box;
    int a;
	bool playing;
	short progress_value;
public:
    explicit MainControlPanel(QWidget *parent = 0);

signals:

public slots:
    void play();
    void next();
    void pierv();
    void setProgress();
    void setVolume();
	void refreshButtonPlay();
	void autoplay();


};

#endif // MAINCONTROLPANEL_H
