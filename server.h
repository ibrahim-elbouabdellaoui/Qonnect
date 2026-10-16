#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QDataStream>

class Server : public QTcpServer
{
    Q_OBJECT

  public:
    explicit Server(QObject *parent = nullptr);

    // Liste des clients connectés : id_user -> socket
    QMap<int, QTcpSocket*> clients;

  protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H
