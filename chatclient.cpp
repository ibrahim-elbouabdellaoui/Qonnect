#include "chatclient.h"

ChatClient::ChatClient(int monId, QObject *parent)
    : QObject(parent), id(monId)
{
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 5000);  // Adresse IP et port du serveur

    connect(socket, &QTcpSocket::connected, this, &ChatClient::registerAuServeur);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::recevoirNotification);
}

void ChatClient::envoyerNotification(int idDestinataire)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0); // à adapter à ta version Qt
    out << QString("notify") << id << idDestinataire;
    socket->write(data);
}

void ChatClient::registerAuServeur()
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << QString("register") << id;
    socket->write(data);
}

void ChatClient::envoyerNotification(int idDestinataire, const QString& message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << QString("notify") << id << idDestinataire << message;
    socket->write(data);
}

void ChatClient::recevoirNotification()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_0);

    QString type;
    int idEmetteur;
    QString message;
    in >> type >> idEmetteur >> message;

    if (type == "message_recu") {
        emit nouveauMessageRecu(idEmetteur);
    }
}
