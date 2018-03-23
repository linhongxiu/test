#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget), err(-1)
{
    ui->setupUi(this);
    clientfd = socket(AF_INET, SOCK_DGRAM, 0);     /*建立套接字*/
    if (clientfd == -1)
    {
        perror("socket()");
    }
    /*初始化地址*/
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(8000);

    /*绑定socket*/
    err = bind(clientfd,(struct sockaddr*)&local_addr, sizeof(local_addr)) ;
    if(err < 0)
    {
        perror("bind()");
    }

    int loop = 1;   //设置回环许可
    err = setsockopt(clientfd,IPPROTO_IP, IP_MULTICAST_LOOP,&loop, sizeof(loop));   /*设置回环许可*/
    if(err < 0)
    {
        perror("setsockopt():IP_MULTICAST_LOOP");
    }

    /*加入多播组*/
    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.88"); /*多播地址*/
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); /*网络接口为默认*/
                                                        /*将本机加入多播组*/
    err = setsockopt(clientfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));  //将本机加入组播组
    if (err < 0)
    {
        perror("setsockopt():IP_ADD_MEMBERSHIP");
    }

    recvData();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::recvData()
{
    qDebug() << "start recv data11111111";
    while(1)
    {
        socklen_t addr_len = sizeof(local_addr);
        char headbuff[4] = {0};
        int headsize = 0;
        while(headsize < 4)
        {
            memset(headbuff, 0, 4);                 /*清空接收缓冲区*/
            qDebug() << "start recv data22222222222";                                       /*接收数据*/
            int n = recvfrom(clientfd, headbuff + headsize, 4 - headsize, 0,(struct sockaddr*)&local_addr, &addr_len);
            qDebug() << "start recv data33333333333333";
            if(n <= 0)
            {
                perror("recvfrom()");
                break;
            }
            headsize += n;
        }
        qDebug() << "start recv data4444444444";
        if(headsize != 4)
        {
            qDebug() << "recv headdata faild";
            break;
        }

        QByteArray head_data(headbuff, 4);
        int datalen = ntohl(*((int*)(head_data.data())));

        std::vector<char> data(datalen);
        int bytes = 0;
        while(bytes < datalen)
        {
            int n = ::recv(clientfd, data.data() + bytes, datalen - bytes, 0);
            if(n <= 0)
            {
                break;
            }
            bytes += n;
        }
        if(bytes != datalen)
        {
            qDebug() << "recv data failed";
            break;
        }
        for(std::vector<char>::iterator it = data.begin(); it != data.end(); it++)
        {
                qDebug() << *it <<"";
        }
    }

   /*退出多播组*/
   err = setsockopt(clientfd, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(mreq));

   ::close(clientfd);
}
