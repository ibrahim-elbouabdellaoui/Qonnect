#include "loginwindow.h"
#include"inserer_database.h"
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QFrame>
#include"recuperer_database.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    // Widget principal avec ombre portée
    QWidget *Sous_LoginWidget = new QWidget(this);
    Sous_LoginWidget->setStyleSheet(
        "background-color: #ffffff;"
        "border-radius: 12px;"
        "padding: 24px;"
        );

    Sous_LoginWidget->setFixedSize(300, 480);

    // Effet d'ombre
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(25);
    shadow->setOffset(0, 5);
    shadow->setColor(QColor(0, 0, 0, 60));
    Sous_LoginWidget->setGraphicsEffect(shadow);

    // Layout principal
    QVBoxLayout *MainLayout = new QVBoxLayout(this);

    QLabel* logoLabel = new QLabel(this);
    logoLabel->setStyleSheet("background : none ;") ;
    QPixmap logo(":/icons/logo.png");
    logoLabel->setPixmap(logo.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    MainLayout->addWidget(logoLabel) ;
    MainLayout->addSpacing(10) ;


    MainLayout->addWidget(Sous_LoginWidget, 0, Qt::AlignCenter);
    MainLayout->setContentsMargins(40, 40, 40, 40);
    setStyleSheet("background-color: #f0f2f5;");



    // Titre
    QLabel *Header = new QLabel("Se connecter", Sous_LoginWidget);
    Header->setAlignment(Qt::AlignCenter);
    Header->setStyleSheet(
        "color: #075e54;"  // Couleur verte foncée WhatsApp
        "font-size: 22px;"
        "font-weight: bold;"
        "margin-bottom: 20px;"
        ) ;


    QString inputStyle =
        "QLineEdit {"
        "   border: 1px solid #d1d7db;"  // Gris clair WhatsApp
        "   border-radius: 8px;"
        "   padding: 12px 16px;"
        "   font-size: 15px;"
        "   background-color: white;"
        "   color: #3b4a54;"  // Texte foncé WhatsApp
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #25d366;"  // Vert WhatsApp
        "   outline: none;"
        "}";



    // Input email/numéro
    NameInput = new QLineEdit(Sous_LoginWidget);
    NameInput->setPlaceholderText("Adresse e-mail ou numéro de téléphone");
    NameInput->setStyleSheet(inputStyle);

    // Input mot de passe
    PasswordInput = new QLineEdit(Sous_LoginWidget);
    PasswordInput->setPlaceholderText("Mot de passe");
    PasswordInput->setEchoMode(QLineEdit::Password);
    PasswordInput->setStyleSheet(inputStyle);

    // Bouton de connexion
    LoginButton = new QPushButton("Se connecter", Sous_LoginWidget);

    LoginButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #25d366;"  // Vert WhatsApp
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 14px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #20bd5b;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1aa350;"
        "}" ) ;

    LoginButton->setCursor(Qt::PointingHandCursor);

    // Lien mot de passe oublié
    QPushButton *Lien_mot_passe_oublie = new QPushButton("Mot de passe oublié ?", Sous_LoginWidget);
    Lien_mot_passe_oublie->setStyleSheet(
        "QPushButton {"
        "   color: #1877f2;"
        "   font-size: 13px;"
        "   background: transparent;"
        "   border: none;"
        "   text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "   text-decoration: underline;"
        "}"
        );
    Lien_mot_passe_oublie->setCursor(Qt::PointingHandCursor);

    // Séparateur
    QFrame *separator = new QFrame(Sous_LoginWidget);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet(
        "border: none;"
        "border-top: 1px solid #dadde1;"
        "margin: 20px 0;"
        );

    // Bouton créer un compte
    QPushButton *createAccountButton = new QPushButton("Créer un compte", Sous_LoginWidget);

    createAccountButton->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "               stop:0 #34B7F1, stop:1 #25D366);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "               stop:0 #2AA8E0, stop:1 #20BD5B);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "               stop:0 #1F97D1, stop:1 #1AA350);"
        "}");

    createAccountButton->setCursor(Qt::PointingHandCursor);
    connect(createAccountButton, &QPushButton::clicked, this, &LoginWindow::showSignupRequested);

    // Layout des éléments
    QVBoxLayout *Vlayout = new QVBoxLayout(Sous_LoginWidget);
    Vlayout->addWidget(Header);
    Vlayout->addSpacing(10);
    Vlayout->addWidget(NameInput);
    Vlayout->addSpacing(12);
    Vlayout->addWidget(PasswordInput);
    Vlayout->addSpacing(16);
    Vlayout->addWidget(LoginButton);
    Vlayout->addSpacing(12);
    Vlayout->addWidget(Lien_mot_passe_oublie, 0, Qt::AlignCenter);
    Vlayout->addWidget(separator);
    Vlayout->addWidget(createAccountButton);
    Vlayout->addStretch();

      connect(LoginButton,&QPushButton::clicked,this,[=](){
        if(!NameInput->text().isEmpty() && !PasswordInput->text().isEmpty()){
            recuperer_database R ;
            int verification=R.Verifie_inscription(NameInput->text(),PasswordInput->text()) ;
            qDebug()<<"verification :"<<verification  ;
            if(verification==1){
                Inserer_database inserer ;
                   inserer.Status_EnLigne(R.get_id_byEmail(NameInput->text())) ;

                emit loginSuccessful(NameInput->text()) ; }
            else if(verification==0)  emit loginFailed("Mot de passe incorrect !");
            else emit loginFailed("aucun utilisateur n'existe avec cet email");
        }
      }) ;


    Sous_LoginWidget->setLayout(Vlayout);
    setLayout(MainLayout);
    //----------------------------
    Header->setStyleSheet(
        "color: #075E54;"  // Vert foncé WhatsApp (officiel pour les titres)
        "font-size: 22px;"
        "font-weight: bold;"
        "margin-bottom: 20px;"
        "font-family: 'Segoe UI', Helvetica, sans-serif;"  // Police WhatsApp-like
        );

    Header->setStyleSheet(
        "color: #128C7E;"  // Vert WhatsApp intermédiaire
        "font-size: 22px;"
        "font-weight: bold;"
        "margin-bottom: 20px;"
        "text-shadow: 1px 1px 2px rgba(0,0,0,0.1);"
        );

    setStyleSheet("background-color: #e6ebe8;");
}
