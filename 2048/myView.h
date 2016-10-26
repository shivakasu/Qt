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

    void restartGame(); //重新开始游戏
    void returnGame(); //暂停后再返回游戏

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

    //添加文本图形项对象，用来显示分数
    QGraphicsTextItem *gameScoreText;

    //遮罩面板
    QGraphicsWidget *maskWidget;
    //各种按钮
    QGraphicsWidget *restartButton;
    QGraphicsWidget *musicButton;
    QGraphicsWidget *returnButton;
    QGraphicsWidget *helpButton;
    QGraphicsWidget *exitButton;
    QGraphicsWidget *showMenuButton;
    //各种文本
    QGraphicsTextItem *gameOverText;

    QGraphicsTextItem *gameScoreLabel;
    QGraphicsTextItem *recordNo1;

    QGraphicsPixmapItem *logo; //2048游戏logo
    QGraphicsPixmapItem *scoreBk;
    QGraphicsPixmapItem *recordBk;

    Phonon::MediaObject *backgroundMusic; //用来控制背景音乐
    Phonon::MediaObject *clearRowSound; //用来控制销毁动画的声音

     void initView();
     void initGame();

};
#endif // MYVIEW_H
