#include "AnimatedButtonEffect.h"
#include"QColor"

AnimatedButtonEffect::AnimatedButtonEffect(QPushButton *targetButton, QObject *parent)
    : QObject(parent), button(targetButton) {

    if (!button) return;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        angle += 2.0;
        if (angle >= 360.0) angle = 0.0;
        updateOverlay();
    });

    installOverlay();
}

void AnimatedButtonEffect::installOverlay() {
    if (!button) return;

    overlay = new OverlayWidget(button);
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    //overlay->setGeometry(button->rect());
    qDebug()<<" rect : "<<button->rect() ;
    overlay->setGeometry(QRect(0,0,150,150)) ;
    overlay->hide();

    connect(button, &QPushButton::pressed, this, [=]() {
        overlay->hide();
    });


    connect(button, &QPushButton::destroyed, this, [=]() {
        overlay = nullptr;
        timer->stop();
    });
}

void AnimatedButtonEffect::start() {
    if (timer && overlay){
        overlay->show();
        timer->start(16); // ~60 fps
    }
}

void AnimatedButtonEffect::stop() {
    if (timer)  timer->stop();
    if (overlay) overlay->hide();
}

void AnimatedButtonEffect::updateOverlay() {
    if (overlay) {
        overlay->setAngle(angle);
        overlay->update();
    }
}

// ---------------------- OverlayWidget ----------------------

AnimatedButtonEffect::OverlayWidget::OverlayWidget(QPushButton *parentButton)
    : QWidget(parentButton) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_AlwaysStackOnTop);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
}

void AnimatedButtonEffect::OverlayWidget::setAngle(qreal angle) {
    currentAngle = angle;
}

void AnimatedButtonEffect::OverlayWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect r = rect().adjusted(-2, -2, 2, 2);

    painter.save();
    painter.translate(r.center());
    painter.rotate(currentAngle);
    painter.translate(-r.center());
/*
    QConicalGradient gradient(r.center(), 0);
    gradient.setColorAt(0.0, QColor(255, 0, 255, 80));
    gradient.setColorAt(0.25, QColor(0, 255, 255, 80));
    gradient.setColorAt(0.5, QColor(255, 255, 0, 80));
    gradient.setColorAt(0.75, QColor(0, 255, 100, 80));
    gradient.setColorAt(1.0, QColor(255, 0, 255, 80));

*/
    QConicalGradient gradient(r.center(), 0);
    gradient.setColorAt(0.0, QColor(37, 211, 102, 80));   // Vert (#25D366)
    gradient.setColorAt(0.33, QColor(52, 183, 241, 80));  // Bleu (#34B7F1)
    gradient.setColorAt(0.66, QColor(236, 236, 236, 80)); // Gris (#ECECEC)
    gradient.setColorAt(1.0, QColor(37, 211, 102, 80));   // Vert boucle



   /* QConicalGradient gradient(r.center(), 0);
    gradient.setColorAt(0.0, QColor(255, 215, 0, 80));   // Or (#FFD700)
    gradient.setColorAt(0.5, QColor(255, 165, 0, 80));   // Ambre / Orange (#FFA500)
    gradient.setColorAt(1.0, QColor(255, 215, 0, 80));   // Or (#FFD700)
*/



    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(r, 12, 12);
    painter.restore();

}
