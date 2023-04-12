#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QLocalSocket>
#include <QMessageBox>
#include <QLocalServer>



class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void showMessage(QString text);
    void showPosition();

    //void down();

private:
    QLabel* pet;
    QLabel* eyes;

    QPoint m_dragStartPosition;
    QTimer* t;
    int i = 0;

    QTimer* t1;
    //QPoint position_pet;
    int count = 0;
    int i1 = 0;

    bool stop = true;
    bool right = true;
    //int direction = 0;//往右，1 待中间， 2 往左
    int count2 = 0;
    int i2 = 0;

    int count3 = 0;
    int count4 = 0;
    bool firstTime = true;

    bool rotated = false;
    bool one = true;


//Socket
private slots:
    void requestNewPosition();
    void readPosition();

private: signals:
    void positionChanged();

private:

    bool another = false;
    bool alwaysStop = false;

    QLocalSocket *socket;
    QDataStream in;

    QPoint* position;
    int i3 = 0;
    bool enabled = true;
    bool clicked = false;


//Server
private slots:
    void sendPosition();

private:
    QLocalServer *server;


};
#endif // WIDGET_H
