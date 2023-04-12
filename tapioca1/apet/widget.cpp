#include "widget.h"
#include <QTimer>
#include <QPainter>
#include <QPalette>
#include <QSharedMemory>
#include <QMessageBox>
#include <QLocalSocket>
#include <iostream>
#include <QPushButton>
#include <QtGlobal>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

Widget::Widget(QWidget *parent)
    : QWidget(parent)

{
    QPoint p = pos();
    position = &p;

    int size = 106; //153, 143, 131, 112

    this->resize(size, size);
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    setStyleSheet("background-color: transparent;");
    setAttribute(Qt::WA_TranslucentBackground);


    pet = new QLabel(this);
    pet->setPixmap(QPixmap(":/p/ima/n1.png"));
    pet->setScaledContents(true);
    pet->setFixedSize(QSize(size, size));
    pet->move(0, 0);


    eyes = new QLabel(this);
    eyes->setPixmap(QPixmap(":/p/ima/t0.png"));
    eyes->setScaledContents(true);
    eyes->setFixedSize(QSize(size, size));
    eyes->move(0, 0);

    //Socket
    socket = new QLocalSocket(this);
    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_5_10);

    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout,
            this, &Widget::requestNewPosition);

    connect(socket, &QLocalSocket::readyRead, this, &Widget::readPosition);

    timer->start(10);

    connect(this, &Widget::positionChanged, this, [=]() {        

        double distance = std::sqrt(std::pow(pos().x() - position->x(), 2) + std::pow(pos().y() - position->y(), 2));
        if(enabled && distance <= 100) {
            alwaysStop = true;
            move(position->x() - 3, position->y() - 41); //-5, 56, -6, -54, -5, -48, -3, -43
        }        

    });


    //Server
    server = new QLocalServer(this);

    if (!server->listen("anotherpet2")) {
        QMessageBox::critical(this, tr("Local Pet Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        close();
        return;
    }
    connect(server, &QLocalServer::newConnection, this, &Widget::sendPosition);


    QList<QPixmap> ms = {};

    for(int k = 0; k < 50; k ++) {
        ms.push_back(QPixmap(":/p/ima/t0.png"));
    }
    ms.push_back(QPixmap(":/p/ima/t1.png"));
    ms.push_back(QPixmap(":/p/ima/t2.png"));
    ms.push_back(QPixmap(":/p/ima/t3.png"));
    ms.push_back(QPixmap(":/p/ima/t4.png"));
    ms.push_back(QPixmap(":/p/ima/t4.png"));
    ms.push_back(QPixmap(":/p/ima/t3.png"));
    ms.push_back(QPixmap(":/p/ima/t2.png"));
    ms.push_back(QPixmap(":/p/ima/t1.png"));

    for(int k = 0; k < 60; k ++) {
        ms.push_back(QPixmap(":/p/ima/t0.png"));
    }
    ms.push_back(QPixmap(":/p/ima/t1.png"));
    ms.push_back(QPixmap(":/p/ima/t2.png"));
    ms.push_back(QPixmap(":/p/ima/t3.png"));
    ms.push_back(QPixmap(":/p/ima/t4.png"));
    ms.push_back(QPixmap(":/p/ima/t4.png"));
    ms.push_back(QPixmap(":/p/ima/t3.png"));
    ms.push_back(QPixmap(":/p/ima/t2.png"));
    ms.push_back(QPixmap(":/p/ima/t1.png"));

    for(int k = 0; k < 80; k ++) {
        ms.push_back(QPixmap(":/p/ima/t0.png"));
    }
    ms.push_back(QPixmap(":/p/ima/t1.png"));
    ms.push_back(QPixmap(":/p/ima/t2.png"));
    ms.push_back(QPixmap(":/p/ima/t3.png"));
    ms.push_back(QPixmap(":/p/ima/t4.png"));
    ms.push_back(QPixmap(":/p/ima/t4.png"));
    ms.push_back(QPixmap(":/p/ima/t3.png"));
    ms.push_back(QPixmap(":/p/ima/t2.png"));
    ms.push_back(QPixmap(":/p/ima/t1.png"));


    t = new QTimer(this);

    connect(t, &QTimer::timeout, this, [=]() {

        i = (i + 1) % ms.size();
        eyes->setPixmap(ms.at(i));

    });
    t->start(25);

    t1 = new QTimer(this);

    //position_pet = pet->pos();
    //QTimer::singleShot(700, t1, SLOT(start(50)));

    QPixmap pixmap1(":/p/ima/n1.png");
    QPixmap pixmap2(":/p/ima/n2.png");
    QPixmap pixmap3(":/p/ima/n1.png");
    QPixmap pixmap4(":/p/ima/n3.png");

    QList<QPixmap> maps = {pixmap1, pixmap2, pixmap3, pixmap4};

    QList<QPixmap> maps2 = {};
    for(int j = 0; j < 30; j ++) {
        maps2.push_back(QPixmap(":/p/ima/r0.png"));
    }
    maps2.push_back(QPixmap(":/p/ima/d1.png"));
    //maps2.push_back(QPixmap(":/p/ima/r0.png"));
    //maps2.push_back(QPixmap(":/p/ima/d2.png"));

    for(int j = 0; j < 40; j ++) {
        maps2.push_back(QPixmap(":/p/ima/r0.png"));
    }
    maps2.push_back(QPixmap(":/p/ima/d2.png"));
    //maps2.push_back(QPixmap(":/p/ima/r0.png"));
    //maps2.push_back(QPixmap(":/p/ima/d1.png"));

    for(int j = 0; j < 50; j ++) {
        maps2.push_back(QPixmap(":/p/ima/r0.png"));
    }
    maps2.push_back(QPixmap(":/p/ima/d1.png"));
    //maps2.push_back(QPixmap(":/p/ima/r0.png"));
    //maps2.push_back(QPixmap(":/p/ima/d2.png"));

    connect(t1, &QTimer::timeout, this, [=]() {

        if(alwaysStop) {
            pet->setPixmap(QPixmap(":/p/ima/r0.png"));

            i3 = (i3 + 1) % maps2.size();
            pet->setPixmap(maps2.at(i3));
            return;
        }


        count++;

        if(count % 3 == 0) {
            i1 = (i1 + 1) % maps.size();
            pet->setPixmap(maps.at(i1));

        }

        if(stop) {
            count2 ++;

            if(firstTime) {
                move(pos().x(), pos().y() + 5);
                firstTime = false;
            }

            pet->setPixmap(QPixmap(":/p/ima/r0.png"));

            i2 = (i2 + 1) % maps2.size();
            pet->setPixmap(maps2.at(i2));

            if(count2 <= 80) {
                return;
            }
            else {
                count2 = 0;
                stop = false;
                move(pos().x(), pos().y() - 5);
                pet->setPixmap(QPixmap(":/p/ima/n1.png"));
            }

        }

        if(right) {
            move(pos().x() + 1, pos().y());
            emit positionChanged();

            count3 ++;
            if(count3 >= 70) {
                right = false;
                stop = true;
                firstTime = true;
                count3 = 0;
            }

        }

        else {
            move(pos().x() - 1, pos().y());
            emit positionChanged();

            count4 ++;
            if(count4 >= 70) {
                right = true;
                stop = true;
                firstTime = true;
                count4 = 0;
            }

        }

    });

    t1->start(50);

}

void Widget::mousePressEvent(QMouseEvent *event) {
    //if(alwaysStop) return;

    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
        stop = true;
        firstTime = false;
        enabled = false;
        clicked = true;
        alwaysStop = true;
    }
    //raise();

}


void Widget::mouseMoveEvent(QMouseEvent *event) {
    //if(alwaysStop) return;
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - m_dragStartPosition;
        move(pos() + delta);
        emit positionChanged();
    }

}

void Widget::mouseReleaseEvent(QMouseEvent *event) {

    if(clicked) {
        alwaysStop = false;
        enabled = true;
        clicked = false;
    }
    //lower();
}

void Widget::showMessage(QString text) {
    QMessageBox msg;
    //msg.setFont(msgFont);
    msg.setText(text);
    msg.setWindowTitle("提示");
    msg.exec();
}

//Socket
void Widget::requestNewPosition()
{

    socket->abort();
    socket->connectToServer("anotherpet");

}

void Widget::readPosition()
{
    QByteArray data = socket->readAll();
    QDataStream stream(data);

    QPoint posi;
    bool r;
    stream >> posi >> r;
    position = &posi;
    rotated = r;

    emit positionChanged();

}

void Widget::showPosition() {
    showMessage(QString::number(position->x(), position->y()));
}

//Server
void Widget::sendPosition() {
    QByteArray block;

    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    QPoint position = pos();

    out << position;

    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, &QLocalSocket::disconnected,
            clientConnection, &QLocalSocket::deleteLater);

    clientConnection->write(block);

}

Widget::~Widget()
{

}

