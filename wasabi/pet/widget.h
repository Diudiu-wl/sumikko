#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QLocalSocket>
#include <QLocalServer>
#include <QMessageBox>



class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;;

    void down();    

private:
    QLabel* pet;
    QLabel* eyes;


    QPoint m_dragStartPosition;
    QTimer* t;
    int i = 0;

    int stopwink = 0;
    int wink = 0;
    int random = 60;
    bool notWink = true;

    QTimer* t1;
    QPoint position_pet;
    int count = 0;
    int i1 = 0;

    bool stop = true;
    bool right = true;
    //int direction = 0;//往右，1 待中间， 2 往左
    int count2 = 0;

    int count3 = 0;
    int count4 = 0;

    bool firstTime = true;
    bool alwaysStop = false;
    bool clicked = false;

    bool low = false;

    int v = 70;
    int randomNumber = 70;

    int randomNumber2 = 73;
    int randomNumber3 = 73;

    int randomNumber4 = 1;
    bool set = false;

    bool rotated = false;

private slots:
    void sendPosition();
    void sendPosition2();

private:
    QLocalServer *server;
    QLocalServer *server2;


};
#endif // WIDGET_H
