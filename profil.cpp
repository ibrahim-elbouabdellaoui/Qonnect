#include "profil.h"
#include "recuperer_database.h"
#include"inserer_database.h"
#include <QPainter>
#include <QPainterPath>
#include<QTimer>
#include <QFileDialog>
#include <QToolButton>

Profil::Profil(int id_user, QWidget* parent)
    : QWidget(parent),id_user(id_user)
{
    recuperer_database db;
    QVariantMap Info = db.Recuperer_info(id_user);

    // ------------------ PHOTO DE PROFIL ------------------
    photoLabel = new QLabel(this);
    photoLabel->setFixedSize(100, 100);
    photoLabel->setAlignment(Qt::AlignCenter);

    QString path = Info["photo_profil"].toString().isEmpty()
                       ? ":/icons/profile.png"
                       : Info["photo_profil"].toString();

    QPixmap avatar;
    if (!avatar.load(path)) {
        avatar.load(":/icons/profile.png");
    }
    QPixmap circularPixmap(avatar.size());
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath circlePath;
    circlePath.addEllipse(avatar.rect());
    painter.setClipPath(circlePath);
    painter.drawPixmap(0, 0, avatar);
    photoLabel->setPixmap(circularPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    changerPhotoButton = new QPushButton("Changer la photo");
    connect(changerPhotoButton, &QPushButton::clicked, this, &Profil::changerPhoto);

    // ------------------ CHAMPS AVEC BOUTON CRAYON ------------------

    QVBoxLayout* VLayout = new QVBoxLayout(this);

    VLayout->addWidget(photoLabel, 0, Qt::AlignCenter);

    VLayout->addWidget(changerPhotoButton, 0, Qt::AlignCenter);

    QToolButton* crayon[6] ;
    QToolButton* save[6] ;


    auto creerChampAvecCrayon = [&](const QString& texte, const QString& placeholder, QLineEdit** outEdit,int index_widget) -> QWidget* {

        QWidget* champ = new QWidget(this);
        QHBoxLayout* champLayout = new QHBoxLayout(champ);
        champLayout->setContentsMargins(0, 0, 0, 0);

        QLineEdit* lineEdit = new QLineEdit(texte, champ);
        lineEdit->setPlaceholderText(placeholder);
        lineEdit->setReadOnly(true);
        *outEdit = lineEdit;

            crayon[index_widget] = new QToolButton(champ) ;
            crayon[index_widget]->setIcon(QIcon(":/icons/pencil.png"));
            crayon[index_widget]->setCursor(Qt::PointingHandCursor);
            crayon[index_widget]->setToolTip("Modifier");
            crayon[index_widget]->setStyleSheet("QToolButton { border: none; }");
            crayon[index_widget]->setFixedSize(22, 22);

             save[index_widget]= new QToolButton(champ);
             save[index_widget]->setIcon(QIcon(":/icons/check-mark.png"));
            save[index_widget]->setCursor(Qt::PointingHandCursor);
            save[index_widget]->setToolTip("enregistrer");
            save[index_widget]->setStyleSheet("QToolButton { border: none; }");
             save[index_widget]->setFixedSize(22, 22);

            save[index_widget]->hide() ;



        connect(crayon[index_widget], &QToolButton::clicked, [=]() {
            lineEdit->setReadOnly(false);
            lineEdit->setFocus();
            crayon[index_widget]->hide() ;
            save[index_widget]->show() ;
         });

        connect(save[index_widget], &QToolButton::clicked, [=]() {
            Inserer_database  inserer ;
            if(index_widget==0){
                inserer.Update_nom(id_user,nomEdit->text()) ;

                recuperer_database recuperer ;
                QList<QVariantMap> Contacts = recuperer.Recuperer_Contacts_DernierMessage(id_user) ;
                for(auto contact : Contacts ){
                    inserer.signal_update_profil(contact["id_utilisateur"].toInt())  ;
                }

            }

            else if(index_widget==1) {

                inserer.Update_prenom(id_user,prenomEdit->text()) ;

                recuperer_database recuperer ;
                QList<QVariantMap> Contacts = recuperer.Recuperer_Contacts_DernierMessage(id_user) ;
                for(auto contact : Contacts ){
                    inserer.signal_update_profil(contact["id_utilisateur"].toInt())  ;
                }

            }
            else if(index_widget==2) inserer.Update_email(id_user,emailEdit->text()) ;
            else if(index_widget==3) inserer.Update_adresse(id_user,adresseEdit->text()) ;
            else if(index_widget==4) inserer.Update_birthday(id_user,birthdayEdit->text()) ;
            else if(index_widget==5) inserer.Update_tele(id_user,TeleEdit->text()) ;

            lineEdit->setReadOnly(true);
            lineEdit->clearFocus();
            crayon[index_widget]->show() ;
            save[index_widget]->hide() ;
        });

        champLayout->addWidget(lineEdit);
        champLayout->addWidget(crayon[index_widget]);
        champLayout->addWidget(save[index_widget]);

        champ->setStyleSheet("font-family: Segoe UI, sans-serif;font-size: 14px;border: 1px solid #ccc;") ;
        return champ;
    };

    QWidget* nomWidget = creerChampAvecCrayon(Info["nom"].toString(), "Nom", &nomEdit,0);
    QWidget* prenomWidget = creerChampAvecCrayon(Info["prenom"].toString(), "Prénom", &prenomEdit,1);
    QWidget* emailWidget = creerChampAvecCrayon(Info["email"].toString(), "Email", &emailEdit,2);
    QWidget* adresseWidget = creerChampAvecCrayon(Info["adresse"].toString(), "Adresse", &adresseEdit,3);
    QWidget* birthdayWidget = creerChampAvecCrayon(Info["date_naissance"].toString(), "Date de naissance", &birthdayEdit,4);
    QWidget* teleWidget = creerChampAvecCrayon(Info["numero_telephone"].toString(), "numéro de téléphone", &TeleEdit,5);


    VLayout->addWidget(nomWidget);
    VLayout->addWidget(prenomWidget);
    VLayout->addWidget(emailWidget);
    VLayout->addWidget(adresseWidget) ;
    VLayout->addWidget(birthdayWidget) ;
    VLayout->addWidget(teleWidget) ;
    //VLayout->addStretch();

    setLayout(VLayout);

    setStyleSheet(R"(
        QLineEdit {
            border : none ;
            border-radius: 5px;
            padding: 6px;
        }
        QPushButton {
             background-color: #25D366;
             color: white ;
            border: none;
            padding: 6px 12px;
            border-radius: 5px;
        }
        QPushButton:hover {
            background-color: #1ebe5b;
        }
          QPushButton:pressed {
           background-color: #128C7E;
            }
    )");
}

void Profil::changerPhoto()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.jpeg)");
    if (!fichier.isEmpty()) {

        Inserer_database Inserer ;
        Inserer.Update_photo(id_user,fichier) ;

        QPixmap avatar(fichier);

        QPixmap circularPixmap(avatar.size());
        circularPixmap.fill(Qt::transparent);

        QPainter painter(&circularPixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPainterPath path;
        path.addEllipse(avatar.rect());
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, avatar);

        QTimer::singleShot(800,[=](){
          photoLabel->setPixmap(circularPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }) ;

        Inserer_database inserer ;
        recuperer_database recuperer ;
        QList<QVariantMap> Contacts = recuperer.Recuperer_Contacts_DernierMessage(id_user) ;
        for(auto contact : Contacts ){
            inserer.signal_update_profil(contact["id_utilisateur"].toInt())  ;
        }

    }
}
