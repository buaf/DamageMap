#include "client.h"

#include <QDataStream>


Client::Client(QObject * parent, QString strHost, int nPort) : QObject(parent), hostAdress(strHost), hostPort(nPort)
{
    clientSocket = new QUdpSocket(this);
    clientSocket->bind(QHostAddress::LocalHost, 1234);

    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readyReadHandler()));
}

Client::~Client()
{
    delete clientSocket;
}

void Client::sendToServer(QString str)
{
    QByteArray data;
    data.append(str);
    clientSocket->writeDatagram(data, hostAdress, hostPort);
    qDebug() << "Send to server:" << str;
}

void Client::readyReadHandler()
{
    QByteArray buffer;
    buffer.resize(clientSocket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    clientSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    qDebug() << sender.toString() << ":" << senderPort << "   " << buffer.data() << "\n";
}


