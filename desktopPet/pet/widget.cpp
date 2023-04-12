#include "widget.h"
#include <QTimer>
#include <QPainter>
#include <QPalette>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>
#include <QMessageBox>
#include <QSharedMemory>
#include <QLocalServer>
#include <QLocalSocket>
#include <QtWidgets>
#include <QtNetwork>


Widget::Widget(QWidget *parent)
    : QWidget(parent)

{

    int size = 95; //140, 128, 120, 105，99

    this->resize(size, size);
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    //setStyleSheet("background-color: transparent;");
    //窗体透明
    setAttribute(Qt::WA_TranslucentBackground);


    pet = new QLabel(this);
    pet->setPixmap(QPixmap(":/p/images/c2noeyes.png"));
    pet->setScaledContents(true);
    pet->setFixedSize(QSize(size, size));
    pet->move(0, 0);


    eyes = new QLabel(this);
    eyes->setPixmap(QPixmap(":/p/images/e_quan.png"));
    eyes->setScaledContents(true);
    eyes->setFixedSize(QSize(size, size));
    eyes->move(0, 0);

    setMouseTracking(true);


    QPixmap map1 = QPixmap(":/p/images/e_quan.png");
    QPixmap map2 = QPixmap(":/p/images/e_banquan.png");
    QPixmap map3 = QPixmap(":/p/images/e_ban.png");
    QPixmap map4 = QPixmap(":/p/images/e_banbi.png");
    QPixmap map5 = QPixmap(":/p/images/e_bi.png");
    QPixmap map6 = QPixmap(":/p/images/e_bi.png");
    QList<QPixmap> ms = {map1, map2, map3, map4, map5, map6, map5, map4, map3, map2, map1};

    t = new QTimer(this);

    connect(t, &QTimer::timeout, this, [=]() {
        if(notWink) {
            stopwink ++;
            eyes->setPixmap(QPixmap(":/p/images/e_quan.png"));
            if(stopwink <= random) {
                return;
            }
            else {
                //qDebug() << random;
                stopwink = 0;
                notWink = false;
                random = QRandomGenerator::global()->bounded(55, 100);

            }

        }
        wink ++;
        if(wink <= ms.size()) {
            i = (i + 1) % ms.size();
            eyes->setPixmap(ms.at(i));
        }
        else {
            wink = 0;
            notWink = true;
        }

    });
    t->start(30); //25

    t1 = new QTimer(this);

    position_pet = this->pos();
    //QTimer::singleShot(700, t1, SLOT(start(50)));

    QPixmap pixmap1(":/p/images/c2noeyes.png");
    QPixmap pixmap2(":/p/images/c22.png");
    QPixmap pixmap3(":/p/images/c23.png");
    QPixmap pixmap4(":/p/images/c4.png");

    QList<QPixmap> maps1 = {pixmap2, pixmap3, pixmap4, pixmap1};

    QList<QPixmap> maps2 = {pixmap4, pixmap3, pixmap2, pixmap1};

    connect(t1, &QTimer::timeout, this, [=]() {

        count++;

        //1, 2
        //qDebug() << randomNumber4;

        if(alwaysStop) {
            if(low) {
               pet->setPixmap(QPixmap(":/p/images/pa3.png"));
            }
            else {
               pet->setPixmap(QPixmap(":/p/images/c2noeyes.png")); //pa3
            }

            return;
        }

        if(stop) {

            if(!set) {
                randomNumber4 = QRandomGenerator::global()->bounded(1, 100);
                //qDebug() << randomNumber4;
                set = true;
            }

            if(randomNumber4 % 2 == 0) {
                down();
            }
            else {
                pet->setPixmap(QPixmap(":/p/images/c2noeyes.png"));
            }

            count2 ++;

            if(count2 <= randomNumber) {
                //qDebug() << randomNumber;

                return;
            }
            else {
                //qDebug() << randomNumber;
                count2 = 0;
                stop = false;
                //move(pos().x(), pos().y() - 5);
                pet->setPixmap(QPixmap(":/p/images/c2noeyes.png"));

                randomNumber = QRandomGenerator::global()->bounded(60, 200);

            }

        }


        if(low) {
           move(pos().x(), pos().y() - 5);
           low = false;
        }


        if(right) {
            if(count % 3 == 0) {
                i1 = (i1 + 1) % maps1.size();
                pet->setPixmap(maps1.at(i1));

            }
            move(pos().x() + 1, pos().y());
            count3 ++;
            if(count3 >= randomNumber2) {
                right = false;
                stop = true;
                set = false; //
                firstTime = true;
                count3 = 0;
                randomNumber2 = QRandomGenerator::global()->bounded(50, 80);


            }

        }

        else {
            if(count % 3 == 0) {
                i1 = (i1 + 1) % maps2.size();
                pet->setPixmap(maps2.at(i1));

            }
            move(pos().x() - 1, pos().y());
            count4 ++;
            if(count4 >= randomNumber3) {
                stop = true;
                set = false;
                firstTime = true;
                right = true;
                count4 = 0;

                randomNumber3 = QRandomGenerator::global()->bounded(50, 80);
            }
        }        
    });

    t1->start(50);

    //Server
    server = new QLocalServer(this);

    if (!server->listen("anotherpet")) {
        QMessageBox::critical(this, tr("Local Pet Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        close();
        return;
    }
    connect(server, &QLocalServer::newConnection, this, &Widget::sendPosition);

    server2 = new QLocalServer(this);

    if (!server2->listen("anotherpet3")) {
        QMessageBox::critical(this, tr("Local Pet Server2"),
                              tr("Unable to start the server2: %1.")
                              .arg(server2->errorString()));
        close();
        return;
    }
    connect(server2, &QLocalServer::newConnection, this, &Widget::sendPosition2);

}

void Widget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        clicked = true;        
        m_dragStartPosition = event->pos();
        alwaysStop = true;        
        firstTime = false;

    }


}


void Widget::mouseMoveEvent(QMouseEvent *event) {

    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - m_dragStartPosition;
        move(pos() + delta);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event) {

    if(clicked) {
        alwaysStop = false;
        clicked = false;
    }

}

void Widget::down() {

    if(firstTime) {
        move(pos().x(), pos().y() + 5);
        firstTime = false;
        low = true;
    }

    pet->setPixmap(QPixmap(":/p/images/pa3.png"));

}


//Server
void Widget::sendPosition() {
    QByteArray block;

    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    QPoint position = pos();  

    out << position << rotated;

    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, &QLocalSocket::disconnected,
            clientConnection, &QLocalSocket::deleteLater);

    clientConnection->write(block);

}

void Widget::sendPosition2() {
    QByteArray block2;

    QDataStream out2(&block2, QIODevice::WriteOnly);
    out2.setVersion(QDataStream::Qt_5_10);

    QPoint position2 = pos();

    out2 << position2;

    QLocalSocket *clientConnection2 = server2->nextPendingConnection();
    connect(clientConnection2, &QLocalSocket::disconnected,
            clientConnection2, &QLocalSocket::deleteLater);

    clientConnection2->write(block2);

}


Widget::~Widget()
{
    server->close();
    server2->close();
}


