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

    QPoint p = pos();
    position = &p;


    int size = 105; //150, 138, 111

    this->resize(size, size);
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setStyleSheet("background-color: transparent;");
    setAttribute(Qt::WA_TranslucentBackground);


    pet = new QLabel(this);
    pet->setPixmap(QPixmap(":/p/im/f0.png"));
    pet->setScaledContents(true);
    pet->setFixedSize(QSize(size, size));
    pet->move(0, 0);


    eyes = new QLabel(this);
    eyes->setPixmap(QPixmap(":/p/im/t0.png"));
    eyes->setScaledContents(true);
    eyes->setFixedSize(QSize(size, size));
    eyes->move(0, 0);

    setMouseTracking(true);

    //Socket
    socket = new QLocalSocket(this);
    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_5_10);

    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Widget::requestNewPosition);
    connect(socket, &QLocalSocket::readyRead, this, [=]() {
        QPoint posi;
        in >> posi;
        position = &posi;
        emit positionChanged();
    });

    timer->start(10);

    connect(this, &Widget::positionChanged, this, [=]() {
        double distance = std::sqrt(std::pow(pos().x() - position->x(), 2) + std::pow(pos().y() - position->y(), 2));

        if(enabled && distance <= 100) {
            alwaysStop = true;
            if(second) {
                move(position->x() - 3, position->y() - 42); //-4, -54, -5, -54, -4, -48
            }
            else move(position->x() + 1, position->y() - 32); //+2, -40, +1, -44, +3, -41, +2, -40, +2, -37, +1, -34
        }


    });


    //Server
    server = new QLocalServer(this);

    if (!server->listen("thepet")) {
        QMessageBox::critical(this, tr("Local Pet Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        close();
        return;
    }
    connect(server, &QLocalServer::newConnection, this, [=]() {
        QByteArray block;

        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);

        QPoint position = pos();

        out << position;

        QLocalSocket *clientConnection = server->nextPendingConnection();
        connect(clientConnection, &QLocalSocket::disconnected,
                clientConnection, &QLocalSocket::deleteLater);

        clientConnection->write(block);
    });

    //


    QList<QPixmap> ms = {QPixmap(":/p/im/t0.png"), QPixmap(":/p/im/t1.png"), QPixmap(":/p/im/t2.png"), QPixmap(":/p/im/t3.png"), QPixmap(":/p/im/t4.png"), QPixmap(":/p/im/t5.png"), QPixmap(":/p/im/t5.png"), QPixmap(":/p/im/t4.png"), QPixmap(":/p/im/t3.png"), QPixmap(":/p/im/t2.png"), QPixmap(":/p/im/t1.png"), QPixmap(":/p/im/t0.png")};

    t = new QTimer(this);

    connect(t, &QTimer::timeout, this, [=]() {
        if(notWink) {
            stopwink ++;
            eyes->setPixmap(QPixmap(":/p/im/t0.png"));
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
    t->start(25); //25


    t1 = new QTimer(this);

    position_pet = this->pos();
    //QTimer::singleShot(700, t1, SLOT(start(50)));

    QPixmap pixmap1(":/p/im/f0.png");
    QPixmap pixmap2(":/p/im/f1.png");
    QPixmap pixmap3(":/p/im/f0.png");
    QPixmap pixmap4(":/p/im/f3.png");

    QList<QPixmap> maps1 = {pixmap1, pixmap2, pixmap3, pixmap4};

    QList<QPixmap> maps2 = {pixmap1, pixmap4, pixmap3, pixmap2};

    connect(t1, &QTimer::timeout, this, [=]() {
        count++;

        if(changeImage) {
            pet->setPixmap(QPixmap(":/p/im/pa5.png"));
            return;
        }

        if(alwaysStop) {
            pet->setPixmap(QPixmap(":/p/im/pa.png"));
            return;
        }

        if(stop) {

            down();
            count2 ++;
            if(count2 <= randomNumber1) {
                return;
            }
            else {
                count2 = 0;
                stop = false;
                //move(pos().x(), pos().y() - 5);
                pet->setPixmap(QPixmap(":/p/im/f0.png"));

                randomNumber1 = QRandomGenerator::global()->bounded(60, 200);

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
            emit positionChanged();
            count3 ++;
            if(count3 >= randomNumber2) {
                right = false;
                stop = true;
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
            emit positionChanged();
            count4 ++;
            if(count4 >= randomNumber3) {
                stop = true;
                firstTime = true;
                right = true;
                count4 = 0;
                randomNumber3 = QRandomGenerator::global()->bounded(50, 80);

            }

        }

    });

    t1->start(50);




}

void Widget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {

        //move(pos().x(), pos().y() + 5);
        m_dragStartPosition = event->pos();
        low = true;
        firstTime = false;
        enabled = false;
        clicked = true;
        alwaysStop = true;
        //changeImage = false;


    }
    //lower();
}


void Widget::mouseMoveEvent(QMouseEvent *event) {

    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - m_dragStartPosition;
        move(pos() + delta);
        emit positionChanged();

    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event) {

    if(clicked) {
        changeImage = false;
        alwaysStop = false;
        enabled = true;
        clicked = false;
    }
    //lower();
}

void Widget::down() {

    if(firstTime) {
        move(pos().x(), pos().y() + 5);
        firstTime = false;
        low = true;
    }

    pet->setPixmap(QPixmap(":/p/im/pa.png"));
    //return;
}


void Widget::showMessage(QString text) {
    QMessageBox msg;
    //msg.setFont(msgFont);
    msg.setText(text);
    msg.setWindowTitle("提示");
    msg.exec();
}


void Widget::requestNewPosition()
{

    socket->abort();
    socket->connectToServer("anotherpet2");
    if(socket->state() == QLocalSocket::UnconnectedState) {
        socket->connectToServer("anotherpet");
        second = true;
    }
    else {
        second = false;
    }


}



void Widget::readPosition()
{

    QPoint posi;
    in >> posi;
    position = &posi;
    emit positionChanged();

}


Widget::~Widget()
{
    //server->close();
}


