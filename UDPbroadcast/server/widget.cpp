#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QByteArray>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    int n = 1;
    text = ui->label->text();
    LocalIp = getLocalIp();
    serverfd = socket(AF_INET, SOCK_DGRAM, 0);         /*建立套接字*/
    if(serverfd == -1)
    {
        perror("socket()");
    }

    setsockopt(serverfd, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&n, sizeof(n));

    memset(&mcast_addr, 0, sizeof(mcast_addr));     /*初始化IP多播地址为0*/
    mcast_addr.sin_family = AF_INET;                /*设置协议族类行为AF*/
    mcast_addr.sin_addr.s_addr = inet_addr("224.0.0.88");   /*设置多播IP地址,一个局部连接多播地址，路由器不进行转发*/
    mcast_addr.sin_port = htons(8000);        /*设置多播端口*/

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    /*向多播地址发送数据*/
    QString str = LocalIp + "+" + text;
    char *data = const_cast<char*>(str.toStdString().data());
    int len = str.length();

    std::vector<char> buf;   //totalsize个元素的buf
    buf.insert(buf.end(), data, data + len);

    int datasize = htonl(buf.size());
    buf.insert(buf.begin(), (char*)&datasize, ((char*)&datasize) + 4);

    int totalsize = buf.size();
    int sendsize = 0;
    while(1)
    {
        int n = sendto(serverfd, buf.data() + sendsize, totalsize - sendsize, 0, (struct sockaddr*)&mcast_addr, sizeof(mcast_addr)) ;
        if( n <= 0)
        {
            perror("sendto()");
            break;
        }
        sendsize += n;
        if(sendsize == totalsize)
        {
            qDebug() << "send data success";
            break;
        }
    }
}

QString Widget::getLocalIp()
{
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    foreach(const QHostAddress& addr, AddressList){
       if(addr.protocol() == QAbstractSocket::IPv4Protocol &&
               addr != QHostAddress::Null &&
               addr != QHostAddress::LocalHost){
           return addr.toString();
           qDebug() << addr.toString();
       }
    }
    return QString("");
}
