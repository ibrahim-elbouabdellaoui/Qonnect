#include "carte_top.h"
#include<QPushButton>

Carte_top::Carte_top(QWidget* parent)
    : QWidget(parent)
{
    initialiserUI();
}

void Carte_top::initialiserUI()
{
    avatarLabel = new QLabel(this);
    avatarLabel->setFixedSize(46, 46);

    nomLabel = new QLabel(this);
    nomLabel->setStyleSheet("font-weight: 600; font-size: 15px; color: #1A1A1A; background: transparent;");
    nomLabel->setContentsMargins(0, 0, 0, 0);

    statutLabel = new QLabel(this);
    statutLabel->setStyleSheet("font-size: 12px; background: transparent;");
    statutLabel->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* infoLayout = new QVBoxLayout;
    infoLayout->addWidget(nomLabel);
    infoLayout->addWidget(statutLabel);
    infoLayout->setSpacing(2);
    infoLayout->setContentsMargins(10, 0, 0, 0);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(avatarLabel);
    mainLayout->addLayout(infoLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(12, 8, 12, 8);
    mainLayout->setSpacing(10);
    this->setLayout(mainLayout);

    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_StyledBackground, true);

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor("#ECE5DD"));
    this->setPalette(pal);

    this->setStyleSheet(R"(
        QWidget {
            background-color: #ECE5DD !important;
            border-radius: 10px;
        }
        QWidget:hover {
            background-color: #D9FDD3 !important;
        }
    )");

    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(6);
    shadow->setOffset(0, 2);
    shadow->setColor(QColor(0, 0, 0, 30));
    this->setGraphicsEffect(shadow);
}

void Carte_top::mise_a_jour(int id_contact)
{
    recuperer_database Recuperer;
    QVariantMap Info = Recuperer.Recuperer_info(id_contact);

    // Avatar
    QPixmap avatarPixmap;
    const QString photoPath = Info["photo_profil"].toString();
    if (photoPath.isEmpty() || !avatarPixmap.load(photoPath)) {
        avatarPixmap.load(":/icons/profile.png");
    }

    avatarPixmap = avatarPixmap.scaled(42, 42, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPixmap circularPixmap(46, 46);
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(2, 2, 42, 42);
    painter.setClipPath(path);
    painter.drawPixmap(2, 2, avatarPixmap);

    avatarLabel->setPixmap(circularPixmap);

    // Nom complet
    QString nom_complet = Info["nom"].toString() + " " + Info["prenom"].toString();
    nomLabel->setText(nom_complet);

    // Statut
    bool enLigne = Info["status"].toInt();
    QString str = "Le " + Info["last_seen"].toString();
    QString statusText = enLigne ? "● En ligne" : str;
    QString color = enLigne ? "#27AE60" : "#C0392B";
    statutLabel->setText(statusText);
    statutLabel->setStyleSheet(QString("font-size: 12px; color: %1; background: transparent;").arg(color));
}
