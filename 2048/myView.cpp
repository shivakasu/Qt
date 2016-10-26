#include "myView.h"
#include <QDebug>
#include <QIcon>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QTimer>
#include <QFileInfo>

//static const QString SOUNDPATH = "../2048New/sounds/"; //编译时路径  //指定声音文件路径，方便更改声音文件
static const QString SOUNDPATH = "./sounds/"; //发布时路径， 发布时将声音文件夹拷贝到发布目录下

myView::myView(QWidget *parent)
    :QGraphicsView(parent)
{
    initView();
}

void myView::initView()
{
    setMouseTracking(true);
    //使用抗锯齿渲染
    setRenderHint(QPainter::Antialiasing);
    //设置缓存背景，这样可以加快渲染速度
    setCacheMode(CacheBackground);
    setWindowIcon(QIcon(":/images/2048logo.png"));
    setWindowTitle(tr("2048"));
    setMinimumSize(342, 412);
    setMaximumSize(342, 412);
    //设置场景
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, 340, 410);
    scene->setBackgroundBrush(QPixmap(":/images/background.png"));
    setScene(scene);

    //设置方块组和可移动区域
    topLine = scene->addLine(10, 80, 332, 80);
    bottomLine = scene->addLine(10, 400, 332, 400);
    leftLine = scene->addLine(10, 80, 10, 402);
    rightLine = scene->addLine(330, 80, 330, 402);

    //2048logo
    logo = new QGraphicsPixmapItem(0, scene);
    logo->setPixmap(QPixmap(":/images/2048logo.png"));
    logo->setPos(10, 0);

    scoreBk = new QGraphicsPixmapItem(0, scene);
    scoreBk->setPixmap(QPixmap(":/images/scoreBackground.png"));
    scoreBk->setPos(100, 0);

    recordBk = new QGraphicsPixmapItem(0, scene);
    recordBk->setPixmap(QPixmap(":/images/recordBackground.png"));
    recordBk->setPos(180, 0);


    gameScoreText = new QGraphicsTextItem(scoreBk, scene);
    gameScoreText->setFont(QFont("Times", 13, QFont::Bold));
    gameScoreText->setPos(16, 18);

    //纪录显示文本
    recordNo1 = new QGraphicsTextItem(recordBk, scene);
    recordNo1->setFont(QFont("Times", 13, QFont::Bold));
    recordNo1->setPos(12, 18);

    //recordNo1->setHtml(tr("<font color=red>%1</font>").arg("22456"));
    //recordNo1->show();

    //黑色遮罩
    QWidget *mask = new QWidget;
    mask->setAutoFillBackground(true);
    QPalette palette0 = mask->palette();
    palette0.setColor(QPalette::Window, QColor(0, 0, 0, 80));
    mask->setPalette(palette0);
    mask->resize(340, 410);
    maskWidget = scene->addWidget(mask);
    maskWidget->setPos(0, 0);

    //选项面板
    QWidget *musicOption = new QWidget;
    QPushButton *optCloseBtn = new QPushButton(tr("关    闭"), musicOption);
    QPalette palette;
    palette.setColor(QPalette::ButtonText, Qt::black);
    optCloseBtn->setPalette(palette);
    optCloseBtn->move(120, 300);
    connect(optCloseBtn, SIGNAL(clicked()), musicOption, SLOT(hide()));
    musicOption->setAutoFillBackground(true);
    musicOption->setPalette(QPalette(QColor(0, 0, 0, 120)));
    musicOption->resize(340, 410);
    QGraphicsWidget *musicOptionWidget = scene->addWidget(musicOption);
    musicOptionWidget->setPos(0, 0);
    musicOptionWidget->setZValue(3);
    musicOptionWidget->hide();

    //帮助面板
    QWidget *help = new QWidget;
    QPushButton *helpCloseBtn = new QPushButton(tr("关    闭"), help);
    helpCloseBtn->setPalette(palette);
    helpCloseBtn->move(120, 300);
    connect(helpCloseBtn, SIGNAL(clicked()), help, SLOT(hide()));
    help->setAutoFillBackground(true);
    help->setPalette(QPalette(QColor(0, 0, 0, 120)));
    help->resize(340, 410);
    QGraphicsWidget *helpWidget = scene->addWidget(help);
    helpWidget->setPos(0, 0);
    helpWidget->setZValue(3);
    helpWidget->hide();
    QLabel *helpLabel = new QLabel(help);
    helpLabel->setText(tr("<h2><font color=white>基于Qt的图形视图框架的<br />2048<br /><br />方向键←: 左移<br />方向键→: 右移<br />方向键↓: 下移<br />方向键↑: 上移</font></h2>"));
    helpLabel->setAlignment(Qt::AlignCenter);
    helpLabel->move(50, 100);

    //游戏结束文本
    gameOverText = new QGraphicsTextItem(0, scene);
    gameOverText->setHtml(tr("<font color=white>游戏结束!</font>"));
    gameOverText->setFont(QFont("Times", 20, QFont::Bold));


    //游戏中使用的按钮
    QPushButton *button2 = new QPushButton(tr("声    音"));
    QPushButton *button3 = new QPushButton(tr("帮    助"));
    QPushButton *button4 = new QPushButton(tr("退    出"));
    QPushButton *button5 = new QPushButton(tr("重新开始"));
    QPushButton *button7 = new QPushButton(tr("主 菜 单"));
    QPushButton *button8 = new QPushButton(tr("返回游戏"));


    connect(button2, SIGNAL(clicked()), musicOption, SLOT(show()));
    connect(button3, SIGNAL(clicked()), help, SLOT(show()));
    connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(button5, SIGNAL(clicked()), this, SLOT(restartGame()));
    connect(button7, SIGNAL(clicked()), this, SLOT(showMenu()));
    connect(button8, SIGNAL(clicked()), this, SLOT(returnGame()));

    musicButton = scene->addWidget(button2);
    helpButton = scene->addWidget(button3);
    exitButton = scene->addWidget(button4);
    restartButton = scene->addWidget(button5);
    showMenuButton = scene->addWidget(button7);
    returnButton = scene->addWidget(button8);

    returnButton->setPos(120, 120);
    restartButton->setPos(120, 150);
    musicButton->setPos(120, 180);
    helpButton->setPos(120, 210);
    exitButton->setPos(120, 240);
    showMenuButton->setPos(99, 47);
    gameOverText->setPos(100, 100);


    //设置其Z值为1， 这样可以处于Z值为0的图形项上面
    maskWidget->setZValue(1);
    showMenuButton->setZValue(0);
    musicButton->setZValue(2);
    helpButton->setZValue(2);
    exitButton->setZValue(2);
    restartButton->setZValue(2);
    returnButton->setZValue(2);
    gameOverText->setZValue(2);

    initGame();


    //这里就是创建了媒体对象，然后生成了媒体图，并且创建了音量控制部件，最后为媒体对象设置了媒体源。
    //设置声音
    backgroundMusic = new Phonon::MediaObject(this);
    clearRowSound = new Phonon::MediaObject(this);
    Phonon::AudioOutput *audio1 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::AudioOutput *audio2 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(backgroundMusic, audio1);
    Phonon::createPath(clearRowSound, audio2);
    //设置音量控制部件，它们显示在选项面板上
    Phonon::VolumeSlider *volume1 = new Phonon::VolumeSlider(audio1, musicOption);
    Phonon::VolumeSlider *volume2 = new Phonon::VolumeSlider(audio2, musicOption);
    QLabel *volumeLabel1 = new QLabel(tr("背景音乐:"), musicOption);
    QLabel *volumeLabel2 = new QLabel(tr("碰撞音效:"), musicOption);

    volume1->move(100, 100);
    volume2->move(100, 200);
    volumeLabel1->move(50, 105);
    volumeLabel2->move(50, 205);

    connect(backgroundMusic, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));

    //因为播放完毕后会进入暂停状态，再调用play()将无法进行播放
    //需要在播放完毕后使其进入停止状态
    connect(clearRowSound, SIGNAL(finished()), clearRowSound, SLOT(stop()));
    backgroundMusic->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background.m4r"));
    clearRowSound->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "collide.mp3"));
    backgroundMusic->play();

}

void myView::addNewBox()
{
    //这里添加这句代码莫名其妙出错，不知为何,故又新写了个槽另外实现播放碰撞声音
     //clearRowSound->play();

    //添加新方块2或4，在空白位置随机出现
    QList<QPoint> pointList; //将16个点坐标放入数组中
    for(int i=80; i<=320; i+=80) //column
        for(int j=10; j<=250; j+=80) //row
          {
              pointList << QPoint(j, i);
          }

       for(;;)
       {
           int i = qrand()%16;
           int x = pointList.at(i).x();
           int y = pointList.at(i).y();
           QList<QGraphicsItem *> list = scene()->items(x-2, y-2, 84, 84, Qt::ContainsItemShape);
          //qDebug() << list;
           if(list.size()>=1) //说明该位置已经有方块了
               continue;
           boxGroup->createBox(pointList.at(i), randomNumber);

           break;
       }


}

void myView::playColliMusic()
{
     clearRowSound->play();
     QTimer::singleShot(100, this, SLOT(addNewBox()));
}


void myView::initGame()
{

    maskWidget->hide();
    returnButton->hide();
    restartButton->hide();
    musicButton->hide();
    helpButton->hide();
    exitButton->hide();
    gameOverText->hide();


    gameScoreText->setHtml(tr("<font color=red>0</font>"));
    boxGroup = new BoxGroup;
    scene()->addItem(boxGroup);
    //addNewBox();
     QTimer::singleShot(400, this, SLOT(addNewBox())); //400ms后再添加，保证之前的操作有充分的时间

     connect(boxGroup, SIGNAL(needNewBox()), this, SLOT(playColliMusic()));
     connect(boxGroup, SIGNAL(scoreChanged()), this, SLOT(updateScore()));
     connect(boxGroup, SIGNAL(gameFinished()), this, SLOT(gameFinish()));

    boxGroup->setFocus();

/**/
    QFile scoreList("score.txt");
    if(scoreList.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream out(&scoreList);
        QString temp;

        if(!out.atEnd())
        {
            out >> temp;
        }

        scoreList.close();
        recordNo1->setHtml(tr("<font color=red>%1</font>").arg(temp));
        recordNo1->show();
    }
    else
        recordNo1->hide();

}


void myView::showMenu()
{
    maskWidget->show();
    returnButton->show();
    restartButton->show();
    musicButton->show();
    helpButton->show();
    exitButton->show();

    backgroundMusic->pause();

}

void myView::restartGame()
{
    //销毁当前方块组移动区域中的所有小方块
    boxGroup->clearBoxGroup(true);
    boxGroup->deleteLater();

    initGame();

    backgroundMusic->stop();
    backgroundMusic->play();
}

void myView::returnGame()
{
    returnButton->hide();
    restartButton->hide();
    musicButton->hide();
    helpButton->hide();
    exitButton->hide();
    maskWidget->hide();
    boxGroup->setFocus();

    backgroundMusic->play();
}

void myView::aboutToFinish()
{
    //背景音乐将要播放完毕时继续重新播放
    backgroundMusic->enqueue(backgroundMusic->currentSource());
}

void myView::mouseMoveEvent(QMouseEvent *e)
{
    qDebug() << "view:" << e->globalPos().x()- this->geometry().x()<< e->globalPos().y()- this->geometry().y();
}

void myView::keyPressEvent(QKeyEvent *e)
{
    //如果正在进行游戏，当键盘按下时总是方块组获得焦点;遮罩出现说明在菜单模式
    if(maskWidget->isVisible())
        boxGroup->clearFocus();
    else
        boxGroup->setFocus();
    QGraphicsView::keyPressEvent(e);
}


void myView::updateScore()
{
    int score = boxGroup->getScoreValue();
    gameScoreText->setHtml(tr("<font color=red>%1</font>").arg(score));

}

void myView::gameFinish()
{
    maskWidget->show();
    restartButton->show();
    exitButton->show();
    gameOverText->show();
    gameOverText->setPos(100, 80);

    //结束游戏前检查纪录是否需要修改
    QString endScore = gameScoreText->toPlainText();
    qDebug() << "endScore: " << endScore;
    QFile scoreList("score.txt");
    if(scoreList.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream out(&scoreList);
        QString temp;

         out >> temp;
         if(temp.toInt() < endScore.toInt())
         {
            scoreList.close();

            scoreList.open(QIODevice::WriteOnly|QIODevice::Text);
            QTextStream in(&scoreList);
            in << endScore;
            scoreList.close();
        }

    }
    else
    {
        scoreList.close();
        scoreList.open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream in(&scoreList);
        in << endScore ;
        scoreList.close();

    }
/**/
}




