#include "interface_user.h"
#include"QStackedWidget"
#include"AnimatedButtonEffect.h"


Interface_user::Interface_user(const QString& email, QWidget *parent)
    : QWidget(parent),id_contact_selectionne(-1)
{


     R= new recuperer_database ;
     I= new Inserer_database ;

    id_moi=R->get_id_byEmail(email);


     carteTop=new Carte_top ;
     carteTop->setVisible(false) ;

     index_selectionee = -1 ;
     //-----------
     Container_Invitations = new QWidget() ;
     Container_Amis = new QWidget() ;

     ScrollArea_Invitations= new QScrollArea ;
     ScrollArea_Invitations->setWidget(Container_Invitations);
     ScrollArea_Invitations->setWidgetResizable(true);
     ScrollArea_Invitations->setStyleSheet(
         "QScrollArea { background-color: transparent; border: none; padding : 4px ;margin : 0px ;  }"
         "QScrollArea > QWidget > QWidget { background-color: transparent; }"
         );


     ScrollArea_Amis = new QScrollArea ;
     ScrollArea_Amis->setWidget(Container_Amis);
     ScrollArea_Amis->setWidgetResizable(true);
     ScrollArea_Amis->setStyleSheet(
         "QScrollArea { background-color: transparent; border: none;padding : 4px ;margin : 0px ; }"
         "QScrollArea > QWidget > QWidget { background-color: transparent; }"
         );


     layoutInvitations = new QVBoxLayout(Container_Invitations) ;
     layoutInvitations->setAlignment(Qt::AlignTop);
     layoutInvitations->setContentsMargins(0,0,0,0) ;

     Container_Invitations->setStyleSheet("background-color: transparent;");

     Layout_Amis = new QVBoxLayout(Container_Amis) ;
     Layout_Amis->setAlignment(Qt::AlignTop) ;
     Layout_Amis->setContentsMargins(0,0,0,0) ;
     Layout_Amis->setSpacing(0) ;
     Container_Amis->setStyleSheet("background-color: transparent; ") ;
     //-------------

     //button rechrcher amis profil logout
     rechercher = new QPushButton(this);
     rechercher->setToolTip("Chercher de nouveaux utilisateurs") ;

     Amis= new QPushButton(this);
     Amis->setToolTip("Amis") ;

     profil= new QPushButton(this);
     profil->setToolTip("Profil") ;

     logout = new QPushButton(this) ;
     logout->setToolTip("Se déconnecter") ;

     invitation=new QPushButton(this) ;
     invitation->setToolTip("Les demandes d'amitié") ;

     //pour le style des QToolTip

     profil->setCheckable(true);
     Amis->setCheckable(true);
     rechercher->setCheckable(true);
     invitation->setCheckable(true) ;

     //------------------

    // simuler les sockets
    timer_for_msg_sent = new QTimer(this);
    connect(timer_for_msg_sent, &QTimer::timeout, this, [=]() {
        if(R->msg_sent(id_moi)){
            actualiserContacts() ;

            if(id_contact_selectionne != -1 ){
                cleanMessageContainer() ;
                chargerMessages(id_contact_selectionne) ;

                scrollToBottom() ;


            }


            I->no_signal_msg_sent(id_moi) ;
        }
    });
    timer_for_msg_sent->start(800);


    timer_for_msg_read= new QTimer(this) ;
    connect(timer_for_msg_read, &QTimer::timeout, this, [=]() {
        if(R->msg_read(id_moi)){
            if(id_contact_selectionne != -1 ){

                cleanMessageContainer() ;
                chargerMessages(id_contact_selectionne) ;
                scrollToBottom() ;

            }

            I->no_signal_msg_read(id_moi) ;

        }

    });
    timer_for_msg_read->start(800);


    timer_for_update_profil= new QTimer(this) ;
    connect(timer_for_update_profil, &QTimer::timeout, this, [=]() {
        if(R->profil_updated(id_moi)){
            actualiserContacts() ;

            if(id_contact_selectionne != -1 ){

                carteTop->mise_a_jour(id_contact_selectionne) ;


            }

            I->no_signal_update_profil(id_moi) ;

        }

    });
    timer_for_update_profil->start(800);


    animationAmis= new AnimatedButtonEffect(Amis,this) ;
    animationAmiStart = false ;
    timer_for_new_friend = new QTimer(this) ;
      connect(timer_for_new_friend, &QTimer::timeout, this, [&]() {
        if(R->new_friend(id_moi)){
            animationAmis->start() ;
            Amis->setStyleSheet(R"(
    QPushButton {
        background-color: #d3d3d3 ;
        border: none ;
        border-radius : 10px ;
    }
  )") ;
            animationAmiStart =true ;
            I->no_signal_new_friend(id_moi) ;
        }
      });
    timer_for_new_friend->start(800) ;


      // animation de button
      animationInvitation = new AnimatedButtonEffect(invitation, this) ;
      timer_for_invitation_sent = new QTimer(this) ;
      animationInvitationStart = false ;
    connect(timer_for_invitation_sent,&QTimer::timeout,this,[=](){
        if(R->invitation_sent(id_moi)){

              animationInvitation->start();

     invitation->setStyleSheet(R"(
    QPushButton {
        background-color: #d3d3d3 ;
        border: none ;
        border-radius : 10px ;
        }
    )") ;

               animationInvitationStart = true ;

              I->no_signal_invitation_sent(id_moi) ;

        }

     }) ;
    timer_for_invitation_sent->start(800) ;


    status_contact_selectionnee = 0 ;  //pas en ligne

    timer_for_status= new QTimer(this) ;
    connect(timer_for_status, &QTimer::timeout, this, [=]() {
        if( id_contact_selectionne!=-1 && R->Recuperer_info(id_contact_selectionne)["status"].toBool()!=status_contact_selectionnee){
            status_contact_selectionnee = R->Recuperer_info(id_contact_selectionne)["status"].toBool() ;
            carteTop->mise_a_jour(id_contact_selectionne) ;
        }
    });
    timer_for_status->start(800);


    zone_recherche = new Zone_recherche(id_moi,this);
    ScrollArea_Container_Messages = new QScrollArea();
    Mon_profil= new Profil(id_moi,this) ;


    zone_stack = new QStackedWidget(this);


    zone_stack->addWidget(ScrollArea_Container_Messages);
    zone_stack->addWidget(zone_recherche);
    zone_stack->addWidget(ScrollArea_Amis) ;
    zone_stack->addWidget(ScrollArea_Invitations) ;
    zone_stack->addWidget(Mon_profil) ;



   // setMinimumSize(800, 500);

    // Layout principal vertical pour toute la fenêtre
    mainVerticalLayout = new QVBoxLayout(this);
    mainVerticalLayout->setContentsMargins(5, 5, 5, 5);
    mainVerticalLayout->setSpacing(5);

    // Layout horizontal principal pour les contacts et messages
    QWidget *mainContentWidget = new QWidget(this);
    main_layout = new QHBoxLayout(mainContentWidget);


    // Les contacts

    int get_id_byEmail = id_moi ;
    Contacts_Cartes = R->Recuperer_Contacts_DernierMessage(get_id_byEmail);
    int Nbr_Contacts = Contacts_Cartes.size();


    Container_Contacts_Cartes = new QWidget(this);
    main_layout_Container_Contacts_Cartes = new QVBoxLayout(Container_Contacts_Cartes);
    main_layout_Container_Contacts_Cartes->setAlignment(Qt::AlignTop);


    if(R->Recuperer_Contacts_DernierMessage(id_moi).isEmpty()){
        Zone_CarteContact_vide() ;
    }


    Carte_contact.resize(Nbr_Contacts);


    for(int i = 0; i < Nbr_Contacts; i++) {
        Carte_contact[i] = new Qwidget_cliquable(Container_Contacts_Cartes);
        Carte_contact[i]->setObjectName("Carte_contact");
        Carte_contact[i]->setAttribute(Qt::WA_StyledBackground, true);

        QLabel *imageLabel = new QLabel(Carte_contact[i]);
        imageLabel->setFixedSize(40, 40);

        QPixmap pixmap;
        if (!Contacts_Cartes[i]["photo_profil"].toString().isEmpty() &&
            QFile::exists(Contacts_Cartes[i]["photo_profil"].toString())) {
            pixmap.load(Contacts_Cartes[i]["photo_profil"].toString());
        } else {
            pixmap.load(":/icons/profile.png");
        }

        QPixmap circularPixmap(pixmap.size());
        circularPixmap.fill(Qt::transparent);

        QPainter painter(&circularPixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPainterPath path;
        path.addEllipse(pixmap.rect());
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, pixmap);

        imageLabel->setPixmap(circularPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));



        Carte_contact[i]->setMaximumHeight(70);

        // Layout horizontal pour date d'envoi de dernier message et le nom et prénom de l'envoyeur
        QHBoxLayout *send_time_nom_prenom_Layout = new QHBoxLayout();
        QLabel *send_time = new QLabel(Contacts_Cartes[i]["send_time"].toDateTime().toString("dd/MM/yyyy à HH:mm"), Carte_contact[i]);
        QLabel *nom_prenom = new QLabel(Contacts_Cartes[i]["prenom"].toString() + " " + Contacts_Cartes[i]["nom"].toString(), Carte_contact[i]);

        send_time->setStyleSheet("font-size: 10px; color: #666;");
        nom_prenom->setStyleSheet("font-weight: bold; font-size: 12px;");

        send_time_nom_prenom_Layout->addWidget(nom_prenom);
        send_time_nom_prenom_Layout->addStretch() ;
        send_time_nom_prenom_Layout->addWidget(send_time);


        // Layout verticale qui contient layout horizontal(send_time et nom_prenom) et le message envoyé
        QVBoxLayout *main_layout_Carte_contact = new QVBoxLayout();
        main_layout_Carte_contact->addLayout(send_time_nom_prenom_Layout);
        QLabel *last_msg_sent = new QLabel(Contacts_Cartes[i]["content"].toString(), Carte_contact[i]);
        last_msg_sent->setStyleSheet("font-size: 11px; color: #555;");
        last_msg_sent->setWordWrap(true);

        QHBoxLayout *HBoxlayout= new QHBoxLayout ;
        HBoxlayout->addWidget(last_msg_sent) ;

        int nbr_msg = R->nbr_msg_unread(id_moi, Contacts_Cartes[i]["id_utilisateur"].toInt());
        if(nbr_msg!=0){
            QLabel *nbr_msg_unread = new QLabel(QString::number(nbr_msg),Carte_contact[i]) ;
            nbr_msg_unread->setFixedSize(20, 20);
            nbr_msg_unread->setAlignment(Qt::AlignCenter);
            nbr_msg_unread->setStyleSheet("font-size: 10px;color: white ;border-radius : 10px ;background-color : #25D366 ;");
            HBoxlayout->addWidget(nbr_msg_unread) ;
        }

        main_layout_Carte_contact->addLayout(HBoxlayout) ;

        QHBoxLayout *Main_Layout= new QHBoxLayout(Carte_contact[i]) ;
        Main_Layout->addWidget(imageLabel) ;
        Main_Layout->addLayout(main_layout_Carte_contact) ;
        Carte_contact[i]->setLayout(Main_Layout);

        main_layout_Container_Contacts_Cartes->addWidget(Carte_contact[i]);
    }

    main_layout_Container_Contacts_Cartes->setContentsMargins(5,5,5,0) ;

    Container_Contacts_Cartes->setObjectName("container_contacts") ;
    Container_Contacts_Cartes->setStyleSheet(
        "#Carte_contact {"
        "   background-color: #D4EFDF;"
        "   padding: 10px;"
        "   border-radius: 10px;"
        "   border: 2px solid #B8E6CE;"

        "}"
        "#Carte_contact:hover {"
        "   background-color: #A9DFBF;"
        "}"
        "#container_contacts{"
        "background-color: #E6F4F1 ;"
        "border: 2px solid #D0CFCB;"
        "border-radius: 8px;}"

        );

    main_layout_Container_Contacts_Cartes->setSpacing(3) ;

    ScrollArea_Contacts_Cartes = new QScrollArea();
    ScrollArea_Contacts_Cartes->setWidget(Container_Contacts_Cartes);
    ScrollArea_Contacts_Cartes->setWidgetResizable(true);
    ScrollArea_Contacts_Cartes->setMaximumWidth(350);

    // Conteneur des messages
    Container_messages = new QWidget(this);
    Container_messages->setStyleSheet("background-color: #ECE5DD;");


    Container_messages_layout = new QVBoxLayout(Container_messages);
    //------------------------------------------------------------------------------------

    WelcomeMessage = new QWidget(this) ;
    QVBoxLayout *Layout_WelcomeMessage= new QVBoxLayout(WelcomeMessage) ;

    QLabel *labelLogo = new QLabel(WelcomeMessage) ;
    labelLogo->setPixmap(QPixmap(":/icons/logo.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    labelLogo->setAlignment(Qt::AlignCenter);
//    #128C7E
    QLabel *labelAppName = new QLabel("Qonnect", WelcomeMessage);
    labelAppName->setStyleSheet("font-size: 24px; font-weight: bold; color: #128C7E ;");
    labelAppName->setAlignment(Qt::AlignCenter);

    // Texte de bienvenue
    QLabel *labelBienvenue = new QLabel("Bienvenue , " + R->Recuperer_info(id_moi)["nom"].toString()+" " +R->Recuperer_info(id_moi)["prenom"].toString() + " !",WelcomeMessage);


    labelBienvenue->setStyleSheet("font-size: 20px; font-weight: bold; color: #2E7D32;");
    labelBienvenue->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout1 = new QHBoxLayout ;
    QLabel *iconLabel1 = new QLabel(WelcomeMessage) ;
    QPixmap iconPixmap1(":/icons/bell.png") ;
    iconPixmap1 = iconPixmap1.scaled(17,17, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconLabel1->setPixmap(iconPixmap1);
    QLabel *labelInvitations = new QLabel(QString("%1 nouvelles invitations").arg(R->nbr_invitation(id_moi)),WelcomeMessage);
    layout1->addWidget(iconLabel1) ;
    layout1->addSpacing(5) ;
    layout1->addWidget(labelInvitations) ;
    layout1->addStretch() ;


    QHBoxLayout *layout2 = new QHBoxLayout ;
    QLabel *iconLabel2 = new QLabel(WelcomeMessage) ;
    QPixmap iconPixmap2(":/icons/chat.png") ;
    iconPixmap2 = iconPixmap2.scaled(17,17, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconLabel2->setPixmap(iconPixmap2);
    QLabel *labelMessages = new QLabel(QString("%1 messages non lus").arg(R->nbr_msg_unread(id_moi)),WelcomeMessage);
    layout2->addWidget(iconLabel2) ;
    layout2->addSpacing(5) ;
    layout2->addWidget(labelMessages) ;
    layout2->addStretch() ;


    labelInvitations->setStyleSheet("font-size: 14px;");
    labelMessages->setStyleSheet("font-size: 14px;");

    Layout_WelcomeMessage->addWidget(labelLogo);
    Layout_WelcomeMessage->addWidget(labelAppName) ;
    Layout_WelcomeMessage->addSpacing(30);
    Layout_WelcomeMessage->addWidget(labelBienvenue);
    Layout_WelcomeMessage->addSpacing(5);
    Layout_WelcomeMessage->addLayout(layout1);
    Layout_WelcomeMessage->addLayout(layout2);
    Layout_WelcomeMessage->addSpacing(20);

    WelcomeMessage->setLayout(Layout_WelcomeMessage) ;


    Container_messages_layout->addWidget(WelcomeMessage,0,Qt::AlignCenter) ;
    //--------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------

    Container_messages_layout->setAlignment(Qt::AlignTop);


    // La zone d'écriture des messages
    Container_input = new QWidget();
    Container_input->setStyleSheet(
        "background-color: #FFFFFF;"
        "border: 1px solid #DDDDDD;"
        "border-radius: 8px;"
        "margin-left : 10px ;"
        );
    Container_input->setVisible(false);

    // QLineEdit pour envoyer des messages
    Zone_input_msg = new QLineEdit(Container_input);
    Zone_input_msg->setPlaceholderText("Écrire un message...");
    Zone_input_msg->setMinimumSize(90, 45);
    Zone_input_msg->setStyleSheet(
        "background: transparent;"
        "border: none;"
        "padding: 5px;"
        "margin-right: 8px;"
        "margin-left: 8px;"
        "font-size: 12px;"
        );

    // Button envoyer
    QPushButton *Button_send = new QPushButton(Container_input);
    QIcon icone_Button_send(":/icons/paper-plane.png");
    Button_send->setIcon(icone_Button_send);
    Button_send->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   border: none;"
        "   margin-left: 5px;"
        "   min-width: 35px;"
        "   min-height: 35px;"
        "   max-width: 35px;"
        "   max-height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(18, 140, 126, 0.1);"
        "   border-radius: 17px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(18, 140, 126, 0.2);"
        "}"
        );

    Button_send->setIconSize(QSize(18, 18));

     disconnect(Button_send, &QPushButton::clicked, nullptr, nullptr);

    connect(Button_send, &QPushButton::clicked, this, [this]() {


        // Check if a contact is selected and the input is not empty
        if ( id_contact_selectionne!=-1 && !Zone_input_msg->text().isEmpty()) {
            // Insert the message into the database
            I->Inserer_message(id_moi, id_contact_selectionne, Zone_input_msg->text());

            I->signal_msg_sent(id_contact_selectionne) ;  //un nouveau message  , il faut actualiser
            // Clear the input field
            Zone_input_msg->clear();

            // Reload messages to show the newly sent message
            chargerMessages(id_contact_selectionne);
            Container_messages->update();
            ScrollArea_Container_Messages->viewport()->update();
            scrollToBottom() ;

            // Update the contacts list to show the new last message
            actualiserContacts();
        }
    });
    // Button pour un message vocal
    QPushButton *Button_msg_vocal = new QPushButton(Container_input);
    QIcon icone_Button_msg_vocal(":/icons/microphone.png");
    Button_msg_vocal->setIcon(icone_Button_msg_vocal);
    Button_msg_vocal->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   border: none;"
        "   margin-right: 5px;"
        "   min-width: 35px;"
        "   min-height: 35px;"
        "   max-width: 35px;"
        "   max-height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(18, 140, 126, 0.1);"
        "   border-radius: 17px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(18, 140, 126, 0.2);"
        "}"
        );
    Button_msg_vocal->setIconSize(QSize(18, 18));

    // Connexions pour les cartes de contact
    for(int i = 0; i < Nbr_Contacts; i++) {

        connect(Carte_contact[i], &Qwidget_cliquable::clicked, this, [=]() {
            //pour le style des buttons
            Amis->setChecked(false);
            profil->setChecked(false);
            rechercher->setChecked(false);
            invitation->setChecked(false) ;
            logout->setChecked(false);
            Carte_contact[i]->setchecked(true) ;


            // style de l'etat active des cartes
            if(index_selectionee >= 0 && index_selectionee < Carte_contact.size())
            Carte_contact[index_selectionee]->setStyleSheet(
                "#Carte_contact {"
                "   background-color: #D4EFDF;"
                "   padding: 10px;"
                "   border-radius: 10px;"

                "}"
                "#Carte_contact:hover {"
                "   background-color: #A9DFBF;"
                "}"
                );

            Carte_contact[i]->setStyleSheet(
                "#Carte_contact {"
                "   background-color: #82E0AA ;"
                "   padding: 10px;"
                "   border-radius: 10px;"

                "}"
                "#Carte_contact:hover {"
                "   background-color: #A9DFBF;"
                "}"
                );

            index_selectionee = i ;

            int id_contact = Contacts_Cartes[i]["id_utilisateur"].toInt() ;

            //marquer les messages comme lus
          if(R->nbr_msg_unread(id_moi,id_contact)!=0){
            if(I->Update_msg_read(id_moi,id_contact)) {
                actualiserContacts() ;
            }
            I->signal_msg_read(id_contact) ;
          }

            // Ne rien faire si c'est le même contact
            if (id_contact_selectionne == id_contact) {
                return;
            }

            id_contact_selectionne = id_contact;

            switchToMessagesView(id_contact) ;
            scrollToBottom() ;
        });
    }

    ScrollArea_Container_Messages->setWidget(Container_messages);
    ScrollArea_Container_Messages->setWidgetResizable(true);
    scrollToBottom() ;

    // Layout pour organiser LineEdit, Button_send et Button_msg_vocal
    QHBoxLayout *layout_Container_input = new QHBoxLayout(Container_input);
    layout_Container_input->addWidget(Button_send);
    layout_Container_input->addWidget(Zone_input_msg);
    layout_Container_input->addWidget(Button_msg_vocal);


    //--------------------------------------------------------------------------------------------
    QVBoxLayout *layout_ScrollArea_Messages_layout_Container_input = new QVBoxLayout();

    QString styleBouton = R"(
    QPushButton {
        background-color: transparent;
        border: none;


    }
    QPushButton:hover {
        background-color: #d3d3d3;
        border-radius: 10px;
    }

   QPushButton:pressed {
      background-color: #b0b0b0;
    }

    QPushButton:checked {
        background-color: #c2c2c2;
        border-radius: 10px;
    }
  )";


    connect(rechercher, &QPushButton::clicked, this, [=]() {
        //ppour le style des buttons
        rechercher->setChecked(true);
        profil->setChecked(false);
        invitation->setChecked(false) ;
        Amis->setChecked(false);



        if(index_selectionee >= 0 && index_selectionee < Carte_contact.size()) Carte_contact[index_selectionee]->setchecked(false);

        id_contact_selectionne = -1 ;


        carteTop->hide();
        Container_input->hide();
        Amis->setChecked(false);
        profil->setChecked(false);
        zone_stack->setCurrentIndex(1);
        zone_stack->setStyleSheet("margin-top:5px ;background-color: #ECE5DD;border-radius:5px ;") ;

        cleanMessageContainer() ;
    });


    connect(Amis, &QPushButton::clicked, this, [=]() {
        //pour le style des buttons
        Amis->setChecked(true);
        profil->setChecked(false);
        rechercher->setChecked(false);
        invitation->setChecked(false) ;
        if(index_selectionee >= 0 && index_selectionee < Carte_contact.size()) Carte_contact[index_selectionee]->setchecked(false);

        id_contact_selectionne = -1 ;
        carteTop->hide();
        Container_input->hide();

        cleanMessageContainer() ;

        if(animationAmiStart){
            animationAmis->stop();
            Amis->setStyleSheet(styleBouton) ;
        }


        afficherListeAmis();

        zone_stack->setStyleSheet("margin-top:5px ;background-color: #ECE5DD;border-radius:5px ;") ;


    });

    connect(profil, &QPushButton::clicked, this, [=]() {
        //pour le style des buttons
        profil->setChecked(true);
        Amis->setChecked(false);
        invitation->setChecked(false) ;
        rechercher->setChecked(false);
        if(index_selectionee >= 0 && index_selectionee < Carte_contact.size()) Carte_contact[index_selectionee]->setchecked(false);



        id_contact_selectionne = -1 ;
        carteTop->hide();
        Container_input->hide();
        zone_stack->setCurrentWidget(Mon_profil);
        zone_stack->setStyleSheet("margin-top:5px ;background-color: #ECE5DD;border-radius:5px ;") ;


    });

    connect(logout,&QPushButton::clicked,this,[=](){
        rechercher->setChecked(false);
        Amis->setChecked(false);
        profil->setChecked(false);
        invitation->setChecked(false) ;

        id_contact_selectionne = -1 ;


        I->Status_HorsLigne(id_moi) ;

        this->close() ;
        MainWindow *M = new MainWindow  ;
        M->show() ;

    });

    connect(invitation,&QPushButton::clicked,this,[=](){
        // si l'animation commence
        if(animationInvitationStart){
           animationInvitation->stop();
           invitation->setStyleSheet(styleBouton) ;
        }

        profil->setChecked(false);
        Amis->setChecked(false);
        invitation->setChecked(true) ;
        rechercher->setChecked(false);

        if(index_selectionee >= 0 && index_selectionee < Carte_contact.size()) Carte_contact[index_selectionee]->setchecked(false);

        id_contact_selectionne = -1 ;
        cleanMessageContainer() ;

        carteTop->hide();
        Container_input->hide();

        afficherListInvitation() ;


    });



    rechercher->setIcon(QIcon(":/icons/rechercher.png"));
    Amis->setIcon(QIcon(":/icons/group.png"));
    profil->setIcon(QIcon(":/icons/profile_user.png"));
    invitation->setIcon(QIcon(":/icons/inviter.png")) ;
    logout->setIcon(QIcon(":/icons/logout.png")) ;

    // Taille des icônes
    rechercher->setIconSize(QSize(20, 20));
    Amis->setIconSize(QSize(20, 20));
    invitation->setIconSize(QSize(20,20)) ;
    profil->setIconSize(QSize(20, 20));
    logout->setIconSize(QSize(20, 20));

    for (QPushButton *button : {rechercher, Amis, invitation, profil, logout}) {
        button->setFixedSize(36, 36);
    }

    rechercher->setStyleSheet(styleBouton);
    Amis->setStyleSheet(styleBouton);
    profil->setStyleSheet(styleBouton);
    logout->setStyleSheet(styleBouton);
    invitation->setStyleSheet(styleBouton) ;

    QHBoxLayout* boutonsLayout = new QHBoxLayout;
    boutonsLayout->setSpacing(14);
    boutonsLayout->addWidget(rechercher);
    boutonsLayout->addWidget(Amis);
    boutonsLayout->addWidget(invitation) ;
    boutonsLayout->addWidget(profil);
    boutonsLayout->addWidget(logout) ;

    QHBoxLayout *header= new QHBoxLayout ;
    header->addWidget(carteTop) ;
    header->addLayout(boutonsLayout) ;


    layout_ScrollArea_Messages_layout_Container_input->addLayout(header) ;
    layout_ScrollArea_Messages_layout_Container_input->addWidget(zone_stack);
    layout_ScrollArea_Messages_layout_Container_input->addWidget(Container_input);


    main_layout->addWidget(ScrollArea_Contacts_Cartes, 1);
    main_layout->addLayout(layout_ScrollArea_Messages_layout_Container_input, 3);


    // Ajouter le widget principal au layout
    mainVerticalLayout->addWidget(mainContentWidget);

    setLayout(mainVerticalLayout);

}

void Interface_user::chargerMessages(int id_contact) {

    if (id_contact == -1) return;

    cleanMessageContainer() ;

    recuperer_database R ;

    QList<QVariantMap> Messages = R.Recuperer_messages(id_moi, id_contact);

    for (auto msg : Messages) {
        int id_message(msg["id_message"].toInt()) ;
        int id_sender(msg["id_sender"].toInt()) ;
        QString content(msg["content"].toString());
        QString send_time(msg["send_time"].toDateTime().toString("dd/MM/yyyy à HH:mm"));
        bool this_user_sender = msg["this_user_sender"].toBool();
        bool message_read = msg["message_read"].toBool();
        QString Emoji = msg["emoji"].toString().isEmpty() ? "none":msg["emoji"].toString() ;

        Message* message = new Message(id_message,content, send_time, this_user_sender,Emoji,id_sender,message_read);

        QHBoxLayout *Layout_Position_msg = new QHBoxLayout();
        Layout_Position_msg->setContentsMargins(0, 0, 0, 0);
        Layout_Position_msg->setSpacing(0);

        if (this_user_sender) {
            Layout_Position_msg->addStretch();
            Layout_Position_msg->addWidget(message);
        } else {
            Layout_Position_msg->addWidget(message);
            Layout_Position_msg->addStretch();
        }


        Container_messages_layout->addLayout(Layout_Position_msg);
    }
    Container_messages_layout->setSpacing(0);
    Container_messages_layout->setContentsMargins(0, 0, 0, 0);
    Container_messages_layout->addStretch() ;

    ScrollArea_Container_Messages->setContentsMargins(0, 0, 0, 0);
    ScrollArea_Container_Messages->setStyleSheet("QScrollArea { padding: 0px; margin: 0px; }");

}

void Interface_user::switchToMessagesView(int id_contact) {

      cleanMessageContainer() ;

      chargerMessages(id_contact);

      carteTop->mise_a_jour(id_contact) ;
      carteTop->setVisible(true);
      Container_input->setVisible(true);

      zone_stack->setCurrentIndex(0);

      zone_stack->setStyleSheet("background-color: #ECE5DD;border-radius:5px ;margin-top:0px ;");

      scrollToBottom() ;

       if(zone_recherche) {
          zone_recherche->hide();
        }


}

void Interface_user::afficher_message_vide(int id_contact){
    zone_stack->setCurrentIndex(0) ;

    cleanMessageContainer() ;

    recuperer_database db ;
    QMap info_user= db.Recuperer_info(id_contact) ;

    QString str1= "Aucun message" ;
    QString str2= "soyez le premier à envoyer un message à " ;
    QString str3=info_user["nom"].toString()+" "+info_user["prenom"].toString() ;

    QLabel *Label1 = new QLabel(str1,Container_messages) ;
    QLabel *Label2 = new QLabel(str2,Container_messages) ;
    QLabel *Label3 = new QLabel(str3,Container_messages) ;

    Label1->setAlignment(Qt::AlignCenter);
    Label2->setAlignment(Qt::AlignCenter);
    Label3->setAlignment(Qt::AlignCenter);

    Label1->setStyleSheet("font-size: 18px; font-weight: bold; color: #444;");
    Label2->setStyleSheet("font-size: 14px; color: #666;");
    Label3->setStyleSheet("font-size: 14px; font-style: italic; color: #54656F;");


    QVBoxLayout *Layout= new QVBoxLayout(Container_messages) ;
    Layout->addStretch() ;
    Layout->addWidget(Label1) ;
    Layout->addWidget(Label2) ;
    Layout->addWidget(Label3) ;
   Layout->addStretch() ;

    QHBoxLayout *centrer_horizontale=new QHBoxLayout(Container_messages) ;
    centrer_horizontale->addStretch() ;
    centrer_horizontale->addLayout(Layout) ;
    centrer_horizontale->addStretch() ;

    Container_messages_layout->addLayout(centrer_horizontale) ;

    carteTop->mise_a_jour(id_contact) ;
    carteTop->setVisible(true);
    Container_input->setVisible(true);

}

void Interface_user::afficherListeAmis() {

    clean(Layout_Amis) ;

    recuperer_database db;
    QList<QVariantMap> listeAmis = db.Recuperer_Amis(id_moi);

    if (listeAmis.isEmpty()) {
        Zone_Amis_vide() ;

    } else {
        for (const QVariantMap &ami : listeAmis) {
            // Création de la carte ami avec bordure légère
            QWidget *carteAmi = new QWidget();
            carteAmi->setObjectName("carteAmi");
            carteAmi->setStyleSheet(
                "#carteAmi {"
                "   background-color : rgba(255, 255, 255, 150);"
                "   border : 1px solid #E0E0E0;"
                "   border-radius : 8px ;"
                "   padding : 10px ;"

                "}"
                "#carteAmi:hover {"
                "   background-color: rgba(245, 245, 245, 200);"
                "   border: 1px solid #BDBDBD;"
                "}"
                );

            QHBoxLayout *layoutCarte = new QHBoxLayout(carteAmi);
            layoutCarte->setContentsMargins(5,5,5,5);
            layoutCarte->setSpacing(12);

            // Photo de profil
            QLabel *photoLabel = new QLabel();
            photoLabel->setFixedSize(40, 40);
            photoLabel->setStyleSheet(
                "border-radius: 27px;"
                "border: none;"
                );

            QString photoPath = ami["photo_profil"].toString();
            QPixmap photo;

            if (!photoPath.isEmpty() && QFile::exists(photoPath)) {
                photo.load(photoPath);
            } else {
                photo.load(":/icons/profile.png");
            }

            QPixmap circularPixmap(photo.size());
            circularPixmap.fill(Qt::transparent);

            QPainter painter(&circularPixmap);
            painter.setRenderHint(QPainter::Antialiasing, true);
            QPainterPath path;
            path.addEllipse(photo.rect());
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, photo);

            photoLabel->setPixmap(circularPixmap.scaled(37, 37, Qt::KeepAspectRatio, Qt::SmoothTransformation));

            // Infos texte
            QVBoxLayout *layoutInfos = new QVBoxLayout();
            layoutInfos->setSpacing(3);
            layoutInfos->setContentsMargins(0, 0, 0, 0);

            QLabel *nomLabel = new QLabel(ami["prenom"].toString() + " " + ami["nom"].toString());
            nomLabel->setStyleSheet(
                "font-weight: bold;"
                "font-size: 14px;"
                "color: #333333;"
                "background-color: transparent;"
                );

            QString statusText = ami["status"].toBool() ? "En ligne" : "Hors ligne";
            QString statusColor = ami["status"].toBool() ? "#4CAF50" : "#888888";

            QLabel *statusLabel = new QLabel(statusText);
            statusLabel->setStyleSheet(
                QString("font-size: 12px; color: %1; background-color: transparent;").arg(statusColor)
                );

            layoutInfos->addWidget(nomLabel);
            layoutInfos->addWidget(statusLabel);


            QPushButton *envoyer = new QPushButton();
           envoyer->setIcon(QIcon(":/icons/envoyer-email.png")); // Chemin vers votre icône
           envoyer->setIconSize(QSize(24, 24));
           envoyer->setFixedSize(30, 30);
           envoyer->setStyleSheet(
                "QPushButton {"
                "   background-color: transparent;"
                "   border: none;"
                "   border-radius: 15px;"
                "}"
                "QPushButton:hover {"
                "   background-color: rgba(0, 0, 0, 10);"
                "}"
                "QPushButton:pressed {"
                "   background-color: rgba(0, 0, 0, 20);"
                "}"
                );
            envoyer->setCursor(Qt::PointingHandCursor);



            connect(envoyer, &QPushButton::clicked, this, [this, ami]() {
                bool trouve = false ;  // est ce que l'ami est dans mes contactS
                for (int i = 0; i < Contacts_Cartes.size(); i++) {
                    if (Contacts_Cartes[i]["id_utilisateur"] == ami["id"].toInt()) {
                        trouve = true ;
                        emit Carte_contact[i]->clicked();
                        break;
                    }
                }

                if(!trouve){
                    afficher_message_vide(ami["id"].toInt()) ;
                }

            });


            // Assemblage des éléments
            layoutCarte->addWidget(photoLabel);
            layoutCarte->addLayout(layoutInfos,1);
            layoutCarte->addWidget(envoyer, 0, Qt::AlignRight);

            Layout_Amis->addWidget(carteAmi);


        }
    }


    Layout_Amis->addStretch();

    zone_stack->setCurrentIndex(2);
    zone_stack->setStyleSheet("background-color: #ECE5DD;border-radius:5px ;margin-top:0px ;");

}

void Interface_user::afficherListInvitation() {

    clean(layoutInvitations);

    QList<QVariantMap> invitations = R->Recuperer_invitations(id_moi);

    if (invitations.isEmpty()) {
        Zone_Invitation_vide() ;
    }
    else{
    for (auto invitation : invitations) {
        if (invitation["id_otherUser"].toInt() == id_moi) {
            QVariantMap info_user = R->Recuperer_info(invitation["id_sender"].toInt());

            QWidget *carteInvitation = new QWidget;
            carteInvitation->setObjectName("carteInvitation");

            carteInvitation->setStyleSheet(
                "#carteInvitation {"
                "   background-color: rgba(255, 255, 255, 150);"
                "   border: 1px solid #E0E0E0;"
                "   border-radius: 8px;"
                "   padding: 10px;"
                "}"
                "#carteInvitation:hover {"
                "   background-color: rgba(245, 245, 245, 200);"
                "   border: 1px solid #BDBDBD;"
                "}"
                );


            QHBoxLayout *layoutCarte = new QHBoxLayout(carteInvitation);
            layoutCarte->setContentsMargins(5, 5, 5, 5);
            layoutCarte->setSpacing(12);

            // photo
            QLabel *photoLabel = new QLabel;
            photoLabel->setFixedSize(40, 40);

            QPixmap photo;
            if (!info_user["photo_profil"].toString().isEmpty() && QFile::exists(info_user["photo_profil"].toString()))
                photo.load(info_user["photo_profil"].toString());
            else
                photo.load(":/icons/profile.png");

            QPixmap circular(photo.size());
            circular.fill(Qt::transparent);

            QPainter painter(&circular);
            painter.setRenderHint(QPainter::Antialiasing, true);
            QPainterPath path;
            path.addEllipse(photo.rect());
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, photo);

            photoLabel->setPixmap(circular.scaled(37, 37, Qt::KeepAspectRatio, Qt::SmoothTransformation));

            // infos
            QVBoxLayout *layoutInfos = new QVBoxLayout;
            layoutInfos->setSpacing(2);
            layoutInfos->setContentsMargins(0, 0, 0, 0);

            QLabel *nomLabel = new QLabel(info_user["prenom"].toString() + " " + info_user["nom"].toString(),carteInvitation);
            nomLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333333;");
            QLabel *genreLabel = new QLabel(info_user["genre"].toString()=="H" ? "Homme":"Femme" ,carteInvitation);
            genreLabel->setStyleSheet("font-size: 12px; color: #777777;");

            layoutInfos->addWidget(nomLabel);
            layoutInfos->addWidget(genreLabel);

            // boutons
            QPushButton *accepterButton = new QPushButton;
            accepterButton->setIcon(QIcon(":/icons/checkmark.png"));
            accepterButton->setToolTip("Accepter");
            accepterButton->setCursor(Qt::PointingHandCursor);
            accepterButton->setIconSize(QSize(20,20)) ;
            accepterButton->setFixedSize(26, 26);
            accepterButton->setStyleSheet(
                "QPushButton {"
                "   background-color: transparent;"
                "   border: none;"
                "   border-radius: 13px;"
                "}"
                "QPushButton:hover { background-color: #D3D3D3; }"

                "QPushButton:pressed {"
                "  background-color: #A9A9A9;"
                "}"
                );

            QPushButton *refuserButton = new QPushButton;
            refuserButton->setIcon(QIcon(":/icons/cancel.png"));
            refuserButton->setToolTip("Refuser");
            refuserButton->setCursor(Qt::PointingHandCursor);
            refuserButton->setFixedSize(26, 26);
            refuserButton->setIconSize(QSize(20,20)) ;

            refuserButton->setStyleSheet(
                "QPushButton {"
                "   background-color: transparent;"
                "   border: none;"
                "   border-radius: 13px;"
                "}"
                "QPushButton:hover { background-color: #D3D3D3 ; }"

                "QPushButton:pressed {"
                "  background-color: #A9A9A9;"
                "}"
                );

            // actions
            connect(accepterButton, &QPushButton::clicked, this, [=]() {
                I->ValiderInvitation(invitation["id_sender"].toInt(), id_moi);
                I->signal_new_friend(invitation["id_sender"].toInt()) ;

                QTimer::singleShot(400,[=](){
                    carteInvitation->hide() ;
                }) ;
            });

            connect(refuserButton, &QPushButton::clicked, this, [=]() {
                I->RefuserInvitation(invitation["id_sender"].toInt(), id_moi);

                QTimer::singleShot(400,[=](){
                    carteInvitation->hide() ;
                }) ;

            });

            QHBoxLayout *btnLayout = new QHBoxLayout;
            btnLayout->addWidget(accepterButton);
            btnLayout->addWidget(refuserButton);
            btnLayout->setSpacing(6);
            btnLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

            layoutCarte->addWidget(photoLabel);
            layoutCarte->addLayout(layoutInfos, 1);
            layoutCarte->addLayout(btnLayout);

            layoutInvitations->addWidget(carteInvitation);
        }
    }

    layoutInvitations->addStretch();
    }

    zone_stack->setCurrentIndex(3);
    zone_stack->setStyleSheet("background-color: #ECE5DD;border-radius:5px ;margin-top:0px ;");


}

void Interface_user::showMessageComponents(bool show)
{
    if(carteTop) carteTop->setVisible(show);
    if(Container_input) Container_input->setVisible(show);
}

void Interface_user::actualiserContacts()
{

    clean(main_layout_Container_Contacts_Cartes) ;

    // Vidage du vecteur de cartes
    Carte_contact.clear();

    // Récupération des contacts depuis la base de données
    recuperer_database db;

    Contacts_Cartes = db.Recuperer_Contacts_DernierMessage(id_moi);

    if (Contacts_Cartes.isEmpty()) {
        Zone_CarteContact_vide() ;
    }

    // Recréation des cartes de contact
    Carte_contact.resize(Contacts_Cartes.size());

    for(int i = 0; i < Contacts_Cartes.size(); i++) {

        Carte_contact[i] = new Qwidget_cliquable(Container_Contacts_Cartes);
        Carte_contact[i]->setObjectName("Carte_contact");
        Carte_contact[i]->setAttribute(Qt::WA_StyledBackground, true);

        //l'avatar
        QLabel *imageLabel = new QLabel(Carte_contact[i]);
        imageLabel->setFixedSize(40, 40);

        // Gestion de l'image de profil
        QString photoPath = Contacts_Cartes[i]["photo_profil"].toString();
        QPixmap pixmap;

        if (!photoPath.isEmpty() && QFile::exists(photoPath)) {
            if (!pixmap.load(photoPath)) {
                pixmap.load(":/icons/profile.png");
            }
        } else {
            pixmap.load(":/icons/profile.png");
        }

        // Rendre l'image circulaire
        QPixmap circularPixmap(40, 40);
        circularPixmap.fill(Qt::transparent);
        QPainter painter(&circularPixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QBrush(pixmap.scaled(40, 40, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));
        painter.drawEllipse(0, 0, 40, 40);
        imageLabel->setPixmap(circularPixmap);

        // Style de la carte
        Carte_contact[i]->setStyleSheet(
            "#Carte_contact {"
            "   background-color: #D4EFDF;"
            "   padding: 10px;"
            "   border-radius: 10px;"
            "   margin: 2px;"
            "}"
            "#Carte_contact:hover {"
            "   background-color: #A9DFBF;"
            "}"

            );

        Carte_contact[i]->setMaximumHeight(70);

        // Layout pour le nom et l'heure
        QHBoxLayout *infoLayout = new QHBoxLayout();

        QLabel *nameLabel = new QLabel(
            Contacts_Cartes[i]["prenom"].toString() + " " + Contacts_Cartes[i]["nom"].toString(),
            Carte_contact[i]
            );
        nameLabel->setStyleSheet("font-weight: bold; font-size: 12px;");

        QLabel *timeLabel = new QLabel(
            Contacts_Cartes[i]["send_time"].toDateTime().toString("dd/MM/yyyy à HH:mm"),
            Carte_contact[i]
            );
        timeLabel->setStyleSheet("font-size: 10px; color: #666;");

        infoLayout->addWidget(nameLabel);
        infoLayout->addStretch();
        infoLayout->addWidget(timeLabel);

        // Dernier message
        QLabel *lastMsgLabel = new QLabel(
            Contacts_Cartes[i]["content"].toString(),
            Carte_contact[i]
            );
        lastMsgLabel->setStyleSheet("font-size: 11px; color: #555;");
        lastMsgLabel->setWordWrap(true);

        QHBoxLayout *HBoxlayout= new QHBoxLayout ;
        HBoxlayout->addWidget(lastMsgLabel) ;

        int nbr_msg = db.nbr_msg_unread(id_moi, Contacts_Cartes[i]["id_utilisateur"].toInt());
        if(nbr_msg!=0){
            QLabel *nbr_msg_unread = new QLabel(QString::number(nbr_msg),Carte_contact[i]) ;
            nbr_msg_unread->setFixedSize(20, 20);
            nbr_msg_unread->setAlignment(Qt::AlignCenter);
        nbr_msg_unread->setStyleSheet("font-size: 10px;color: white ;border-radius : 10px ;background-color : #25D366 ;");
        HBoxlayout->addWidget(nbr_msg_unread) ;
        }

        // Layout principal de la carte
        QVBoxLayout *mainCardLayout = new QVBoxLayout();
        mainCardLayout->addLayout(infoLayout);
        mainCardLayout->addLayout(HBoxlayout);

        QHBoxLayout *finalLayout = new QHBoxLayout(Carte_contact[i]);
        finalLayout->addWidget(imageLabel);
        finalLayout->addLayout(mainCardLayout);
        Carte_contact[i]->setLayout(finalLayout);



        // Connexion du signal
        connect(Carte_contact[i], &Qwidget_cliquable::clicked, this, [this, i]() {
            rechercher->setChecked(false);
            Amis->setChecked(false);
            profil->setChecked(false);
            logout->setChecked(false);
            Carte_contact[i]->setchecked(true) ;

            // Style de l'etat active d'une carte
            if(index_selectionee >= 0 && index_selectionee < Carte_contact.size()){
            Carte_contact[index_selectionee]->setStyleSheet(
                "#Carte_contact {"
                "   background-color: #D4EFDF;"
                "   padding: 10px;"
                "   border-radius: 10px;"
                "   margin: 2px;"
                "}"
                "#Carte_contact:hover {"
                "   background-color: #A9DFBF;"
                "}"
                );
            }

            index_selectionee = i ;


            Carte_contact[i]->setStyleSheet(
                "#Carte_contact {"
                "   background-color: #D4EFDF;"
                "   padding: 10px;"
                "   border-radius: 10px;"
                "   margin: 2px;"
                "}"
                "#Carte_contact:hover {"
                "   background-color: #A9DFBF;"
                "}"
                );


            int id_contact = Contacts_Cartes[i]["id_utilisateur"].toInt();

            if(R->nbr_msg_unread(id_moi,id_contact)!=0){
                //marquer les msg comme lus
                if(I->Update_msg_read(id_moi,id_contact)) {
                    actualiserContacts() ;
                }
                I->signal_msg_read(id_contact) ;
            }

            if (id_contact_selectionne == id_contact) return ; //si la carte clique est le meme que la carte actuel

            id_contact_selectionne = id_contact;

            index_selectionee= i ;

            carteTop->mise_a_jour(id_contact);



            switchToMessagesView(id_contact);
            scrollToBottom() ;



        });

        // Ajout au layout principal
        main_layout_Container_Contacts_Cartes->addWidget(Carte_contact[i]);
    }


    main_layout_Container_Contacts_Cartes->setContentsMargins(5,5,5,0) ;

    Container_Contacts_Cartes->setObjectName("container_contacts") ;
    Container_Contacts_Cartes->setStyleSheet(
        "#Carte_contact {"
        "   background-color: #D4EFDF;"
        "   padding: 10px;"
        "   border-radius: 10px;"
        "   border: 2px solid #B8E6CE;"

        "}"
        "#Carte_contact:hover {"
        "   background-color: #A9DFBF;"
        "}"
        "#container_contacts{"
        "background-color: #E6F4F1 ;"
        "border: 2px solid #D0CFCB;"
        "border-radius: 8px;}"

        );

    main_layout_Container_Contacts_Cartes->setSpacing(3) ;
    main_layout_Container_Contacts_Cartes->setSpacing(3) ;


    // 4. Mise à jour de l'affichage
    Container_Contacts_Cartes->adjustSize();
    if (ScrollArea_Contacts_Cartes) {
        ScrollArea_Contacts_Cartes->viewport()->update();
    }

    qDebug() << "Contacts actualisés avec succès. Nombre de contacts:" << Carte_contact.size();




}

void Interface_user::clean(QLayout *layout){

    if (!layout) return;

    while (QLayoutItem* item = layout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }

        if (item->layout()) {
            clean(item->layout());
        }

        delete item;

    }
}

void Interface_user::cleanMessageContainer() {




    QWidget* nouveauContainer = new QWidget();

    QVBoxLayout* nouveauLayout = new QVBoxLayout();

    nouveauContainer->setLayout(nouveauLayout);

    ScrollArea_Container_Messages->takeWidget();
    ScrollArea_Container_Messages->setWidget(nouveauContainer);
    Container_messages = nouveauContainer;
    Container_messages_layout = nouveauLayout;
}

void Interface_user::scrollToBottom()
{
    // Force update of layout and widgets
    ScrollArea_Container_Messages->updateGeometry();
    ScrollArea_Container_Messages->viewport()->update();

    QScrollBar* bar = ScrollArea_Container_Messages->verticalScrollBar();
    if (bar) {
        bar->setValue(bar->maximum());
        // Alternative plus robuste :
        QMetaObject::invokeMethod(bar, [bar]() {
            bar->setValue(bar->maximum());
        }, Qt::QueuedConnection);
    }
}

void Interface_user::Zone_CarteContact_vide(){


        QLabel *iconVide = new QLabel(Container_Contacts_Cartes);
        iconVide->setPixmap(QPixmap(":/icons/no-message.png").scaled(35,35, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconVide->setAlignment(Qt::AlignCenter);

        QLabel *messageVide = new QLabel("Aucun contact pour le moment !",Container_Contacts_Cartes);
        messageVide->setAlignment(Qt::AlignCenter);
        messageVide->setStyleSheet("font-size: 16px; color: #999999;");

        QHBoxLayout *HBoxlayout= new QHBoxLayout ;
        QLabel *Label1= new QLabel("Vous pouvez ",Container_Contacts_Cartes) ;
        Label1->setStyleSheet("font-size: 13px; color: #666666;");

        QPushButton *buttonRecherche= new QPushButton("chercher des utilisateurs",Container_Contacts_Cartes) ;

        QLabel *Label2 = new QLabel("pour démarrer une conversation.", Container_Contacts_Cartes);
        Label2->setStyleSheet("font-size: 13px; color: #666666;");

        HBoxlayout->addWidget(Label1) ;
        HBoxlayout->addWidget(buttonRecherche) ;
        HBoxlayout->setSpacing(0) ;
        HBoxlayout->setAlignment(Qt::AlignCenter) ;
        Label2->setAlignment(Qt::AlignCenter) ;


        buttonRecherche->setStyleSheet(
            "QPushButton {"
            "   background: none;"
            "   border: none;"
            "   color: #128C7E;"  // Couleur verte WhatsApp
            "   text-decoration: underline;"
            "   font-size: 13px;"
            "   font-weight: bold ;"
            "}"
            "QPushButton:hover {"
            "   color: #0f7a6c;"
            "   text-decoration: underline;"
            "   cursor: pointer;"
            "}"
            "QPushButton:pressed {"
            "   color: #0c6155;"
            "}"
            );


        connect(buttonRecherche,&QPushButton::clicked,this,[=](){

            emit rechercher->clicked(true) ;

        });


        main_layout_Container_Contacts_Cartes->addStretch();
        main_layout_Container_Contacts_Cartes->addWidget(iconVide);
        main_layout_Container_Contacts_Cartes->addSpacing(10);
        main_layout_Container_Contacts_Cartes->addWidget(messageVide);
        main_layout_Container_Contacts_Cartes->addLayout(HBoxlayout) ;
        main_layout_Container_Contacts_Cartes->addWidget(Label2) ;
        main_layout_Container_Contacts_Cartes->addStretch();


        Container_Contacts_Cartes->setObjectName("container_contacts") ;
           Container_Contacts_Cartes ->setStyleSheet(
            "#container_contacts{"
            "background-color: #e8e8e6;"
            "border: 2px solid #D0CFCB;"
            "border-radius: 8px;}"
            );

}

void Interface_user::Zone_Amis_vide(){

    clean(Layout_Amis) ;

    QLabel *Icone = new QLabel(Container_Amis) ;
    Icone->setPixmap(QPixmap(":/icons/no_friend.png").scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    Icone->setAlignment(Qt::AlignCenter);

    QLabel *Message = new QLabel("Vous n'avez encore aucun ami ",Container_Amis);
    Message->setAlignment(Qt::AlignCenter);
    Message->setStyleSheet("color: #5C5C5C; font-size: 16px; font-weight: 500;");

    QPushButton *BtnRecherche = new QPushButton("Chercher des amis",Container_Amis);
    BtnRecherche->setCursor(Qt::PointingHandCursor);
    BtnRecherche->setStyleSheet(
        "QPushButton {"
        "   background: none;"
        "   border: none;"
        "   color: #128C7E;"  // Couleur verte WhatsApp
        "   text-decoration: underline;"
        "   font-size: 15px;"
        "   font-weight: bold ;"
        "}"
        "QPushButton:hover {"
        "   color: #0f7a6c;"
        "   text-decoration: underline;"
        "   cursor: pointer;"
        "}"
        "QPushButton:pressed {"
        "   color: #0c6155;"
        "}"
        );

    connect(BtnRecherche,&QPushButton::clicked,this,[=](){

        emit rechercher->clicked(true) ;

    }) ;


    Layout_Amis->addStretch() ;
    Layout_Amis->addWidget(Icone);
    Layout_Amis->addWidget(Message);
    Layout_Amis->addWidget(BtnRecherche);
    Layout_Amis->addStretch() ;


}

void Interface_user::Zone_Invitation_vide() {

    clean(layoutInvitations);

    // Icône pour les invitations vides
    QLabel *Icone = new QLabel(Container_Invitations) ;
    Icone->setPixmap(QPixmap(":/icons/invitation.png").scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    Icone->setAlignment(Qt::AlignCenter);

    // Message indiquant qu'il n'y a aucune invitation
    QLabel *Message = new QLabel("Vous n'avez reçu aucune demande d'amitié pour le moment ", Container_Invitations);
    Message->setAlignment(Qt::AlignCenter);
    Message->setStyleSheet("color: #5C5C5C; font-size: 16px; font-weight: 500;");

    // Bouton ou lien d'action (facultatif)
    QPushButton *BtnRetour = new QPushButton("Envoyer des invitations", Container_Invitations);
    BtnRetour->setCursor(Qt::PointingHandCursor);
    BtnRetour->setStyleSheet(
        "QPushButton {"
        "   background: none;"
        "   border: none;"
        "   color: #128C7E;"
        "   text-decoration: underline;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   color: #0f7a6c;"
        "}"
        "QPushButton:pressed {"
        "   color: #0c6155;"
        "}"
        );

    // Action possible sur le bouton (ex: retour à l'onglet amis)
    connect(BtnRetour, &QPushButton::clicked, this, [=]() {

        emit rechercher->clicked(true) ;

    });

    // Ajout au layout
    layoutInvitations->addStretch();
    layoutInvitations->addWidget(Icone);
    layoutInvitations->addWidget(Message);
    layoutInvitations->addWidget(BtnRetour);
    layoutInvitations->addStretch();
}
