#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <QDebug>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QProcess>
#include <vector>

using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    QString getLocalIp();

private:
    Ui::Widget *ui;
    int serverfd;   //套接字描述符
    struct sockaddr_in mcast_addr;
    QString text;
    QString LocalIp;
//    int reVal;
};

#endif // WIDGET_H
