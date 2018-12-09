#include <QtGui/QApplication>
//#include <QTextCodec>
#include "led.h"
#include "leddialog.h"

#include "leddialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LedDialog w;
    // QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    w.show();
    return a.exec();
}
