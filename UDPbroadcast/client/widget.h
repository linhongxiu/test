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
#include <vector>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void recvData();

private:
    Ui::Widget *ui;
    int clientfd;
    struct sockaddr_in local_addr;
    struct ip_mreq mreq;
    int err;
};

#endif // WIDGET_H
