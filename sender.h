#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QUdpSocket>

class Stream;
class StreamSender : public QObject
{
    Q_OBJECT
public:
    explicit StreamSender(QObject *parent = 0);
    void HelloUDP();
   	void sendStream(Stream* s);
signals:
    
public slots:
    void readyRead();

private:
    QUdpSocket *socket;
    QHostAddress server_ip; 
};

#endif // MYUDP_H
