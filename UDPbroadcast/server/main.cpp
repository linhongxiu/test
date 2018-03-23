#include "widget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForTr(codec );
    QTextCodec::setCodecForLocale(codec );
    QTextCodec::setCodecForCStrings(codec );   //设置QString默认使用utf-8编码

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
