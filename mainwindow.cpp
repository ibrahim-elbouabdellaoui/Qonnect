#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QStyle>
#include"QStatusBar"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qonnect");
      // setMinimumSize(800, 600);


    // Create stacked widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setMinimumSize(600,650) ;


    setCentralWidget(stackedWidget);

    // Create login window
    loginWindow = new LoginWindow();
    stackedWidget->addWidget(loginWindow);

    // Create signup widget
    signupWidget = new SignUp();
    stackedWidget->addWidget(signupWidget);

    userInterface = nullptr;

    // Connect signals
    connect(loginWindow, &LoginWindow::loginFailed, this, &MainWindow::handleLoginFailed);
    connect(loginWindow, &LoginWindow::loginSuccessful, this, &MainWindow::showUserInterface);
    connect(loginWindow, &LoginWindow::showSignupRequested, this, &MainWindow::showSignup);
    connect(signupWidget, &SignUp::signupSuccessful, this, &MainWindow::showUserInterface);
    connect(signupWidget, &SignUp::showLoginRequested, this, &MainWindow::showLogin);

    // Show login window by default
    stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showSignup()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showLogin()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showUserInterface(const QString &email)
{
    if (userInterface) {
        delete userInterface;
    }
    userInterface = new Interface_user(email);
    stackedWidget->addWidget(userInterface);
    stackedWidget->setCurrentWidget(userInterface);

    // Connect back button if needed
    // connect(userInterface, &Interface_user::logoutRequested, this, &MainWindow::showLogin);
}


void MainWindow::handleLoginFailed(const QString &errorMessage)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("Échec de la connexion");

    msgBox.setText(errorMessage);

    msgBox.setWindowIcon(QPixmap(":/icons/Error.png").scaled(48, 48, Qt::KeepAspectRatio));

    msgBox.setIconPixmap(QPixmap(":/icons/Error.png").scaled(48, 48, Qt::KeepAspectRatio));

    msgBox.exec();

    statusBar()->showMessage(errorMessage, 5000);

    qWarning() << "Échec de connexion :" << errorMessage;

}
