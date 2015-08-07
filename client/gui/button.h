#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QIcon>
#include <QPixmap>
class Button: public QPushButton
{
    Q_OBJECT
public:
    Button( QWidget *parent = 0);
signals:

public slots:
};

#endif // __BUTTON_H_
