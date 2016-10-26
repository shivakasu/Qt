#include "myView.h"
#include "myBox.h"
#include <QApplication>
#include <QTextCodec>
#include <QGraphicsScene>
#include <QTime>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QApplication app(argc, argv);
    //����������ĳ�ʼֵ
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    myView *view = new myView;
    view->show();

    return app.exec();
}
