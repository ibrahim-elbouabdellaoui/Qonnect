#ifndef ANIMATEDBUTTONEFFECT_H
#define ANIMATEDBUTTONEFFECT_H

#include <QObject>
#include <QPushButton>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QPointer>

class AnimatedButtonEffect : public QObject {
    Q_OBJECT

public:
    explicit AnimatedButtonEffect(QPushButton *targetButton, QObject *parent = nullptr);
    void start();
    void stop();

private:
    QPointer<QPushButton> button;
    QTimer *timer;
    qreal angle = 0.0;

    void installOverlay();
    void updateOverlay();

    class OverlayWidget : public QWidget {
    public:
        explicit OverlayWidget(QPushButton *parentButton);
        void setAngle(qreal angle);

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        qreal currentAngle = 0.0;
    };

    OverlayWidget *overlay = nullptr;
};

#endif // ANIMATEDBUTTONEFFECT_H
