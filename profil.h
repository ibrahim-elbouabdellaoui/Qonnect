#ifndef PROFIL_H
#define PROFIL_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QPixmap>
#include <QPainter>


class Profil : public QWidget
{

    Q_OBJECT

 public:

    Profil(int id_user,QWidget* parent = nullptr) ;

 private slots:

    void changerPhoto();

 private:
    int id_user ;

    QLabel* photoLabel;
    QLineEdit* nomEdit;
    QLineEdit* prenomEdit;
    QLineEdit* emailEdit;
    QLineEdit* adresseEdit;
    QLineEdit* birthdayEdit;
    QLineEdit* genreEdit;
    QLineEdit* TeleEdit;


    QPushButton* modifierButton;
    QPushButton* changerPhotoButton;

};

#endif // PROFIL_UTILISATEUR_H
