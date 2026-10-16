#ifndef CARTE_TOP_H
#define CARTE_TOP_H

#include "recuperer_database.h"
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QFont>

class Carte_top : public QWidget {
    Q_OBJECT

public:
    explicit Carte_top(QWidget* parent = nullptr);  // plus de paramètre id ici
    void mise_a_jour(int id_contact);               // nouvelle fonction de mise à jour

private:
    QLabel* avatarLabel;
    QLabel* nomLabel;
    QLabel* statutLabel;

    void initialiserUI(); // méthode pour construire l’interface une seule fois
};

#endif // CARTE_TOP_H
