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

//static const QString SOUNDPATH = "../2048New/sounds/"; //����ʱ·��  //ָ�������ļ�·����������������ļ�
static const QString SOUNDPATH = "./sounds/"; //����ʱ·���� ����ʱ�������ļ��п���������Ŀ¼��

myView::myView(QWidget *parent)
    :QGraphicsView(parent)
{
    initView();
}

void myView::initView()
{
    setMouseTracking(true);
    //ʹ�ÿ������Ⱦ
    setRenderHint(QPainter::Antialiasing);
    //���û��汳�����������Լӿ���Ⱦ�ٶ�
    setCacheMode(CacheBackground);
    setWindowIcon(QIcon(":/images/2048logo.png"));
    setWindowTitle(tr("2048"));
    setMinimumSize(342, 412);
    setMaximumSize(342, 412);
    //���ó���
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, 340, 410);
    scene->setBackgroundBrush(QPixmap(":/images/background.png"));
    setScene(scene);

    //���÷�����Ϳ��ƶ�����
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

    //��¼��ʾ�ı�
    recordNo1 = new QGraphicsTextItem(recordBk, scene);
    recordNo1->setFont(QFont("Times", 13, QFont::Bold));
    recordNo1->setPos(12, 18);

    //recordNo1->setHtml(tr("<font color=red>%1</font>").arg("22456"));
    //recordNo1->show();

    //��ɫ����
    QWidget *mask = new QWidget;
    mask->setAutoFillBackground(true);
    QPalette palette0 = mask->palette();
    palette0.setColor(QPalette::Window, QColor(0, 0, 0, 80));
    mask->setPalette(palette0);
    mask->resize(340, 410);
    maskWidget = scene->addWidget(mask);
    maskWidget->setPos(0, 0);

    //ѡ�����
    QWidget *musicOption = new QWidget;
    QPushButton *optCloseBtn = new QPushButton(tr("��    ��"), musicOption);
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

    //�������
    QWidget *help = new QWidget;
    QPushButton *helpCloseBtn = new QPushButton(tr("��    ��"), help);
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
    helpLabel->setText(tr("<h2><font color=white>����Qt��ͼ����ͼ��ܵ�<br />2048<br /><br />�������: ����<br />�������: ����<br />�������: ����<br />�������: ����</font></h2>"));
    helpLabel->setAlignment(Qt::AlignCenter);
    helpLabel->move(50, 100);

    //��Ϸ�����ı�
    gameOverText = new QGraphicsTextItem(0, scene);
    gameOverText->setHtml(tr("<font color=white>��Ϸ����!</font>"));
    gameOverText->setFont(QFont("Times", 20, QFont::Bold));


    //��Ϸ��ʹ�õİ�ť
    QPushButton *button2 = new QPushButton(tr("��    ��"));
    QPushButton *button3 = new QPushButton(tr("��    ��"));
    QPushButton *button4 = new QPushButton(tr("��    ��"));
    QPushButton *button5 = new QPushButton(tr("���¿�ʼ"));
    QPushButton *button7 = new QPushButton(tr("�� �� ��"));
    QPushButton *button8 = new QPushButton(tr("������Ϸ"));


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


    //������ZֵΪ1�� �������Դ���ZֵΪ0��ͼ��������
    maskWidget->setZValue(1);
    showMenuButton->setZValue(0);
    musicButton->setZValue(2);
    helpButton->setZValue(2);
    exitButton->setZValue(2);
    restartButton->setZValue(2);
    returnButton->setZValue(2);
    gameOverText->setZValue(2);

    initGame();


    //������Ǵ�����ý�����Ȼ��������ý��ͼ�����Ҵ������������Ʋ��������Ϊý�����������ý��Դ��
    //��������
    backgroundMusic = new Phonon::MediaObject(this);
    clearRowSound = new Phonon::MediaObject(this);
    Phonon::AudioOutput *audio1 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::AudioOutput *audio2 = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(backgroundMusic, audio1);
    Phonon::createPath(clearRowSound, audio2);
    //�����������Ʋ�����������ʾ��ѡ�������
    Phonon::VolumeSlider *volume1 = new Phonon::VolumeSlider(audio1, musicOption);
    Phonon::VolumeSlider *volume2 = new Phonon::VolumeSlider(audio2, musicOption);
    QLabel *volumeLabel1 = new QLabel(tr("��������:"), musicOption);
    QLabel *volumeLabel2 = new QLabel(tr("��ײ��Ч:"), musicOption);

    volume1->move(100, 100);
    volume2->move(100, 200);
    volumeLabel1->move(50, 105);
    volumeLabel2->move(50, 205);

    connect(backgroundMusic, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));

    //��Ϊ������Ϻ�������ͣ״̬���ٵ���play()���޷����в���
    //��Ҫ�ڲ�����Ϻ�ʹ�����ֹͣ״̬
    connect(clearRowSound, SIGNAL(finished()), clearRowSound, SLOT(stop()));
    backgroundMusic->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background.m4r"));
    clearRowSound->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "collide.mp3"));
    backgroundMusic->play();

}

void myView::addNewBox()
{
    //�������������Ī�����������֪Ϊ��,������д�˸�������ʵ�ֲ�����ײ����
     //clearRowSound->play();

    //����·���2��4���ڿհ�λ���������
    QList<QPoint> pointList; //��16�����������������
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
           if(list.size()>=1) //˵����λ���Ѿ��з�����
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
     QTimer::singleShot(400, this, SLOT(addNewBox())); //400ms������ӣ���֤֮ǰ�Ĳ����г�ֵ�ʱ��

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
    //���ٵ�ǰ�������ƶ������е�����С����
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
    //�������ֽ�Ҫ�������ʱ�������²���
    backgroundMusic->enqueue(backgroundMusic->currentSource());
}

void myView::mouseMoveEvent(QMouseEvent *e)
{
    qDebug() << "view:" << e->globalPos().x()- this->geometry().x()<< e->globalPos().y()- this->geometry().y();
}

void myView::keyPressEvent(QKeyEvent *e)
{
    //������ڽ�����Ϸ�������̰���ʱ���Ƿ������ý���;���ֳ���˵���ڲ˵�ģʽ
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

    //������Ϸǰ����¼�Ƿ���Ҫ�޸�
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




