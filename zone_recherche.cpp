#include "zone_recherche.h"
#include "recuperer_database.h"
#include "interface_user.h"
#include"interface_user.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QScrollArea>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include<QToolTip>
#include <QTimer>
#include <QMessageBox>


Zone_recherche::Zone_recherche(int id_user, Interface_user* parentInterface, QWidget *parent) : QWidget(parent),parentInterfaceUser(parentInterface)
{


        this->id_user = id_user;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    zoneRecherche = new QLineEdit(this);

    zoneRecherche->setPlaceholderText("Rechercher un ami...");
    zoneRecherche->setClearButtonEnabled(true);
    zoneRecherche->setStyleSheet(R"(
        QLineEdit {
            padding: 4px 10px;
            border: 2px solid #d0d0d0;
            border-radius: 10px;
            background-color: #fafafa;
            font-size: 12px;
            color: #333;
        }
        QLineEdit:focus {
            border: 2px solid #0078d7;
            background-color: #ffffff;
        }
    )");
    mainLayout->addWidget(zoneRecherche);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    containerCartes = new QWidget();
    layoutCartes = new QVBoxLayout(containerCartes);
    layoutCartes->setSpacing(0);
    layoutCartes->setContentsMargins(0, -5, 0, 0);
    layoutCartes->setAlignment(Qt::AlignTop);
    containerCartes->setLayout(layoutCartes);

    scrollArea->setWidget(containerCartes);
    mainLayout->addWidget(scrollArea);

    connect(zoneRecherche, &QLineEdit::textChanged,
            this, &Zone_recherche::charger_Users);

    setLayout(mainLayout) ;
}

void Zone_recherche::clearUsers()
{

    QLayoutItem *child;
    while ((child = layoutCartes->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }
}

void Zone_recherche::charger_Users(const QString &texte)
{
     clearUsers();

    if (texte.trimmed().isEmpty()) {
        return;
    }


    recuperer_database recuperer;

    QList<QVariantMap> users = recuperer.Recuperer_users(texte, id_user);

    for (const QVariantMap &user : users) {
        QWidget *carte = new QWidget(containerCartes);
        carte->setObjectName("carte");

        QHBoxLayout *hLayout = new QHBoxLayout(carte);
        hLayout->setContentsMargins(6, 6, 6, 6);
        hLayout->setSpacing(8);

        // Photo de profil
        QLabel *photo = new QLabel(carte);
        photo->setFixedSize(40,40);
        photo->setStyleSheet("background : transparent ;") ;

        QPixmap pix;
        QString photoPath = user["photo_profil"].toString();
        if (!photoPath.isEmpty() && QFile::exists(photoPath)) {
            pix.load(photoPath);
        } else {
            pix.load(":/icons/profile.png");
        }

        QPixmap circularPixmap(pix.size());
        circularPixmap.fill(Qt::transparent);

        QPainter painter(&circularPixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPainterPath path;
        path.addEllipse(pix.rect());
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, pix);

        photo->setPixmap(circularPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        photo->setScaledContents(true);

        hLayout->addWidget(photo, 0, Qt::AlignVCenter);

       QPushButton *btnAjouter = new QPushButton ;
        btnAjouter->setFixedSize(26,26);

        bool estAmi = false;
        bool estInvite = false;

        QList<QVariantMap> amis = recuperer.Recuperer_Amis(id_user);
        QList<QVariantMap> invitations= recuperer.Recuperer_invitations(id_user) ;

        for(const QVariantMap &ami : amis) {
            if (ami["id"].toInt() == user["id_utilisateur"].toInt()) {

                    btnAjouter->setIcon(QIcon(":/icons/ami.png"));
                    btnAjouter->setDisabled(true);
                    estAmi = true;
                    break;

            }
        }

        for(auto invitation: invitations){
            if(invitation["id_sender"].toInt()==user["id_utilisateur"].toInt() || invitation["id_otherUser"].toInt()==user["id_utilisateur"].toInt()){
                btnAjouter->setIcon(QIcon(":/icons/invitation.png"));
                btnAjouter->setDisabled(true);
                estInvite = true;
            }
        }


        if (!estAmi && !estInvite) {
            btnAjouter->setIcon(QIcon(":/icons/ajouter-ami.png"));

            connect(btnAjouter, &QPushButton::clicked, this, [=]() {
                Inserer_database Inserer;
                if (Inserer.Ajouter_ami(id_user, user["id_utilisateur"].toInt())) {

                    btnAjouter->setIcon(QIcon(":/icons/invitation.png"));

                    Inserer.signal_invitation_sent(user["id_utilisateur"].toInt()) ;


                } else {
                    QMessageBox::warning(nullptr, "Erreur", "Échec de l'ajout de l'ami.");
                }
            });
        }



        btnAjouter->setIconSize(QSize(18, 18));
        //btnAjouter->setFlat(true);
        btnAjouter->setStyleSheet("background: transparent;");

        QPushButton *btnMessage = new QPushButton(carte);
        btnMessage->setFixedSize(26, 26);
        btnMessage->setIcon(QIcon(":/icons/envoyer-email.png"));
        btnMessage->setIconSize(QSize(18, 18));
        btnMessage->setFlat(true);
        btnMessage->setStyleSheet("background: transparent;");
        //-------------------------------------------------------------------------------------

        connect(btnMessage, &QPushButton::clicked, this, [=]() {

            int id_contact = user["id_utilisateur"].toInt() ;

            recuperer_database db;
            QList<QVariantMap> Messages=db.Recuperer_messages(id_user,id_contact) ;

            parentInterfaceUser->id_contact_selectionne= id_contact ;


            if(Messages.isEmpty())
            {

                parentInterfaceUser->afficher_message_vide(id_contact) ;

                return ;
            }

            parentInterfaceUser->switchToMessagesView(id_contact) ;

        });


        //----------------------------------------------------------------------

        QVBoxLayout *vLayout = new QVBoxLayout();
        QString nomComplet = user["prenom"].toString() + " " + user["nom"].toString();
        QLabel *labelNom = new QLabel(nomComplet, carte);
        labelNom->setStyleSheet("font-weight: bold; font-size: 12px; background:transparent ;");

        QString genreTexte = (user["genre"].toString() == "H") ? "Homme" : "Femme";
        QLabel *labelGenre = new QLabel("Genre : " + genreTexte, carte);
        labelGenre->setStyleSheet("color: #666666; font-size: 11px; background:transparent ;");

        vLayout->addWidget(labelNom);
        vLayout->addWidget(labelGenre);
        hLayout->addLayout(vLayout);
        hLayout->addStretch();
        hLayout->addWidget(btnAjouter);
        hLayout->addWidget(btnMessage);

        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(6);
        shadow->setOffset(1, 1);
        shadow->setColor(QColor(0, 0, 0, 30));
        carte->setGraphicsEffect(shadow);

        carte->setLayout(hLayout);
        layoutCartes->addWidget(carte);
    }

    setStyleSheet(R"(
        #carte {
            background-color: #ffffff;
            border: 1px solid #dddddd;
            border-radius: 10px;
            padding: 4px;
            margin-bottom: 2px;
        }
        #carte:hover {
            background-color: #f0f8ff;
            border: 1px solid #0078d7;
        }
    )");


  }
