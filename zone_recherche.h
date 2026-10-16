
#ifndef ZONE_RECHERCHE_H
#define ZONE_RECHERCHE_H
#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QVector>
#include <QVariantMap>
#include<QPushButton>
#include<QMessageBox>
#include<QObject>
#include "inserer_database.h"
#include"QApplication"

class Interface_user ;

class Zone_recherche : public QWidget
{
    Q_OBJECT
public:

    explicit Zone_recherche(int,Interface_user *,QWidget * = nullptr);


private slots:
    void charger_Users(const QString &);


private:
    int id_user ;

    QLineEdit *zoneRecherche;
    QScrollArea *scrollArea;
    QWidget *containerCartes;
    QVBoxLayout *layoutCartes;
    Interface_user* parentInterfaceUser;  // Pointeur vers l'interface parente

    void clearUsers();

    void ajouter_ami() ;


};

#endif // ZONE_RECHERCHE_H

