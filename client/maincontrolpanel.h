#ifndef MAINCONTROLPANEL_H
#define MAINCONTROLPANEL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QCheckBox>
#include <QTimer>
#include <QSize>
#include <QLabel>

class MainControlPanel : public QWidget
{
    Q_OBJECT
    QLabel * song_name;
    QLabel * song_time;
    QPushButton *button_play;
    QPushButton *button_stop;
    QPushButton *button_next;
    QPushButton *button_pierv;
    QSlider *progress;
    QSlider *volume;
    QTimer *timer;
	QTimer *button_play_timer;
	QCheckBox* auto_play_box;
    int a;
	bool playing;
	short progress_value;
	virtual void resizeEvent(QResizeEvent* event);
	

	static const QPoint NAME_POINT=QPoint(20, 30);
	static const QSize NAME_SIZE=QSize(260, 30);
	
	static const QSize S_TIME_SIZE=QSize(150, 30);


	static const QSize CONTROL_SIZE=QSize(300, 120);
	static const QPoint CONTROL_POINT=QPoint(0, 0);

	static const QSize B_PLAY_SIZE=QSize(45, 50);
	static const QPoint B_PLAY_POINT=QPoint(10, 60);
	
	static const QSize B_STOP_SIZE=QSize(30, 30);
	static const QPoint B_STOP_POINT=QPoint(70, 80);

	static const QSize B_NEXT_SIZE=QSize(30, 30);
	static const QPoint B_NEXT_POINT=QPoint(140, 80);

	static const QSize B_PREV_SIZE=QSize(30, 30);
	static const QPoint B_PREV_POINT=QPoint(105, 80);

	static const QSize VOLUME_SIZE=QSize(100, 20);
	static const QPoint VOLUME_POINT=QPoint(120, 30);	
	
	static const QPoint PROG_BAR_POINT=QPoint(60, 50);
	
	
	
public:
    explicit MainControlPanel(QWidget *parent = 0);

signals:

public slots:
    void play();
    void next();
    void pierv();
    void setProgress();
    void changeProgress();
    void setVolume();
	void refreshButtonPlay();
	void autoplay();


};

#endif // MAINCONTROLPANEL_H
