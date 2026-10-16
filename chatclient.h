#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>

class ChatClient : public QObject
{
    Q_OBJECT

public:
    explicit ChatClient(int monId, QObject *parent = nullptr);
    void envoyerNotification(int idDestinataire);

private slots:
    void registerAuServeur();
    void recevoirNotification();

signals:
    void nouveauMessageRecu(int idEmetteur);

public slots:
    void envoyerNotification(int idDestinataire, const QString& message);


private:
    int id;
    QTcpSocket* socket;
};

#endif // CHATCLIENT_H
