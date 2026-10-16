#include "SignUp.h"
#include"verification_input.h"
#include"inserer_database.h"
#include"loginwindow.h"
#include"QMessageBox"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QDateEdit>
#include<QRadioButton>
#include<QComboBox>
#include<QCheckBox>
#include<QStackedWidget>
#include<QTimer>
#include<QDebug.h>

SignUp::SignUp(){

    //le style generale :
    setStyleSheet("QWidget { background-color: #F0F0F0 ; }"
                  "QLabel { colooor: #075E54;font-size: 14px; }"
                  "QLineEdit { padding: 5px; border-radius: 5px; border: 1px solid #C0C0C0;; background-color: white; }"
                  "QLineEdit:focus { border:2px solid #25D366; }"
                  "QComboBox {padding: 5px; border-radius: 5px; border: 1px solid rgba(204, 204, 204, 1); background-color: white;}"
                  "QPushButton { padding: 0px 4px; border : 1px solid ;border-radius: 5px; font-weight: bold; }"
                  "QPushButton:disabled { background-color: #D3D3D3; color: #888888;}"
                  "QDateEdit { padding: 5px; border-radius: 5px; border: 1px solid rgba(204, 204, 204, 1); background-color: white; }"
                  "QRadioButton { color: #333333; }"
                  );

    QWidget *Container=new QWidget(this) ;
    Container->resize(800,600) ;

    //logo


    QLabel* logoLabel = new QLabel(this);
    logoLabel->setStyleSheet("background : none ;") ;
    QPixmap logo(":/icons/logo.png");
    logoLabel->setPixmap(logo.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);



    //le header :
    QLabel *Header=new QLabel("Créer  votre compte ",Container) ;
    Header->setStyleSheet(
        "color: #075e54;"
        "font-size: 24px;"
        "font-weight: bold;"
        "margin-bottom: 20px;"
        "font-family: 'Segoe UI', Helvetica, sans-serif;"
        );

    Header->setAlignment(Qt::AlignCenter) ;

    // Message d'erreur
    QLabel *errorMessage = new QLabel(Container);
    errorMessage->setStyleSheet("color: red;");
    errorMessage->setAlignment(Qt::AlignCenter);
    errorMessage->setVisible(false);

    //La barre de progression :
    QLabel *ProgressNumber[4] ;
    ProgressNumber[0]=new QLabel("1",Container) ;
    ProgressNumber[1]=new QLabel("2",Container) ;
    ProgressNumber[2]=new QLabel("3",Container) ;
    ProgressNumber[3]=new QLabel("4",Container) ;

    QLabel *ProgressName[4] ;
    ProgressName[0]=new QLabel("Profil",Container);
    ProgressName[1]=new QLabel("Contact",Container);
    ProgressName[2]=new QLabel("Sécurité",Container);
    ProgressName[3]=new QLabel("Confirmation",Container);


    for(int i=0;i<4;i++){
        ProgressNumber[i]->setFixedSize(55,55) ;
        ProgressNumber[i]->setAlignment(Qt::AlignCenter);

        ProgressName[i]->setAlignment(Qt::AlignCenter) ;
    }

    //fonction pour mettre a jour la barre de progression :

    auto UpdateProgressIndicator = [=](int activeStep) {
        for (int i = 0; i < 4; i++) {
            if (i == activeStep) {
                // Étape active
                ProgressNumber[i]->setStyleSheet(
                    "color: white; font-size: 16px; font-weight: bold;"
                    "border-radius: 27px; background-color: #25D366;"
                    "border: 2px solid #128C7E;"
                    );
                ProgressName[i]->setStyleSheet(
                    "color: #075E54; font-size: 14px; font-weight: bold;"
                    );
            } else if (i < activeStep) {
                // Étape complétée
                ProgressNumber[i]->setStyleSheet(
                    "color: white; font-size: 16px;"
                    "border-radius: 27px; background-color: #128C7E;"
                    "border: 2px solid #075E54;"
                    );
                ProgressName[i]->setStyleSheet(
                    "color: #128C7E; font-size: 14px;"
                    );
            } else {
                // Étape future
                ProgressNumber[i]->setStyleSheet(
                    "color: #999999; font-size: 16px;"
                    "border-radius: 27px; background-color: #FFFFFF;"
                    "border: 2px solid #C0C0C0;"
                    );
                ProgressName[i]->setStyleSheet(
                    "color: #999999; font-size: 14px;"
                    );
            }
        }
    };

    // Lamda pour mettre a jour les button suivant et precedant



    //initialiser la barre de progression :
    UpdateProgressIndicator(0);

    //les informations a saisir :
      //information de profil :
    Profil_Step =new QWidget(Container) ;

    QLineEdit *LineEdit_Prenom=new QLineEdit(Profil_Step) ;
    QLineEdit *LineEdit_Nom=new QLineEdit(Profil_Step);
    LineEdit_Prenom->setPlaceholderText("Prénom") ;
    LineEdit_Nom->setPlaceholderText("Nom") ;
    LineEdit_Nom->setMaximumSize(200,30) ;
    LineEdit_Prenom->setMaximumSize(200,30) ;



    QLabel *Date_Naissance =new QLabel("Date de naissance",Profil_Step) ;
    QComboBox *Jour_Date_Naissance=new QComboBox(Profil_Step) ;
    for(int i=1;i<=31;i++){
        Jour_Date_Naissance->addItem(QString::number(i)) ;
    }
    QComboBox *Mois_Date_Naissance=new QComboBox(Profil_Step);
    QStringList mois = {"jan", "fév", "mar", "avr", "mai", "juin", "juil", "août", "sep", "oct", "nov", "déc"};
    Mois_Date_Naissance->addItems(mois);
    QComboBox *Annee_Date_Naissace=new QComboBox(Profil_Step);
    int annee_actuel=QDate::currentDate().year() ;
    for(int an=annee_actuel;an!=annee_actuel-100;an--){
        Annee_Date_Naissace->addItem(QString::number(an)) ;
    }


    QLabel *Adresse =new QLabel("Adresse",Profil_Step) ;
    QLineEdit *LineEdit_Adresse=new QLineEdit(Profil_Step) ;

    QLabel *Genre =new QLabel("Genre",Profil_Step);
    QRadioButton *RadioButton_Genre_homme=new QRadioButton("homme",Profil_Step);
    QRadioButton *RadioButton_Genre_femme=new QRadioButton("femme",Profil_Step);

    //Layout pour organiser les widets de Profil_Step
    QHBoxLayout *Nom_PrenomLayout = new QHBoxLayout() ;
    Nom_PrenomLayout->addWidget(LineEdit_Prenom) ;
    Nom_PrenomLayout->addWidget(LineEdit_Nom) ;

    QHBoxLayout *Date_Naissance_Layout=new QHBoxLayout() ;
    Date_Naissance_Layout->addWidget(Jour_Date_Naissance) ;
    Date_Naissance_Layout->addWidget(Mois_Date_Naissance) ;
    Date_Naissance_Layout->addWidget(Annee_Date_Naissace) ;

    QHBoxLayout *RadioButton_Genre_Layout=new QHBoxLayout() ;
    RadioButton_Genre_Layout->addWidget(RadioButton_Genre_homme) ;
    RadioButton_Genre_Layout->addWidget(RadioButton_Genre_femme);

    QVBoxLayout *Profil_Step_Layout = new QVBoxLayout(Profil_Step) ;
    Profil_Step_Layout->addLayout(Nom_PrenomLayout) ;
    Profil_Step_Layout->addSpacing(15) ;
    Profil_Step_Layout->addWidget(Date_Naissance) ;
    Profil_Step_Layout->addLayout(Date_Naissance_Layout);
    Profil_Step_Layout->addSpacing(15) ;
    Profil_Step_Layout->addWidget(Adresse) ;
    Profil_Step_Layout->addWidget(LineEdit_Adresse) ;
    Profil_Step_Layout->addSpacing(15) ;
    Profil_Step_Layout->addWidget(Genre);
    Profil_Step_Layout->addLayout(RadioButton_Genre_Layout) ;
    Profil_Step_Layout->addStretch() ;
    Profil_Step->setLayout(Profil_Step_Layout) ;

    //information contact
    Contact_Step=new QWidget(Container) ;

    QLabel *AdresseEmail=new QLabel("Adresse E-mail",Contact_Step) ;
    QLineEdit *LineEdit_AdresseEmail =new QLineEdit() ;
    LineEdit_AdresseEmail->setPlaceholderText("Entrer votre adresse E-mail (sous forme : nom@domaine.extension )");

    QLabel *PhoneNumber=new QLabel("Numéro de téléphone",Contact_Step) ;
    QLineEdit  *LineEdit_PhoneNumber =new QLineEdit() ;
    LineEdit_PhoneNumber->setPlaceholderText("entrer votre numéro de téléphone ") ;

    //layout pour organiser
    QVBoxLayout *Contact_Step_Layout =new QVBoxLayout(Contact_Step) ;
    Contact_Step_Layout->addWidget(AdresseEmail) ;
    Contact_Step_Layout->addWidget(LineEdit_AdresseEmail) ;
    Contact_Step_Layout->addSpacing(20) ;
    Contact_Step_Layout->addWidget(PhoneNumber) ;
    Contact_Step_Layout->addWidget(LineEdit_PhoneNumber) ;
    Contact_Step_Layout->addStretch() ;
    Contact_Step->setLayout(Contact_Step_Layout) ;


    //information securite
    Securite_Step=new QWidget(Container) ;

    QLabel *MotDePasse=new QLabel("Mot de Passe",Securite_Step) ;
    QLineEdit *LineEdit_MotDEPasse = new QLineEdit(Securite_Step) ;
    LineEdit_MotDEPasse->setPlaceholderText("saisir une mot de passe pour votre compte ") ;

    QLabel *Question_Securite=new QLabel("Question de sécurité",Securite_Step) ;
    QComboBox *ComboBox_Question_securite=new QComboBox(Securite_Step) ;
    ComboBox_Question_securite->addItem("Quel est le prénom de votre mère ?");
    ComboBox_Question_securite->addItem("Quel est le nom de votre meilleur ami d'enfance ?") ;
    ComboBox_Question_securite->addItem("Quel est votre film préféré ?") ;

    QLineEdit *LineEdit_Question_securite=new QLineEdit(Securite_Step) ;
    LineEdit_Question_securite->setPlaceholderText("saisir votre réponce ici ") ;

    // layout pour organiser Securite_Step ;
    QVBoxLayout *Securite_Step_Layout =new QVBoxLayout(Securite_Step) ;
    Securite_Step_Layout->addWidget(MotDePasse) ;
    Securite_Step_Layout->addWidget(LineEdit_MotDEPasse) ;
    Securite_Step_Layout->addSpacing(20) ;
    Securite_Step_Layout->addWidget(Question_Securite) ;
    Securite_Step_Layout->addWidget(ComboBox_Question_securite) ;
    Securite_Step_Layout->addWidget(LineEdit_Question_securite) ;
    Securite_Step_Layout->addStretch() ;
    Securite_Step->setLayout(Securite_Step_Layout) ;


    // Comfirmation des informations :
    Confirmation_Step=new QWidget(Container) ;

    QLabel *Choix_Identificateur=new QLabel("Choisir votre identificateur : ",Confirmation_Step);
    QRadioButton *Identificateur_Email=new QRadioButton("E-mail");
    QRadioButton *Identificateur_NumTele=new QRadioButton("Numéro de téléphone ") ;

    QLineEdit *LineEdit_Identificateur= new QLineEdit(Confirmation_Step) ;
    LineEdit_Identificateur->setVisible(false) ;

    // afficher seulement si une case est coche :
    connect(Identificateur_Email,&QRadioButton::toggled,Confirmation_Step,[=](bool isChecked){
        if(isChecked){
            LineEdit_Identificateur->setPlaceholderText("votre E-mail");
            LineEdit_Identificateur->setText(LineEdit_AdresseEmail->text());
            LineEdit_Identificateur->setVisible(isChecked) ;
        }
    }) ;

    connect(Identificateur_NumTele,&QRadioButton::toggled,Confirmation_Step,[=](bool isChecked){
        if(isChecked){
            LineEdit_Identificateur->setPlaceholderText("votre numéro de téléphone");
            LineEdit_Identificateur->setText(LineEdit_PhoneNumber->text());
            LineEdit_Identificateur->setVisible(isChecked) ;
        }
    }) ;

    QLabel *Confirmation_Mot_De_Passe=new QLabel("Confirmer votre mot de passe :",Confirmation_Step) ;
    QLineEdit *LineEdit_Confirmation_Mot_De_Passe =new QLineEdit(Confirmation_Step) ;
    LineEdit_Confirmation_Mot_De_Passe->setPlaceholderText("Reécrire votre mot de passe ") ;

    // Layouts pour organiser Confirmation_Step ;
    QHBoxLayout *Layout_RadioButton_Identificateur=new QHBoxLayout();
    Layout_RadioButton_Identificateur->addWidget(Identificateur_Email) ;
    Layout_RadioButton_Identificateur->addWidget(Identificateur_NumTele) ;

    QVBoxLayout *Confirmation_Step_Layout=new QVBoxLayout(Confirmation_Step) ;
    Confirmation_Step_Layout->addWidget(Choix_Identificateur) ;
    Confirmation_Step_Layout->addLayout(Layout_RadioButton_Identificateur) ;
    Confirmation_Step_Layout->addWidget(LineEdit_Identificateur) ;
    Confirmation_Step_Layout->addSpacing(20) ;
    Confirmation_Step_Layout->addWidget(Confirmation_Mot_De_Passe) ;
    Confirmation_Step_Layout->addWidget(LineEdit_Confirmation_Mot_De_Passe) ;
    Confirmation_Step_Layout->addStretch() ;
    Confirmation_Step->setLayout(Confirmation_Step_Layout) ;

    //les buttons :
    QPushButton *Suivant=new QPushButton("Suivant",Container) ;
    QPushButton  *Precedant =new QPushButton("Précédant",Container);

    auto UpdateButtons= [=](int nextStep){
        if(nextStep==0)  Precedant->setEnabled(false) ;
        else if(nextStep==1) Precedant->setEnabled(true) ;
        else if(nextStep==2) Suivant->setEnabled(true) ;
     } ;

    auto StyleInput=[=](int activeStep){

        if(activeStep==0){
            if(!Verification_Nom_prenom(LineEdit_Prenom)) LineEdit_Prenom->setStyleSheet("border : 2px solid red ;") ;
        }
    };


    Suivant->setStyleSheet(
        "QPushButton {"
        "   background-color: #00a884;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 5px 10px;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #008069; }"
        "QPushButton:pressed { background-color: #006955; }"
        "QPushButton:disabled {  background-color: #e9edef; color: #aebac1; }"
        );

    Precedant->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #128C7E;"
        "   border: 1px solid #128C7E;"
        "   border-radius: 8px;"
        "   padding: 6px 14px;"
        "   font-size: 15px;"
        "}"
        "QPushButton:hover { background-color: rgba(18,140,126,0.1); }"
        "QPushButton:pressed { background-color: rgba(18,140,126,0.2); }"
        "QPushButton:disabled { color: #aebac1; border-color: #aebac1; }"
        );

    Suivant->setMaximumSize(100,30) ;
    Precedant->setMaximumSize(100,30) ;

    UpdateButtons(0) ;

    // le lien vers la fenetre Login :
    QPushButton *Lien_Vers_Login=new QPushButton("Vous avez déjà un compte ?",Container) ;

    Lien_Vers_Login->setStyleSheet(
        "QPushButton {"
        "   color: #1877f2;"
        "   font-size: 13px;"
        "   background: transparent;"
        "   border: none;"
        "   text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "   text-decoration: underline;"
        "color : darkblue ;"
        "}"
        );


    Lien_Vers_Login->setMaximumSize(180,30) ;
    Lien_Vers_Login->setCursor(Qt::PointingHandCursor);

    connect(Lien_Vers_Login, &QPushButton::clicked, this, &SignUp::showLoginRequested);

    QStackedWidget *Widgets = new QStackedWidget(Container);
    Widgets->addWidget(Profil_Step);
    Widgets->addWidget(Contact_Step);
    Widgets->addWidget(Securite_Step);
    Widgets->addWidget(Confirmation_Step);

    // Fonction pour vérifier si l'étape courante est valide
    auto IsCurrentStepValid = [=](int currentIndex) -> bool {
        switch(currentIndex) {
        case 0: // Profil Step
            return Verification_Nom_prenom(LineEdit_Prenom) &&
                   Verification_Nom_prenom(LineEdit_Nom) &&
                   Verification_Adresse(LineEdit_Adresse) &&
                   Verification_Age_Annee(Annee_Date_Naissace) &&
                   (RadioButton_Genre_homme->isChecked() || RadioButton_Genre_femme->isChecked());
        case 1: // Contact Step
            return Verification_AdresseEmail(LineEdit_AdresseEmail) &&
                   Verification_NumTele(LineEdit_PhoneNumber);
        case 2: // Sécurité Step
            return Verification_Mot_De_Passe(LineEdit_MotDEPasse) &&
                   !LineEdit_Question_securite->text().isEmpty();
        case 3: // Confirmation Step
            return Verification_Correspondane_Input(LineEdit_MotDEPasse, LineEdit_Confirmation_Mot_De_Passe) &&
                   (Identificateur_Email->isChecked() || Identificateur_NumTele->isChecked());
        default:
            return false;
        }
    };

    //verification des inputs
    //profil step
    auto Verification_Profil_Step=[=](){
        if(Verification_Nom_prenom(LineEdit_Prenom)){
            LineEdit_Prenom->setStyleSheet("border : 2px solid rgba(204, 204, 204, 1) ;") ;
        }
        else{
            LineEdit_Prenom->setStyleSheet("border: 2px solid red ;") ;
        }

        if(Verification_Nom_prenom(LineEdit_Nom)){
            LineEdit_Nom->setStyleSheet("border : 2px solid  rgba(204, 204, 204, 1) ;") ;
        }
        else{
            LineEdit_Nom->setStyleSheet("border: 2px solid red") ;
        }

        if(Verification_Adresse(LineEdit_Adresse)){
            LineEdit_Adresse->setStyleSheet("border : 2px solid rgba(204, 204, 204, 1) ;") ;
        }
        else{
            LineEdit_Adresse->setStyleSheet("border: 2px solid red ;") ;
        }

        if(Verification_Age_Annee(Annee_Date_Naissace)){
            Annee_Date_Naissace->setStyleSheet("border : 2px solid rgba(204, 204, 204, 1);") ;
            Jour_Date_Naissance->setStyleSheet("border : 2px solid rgba(204, 204, 204, 1);") ;
            Mois_Date_Naissance->setStyleSheet("border : 2px solid rgba(204, 204, 204, 1);") ;
        }
        else{
            Annee_Date_Naissace->setStyleSheet("border : 2px solid red ;") ;
            Jour_Date_Naissance->setStyleSheet("border : 2px solid red ;") ;
            Mois_Date_Naissance->setStyleSheet("border : 2px solid red ;") ;
        }

    };


    // Gestion du bouton Suivant
    connect(Suivant,&QPushButton::clicked,this,[=](){
        int index = Widgets->currentIndex();

        if(!IsCurrentStepValid(index)) {
           errorMessage->setVisible(true);
            switch(index) {
            case 0:
                errorMessage->setText("Veuillez remplir correctement tous les champs du profil");
                break;
            case 1:
                errorMessage->setText("Email et/ou numéro de téléphone invalide");
                break;
            case 2:
                errorMessage->setText("Mot de passe invalide ou réponse de sécurité manquante");
                break;
            case 3:
                errorMessage->setText("Veuillez confirmer votre mot de passe et choisir un identifiant");
                break;
            }

            QTimer::singleShot(3000, this, [=]() {
                errorMessage->hide();
            });

            return;
        }

        errorMessage->setVisible(false);

        UpdateButtons(index+1) ;

        if(index < Widgets->count()-1) {
            Widgets->setCurrentIndex(index+1);
            UpdateProgressIndicator(index+1);

        }

        else {
            //dernier etape
            // Insérer dans la base de données
            Inserer_database Inserer;
            QString date_naissance_format_standard= Jour_Date_Naissance->currentText()+"/"+QString::number(Mois_Date_Naissance->currentIndex()+1)+"/"+Annee_Date_Naissace->currentText();
            QString genre_format_standard = (RadioButton_Genre_homme->isChecked()? "H":"F" );
            QString question_securite_format_number = QString::number(ComboBox_Question_securite->currentIndex()+1);
             Inserer.Inserer_SignUp_Info(LineEdit_Prenom->text(),LineEdit_Nom->text(),date_naissance_format_standard,
                                        LineEdit_Adresse->text(),genre_format_standard,LineEdit_AdresseEmail->text(),
                                        LineEdit_PhoneNumber->text(),LineEdit_MotDEPasse->text(),
                                        question_securite_format_number,LineEdit_Question_securite->text());

            QMessageBox::information(this, "Compte créé", "Félicitations, votre compte a été créé avec succès !");


            emit showLoginRequested();


        }



    });

    // Gestion du bouton Précédent
    connect(Precedant,&QPushButton::clicked,[=](){
        int index=Widgets->currentIndex();
        UpdateButtons(index-1) ;
        if(index>0){
            Widgets->setCurrentIndex(index-1);
            UpdateProgressIndicator(index-1);
        }

        errorMessage->setVisible(false);
    });

    //poser tous dans la fenetre principale avec des layouts:

    QGridLayout *Step_Layout=new QGridLayout() ;
    for(int i=0;i<4;i++){
        Step_Layout->addWidget(ProgressNumber[i],0,i,Qt::AlignCenter) ;
        Step_Layout->addWidget(ProgressName[i],1,i,Qt::AlignCenter) ;
    }
    Step_Layout->setContentsMargins(80,0,80,0) ;

    QHBoxLayout *Button_Layout=new QHBoxLayout() ;
    Button_Layout->addWidget(Precedant) ;
    Button_Layout->addWidget(Suivant) ;

    //POUR FAIRE DES MARGES A DROITE ET A GAUCHE
    QVBoxLayout *Widgets_Layout=new QVBoxLayout() ;
    Widgets_Layout->addWidget(Widgets) ;
    Widgets_Layout->setContentsMargins(150,0,150,0) ;

    QVBoxLayout *Sous_MAIN_LAYOUT=new QVBoxLayout(Container) ;
    Sous_MAIN_LAYOUT->addWidget(logoLabel) ;
    Sous_MAIN_LAYOUT->addWidget(Header) ;
    Sous_MAIN_LAYOUT->addSpacing(5) ;
    Sous_MAIN_LAYOUT->addLayout(Step_Layout) ;
    Sous_MAIN_LAYOUT->addSpacing(17) ;
    Sous_MAIN_LAYOUT->addLayout(Widgets_Layout) ;
    Sous_MAIN_LAYOUT->addSpacing(10) ;
    Sous_MAIN_LAYOUT->addWidget(errorMessage);
    Sous_MAIN_LAYOUT->addSpacing(10) ;
    Sous_MAIN_LAYOUT->addLayout(Button_Layout) ;
    Sous_MAIN_LAYOUT->addSpacing(10) ;
    Sous_MAIN_LAYOUT->addWidget(Lien_Vers_Login,0,Qt::AlignCenter) ;
    Container->setLayout(Sous_MAIN_LAYOUT) ;

    Container->setStyleSheet(
        "background-color: #ffffff;"
        "border-radius: 12px;"

        );

    // Effet d'ombre
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(25);
    shadow->setOffset(0, 5);
    shadow->setColor(QColor(0, 0, 0, 60));
    Container->setGraphicsEffect(shadow);


    QVBoxLayout *MainLayout=new QVBoxLayout(this);
    MainLayout->addWidget(logoLabel) ;
    MainLayout->addSpacing(0) ;
    MainLayout->addWidget(Container,0,Qt::AlignCenter) ;

    setLayout(MainLayout) ;
}
