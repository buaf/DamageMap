#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>


class Client : public QObject
{
    Q_OBJECT

private:
    QUdpSocket * clientSocket;
    QHostAddress hostAdress;
    int hostPort;


public:
    Client(QObject * parent, QString strHost, int nPort);
    ~Client();
    
    void sendToServer(QString str);
    
public slots:
    void readyReadHandler();
    
};

#endif // CLIENT_H
