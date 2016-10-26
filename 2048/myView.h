#ifndef MYVIEW_H
#define MYVIEW_H
#include <QGraphicsView>
#include <QMouseEvent>
#include <QKeyEvent>
#include "myBox.h"
//#include <phonon>
#include <QLabel>

class myView:public QGraphicsView{
    Q_OBJECT
public:
    explicit myView(QWidget *parent=0);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

public slots:
    void updateScore();
    void gameFinish();
    void showMenu();

    void restartGame(); //���¿�ʼ��Ϸ
    void returnGame(); //��ͣ���ٷ�����Ϸ

     void aboutToFinish();
     void addNewBox();
     void playColliMusic();

private:
    BoxGroup *boxGroup;
    QGraphicsLineItem *topLine;
    QGraphicsLineItem *bottomLine;
    QGraphicsLineItem *leftLine;
    QGraphicsLineItem *rightLine;
    QGraphicsRectItem *frameRect;

    //����ı�ͼ�������������ʾ����
    QGraphicsTextItem *gameScoreText;

    //�������
    QGraphicsWidget *maskWidget;
    //���ְ�ť
    QGraphicsWidget *restartButton;
    QGraphicsWidget *musicButton;
    QGraphicsWidget *returnButton;
    QGraphicsWidget *helpButton;
    QGraphicsWidget *exitButton;
    QGraphicsWidget *showMenuButton;
    //�����ı�
    QGraphicsTextItem *gameOverText;

    QGraphicsTextItem *gameScoreLabel;
    QGraphicsTextItem *recordNo1;

    QGraphicsPixmapItem *logo; //2048��Ϸlogo
    QGraphicsPixmapItem *scoreBk;
    QGraphicsPixmapItem *recordBk;

    Phonon::MediaObject *backgroundMusic; //�������Ʊ�������
    Phonon::MediaObject *clearRowSound; //�����������ٶ���������

     void initView();
     void initGame();

};
#endif // MYVIEW_H
