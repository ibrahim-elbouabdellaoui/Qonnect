#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket* socket = new QTcpSocket(this);

    if (!socket->setSocketDescriptor(socketDescriptor)) {
        socket->deleteLater();
        return;
    }

    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_0);

        QString type;
        int sender, receiver;
        in >> type >> sender;

        if (type == "register") {
            clients[sender] = socket;
        }
        else if (type == "notify") {
            in >> receiver;
            if (clients.contains(receiver)) {
                QDataStream out(clients[receiver]);
                out.setVersion(QDataStream::Qt_6_0);
                out << QString("message_recu") << sender;
            }
        }
    });

    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        // Nettoyage : retirer l'utilisateur déconnecté
        clients.erase(std::find_if(clients.begin(), clients.end(),
                                   [=](const QTcpSocket* s) { return s == socket; }));
        socket->deleteLater();

    });

}
