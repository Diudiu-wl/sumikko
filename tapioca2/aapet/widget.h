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
    //void changeEvent(QEvent *event) override;

    void down();

    void showMessage(QString text);

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

    int count2 = 0;

    int count3 = 0;
    int count4 = 0;

    bool firstTime = true;
    bool alwaysStop = false;
    bool clicked = false;

    bool changeImage = false;

    bool low = false;

    int randomNumber1 = 70;
    int randomNumber2 = 73;
    int randomNumber3 = 73;

    //Socket
private slots:
    void requestNewPosition();
    void readPosition();
    //void displayError(QLocalSocket::LocalSocketError socketError);

private: signals:
    void positionChanged();

    void serverNotFound();

private:

    bool another = false;
    //bool alwaysStop = false;

    QLocalSocket *socket;
    QDataStream in;


    QPoint* position;

    int i3 = 0;
    bool enabled = true;

    bool second = false;


    QLocalServer* server;
    //bool clicked = false;

};
#endif // WIDGET_H
