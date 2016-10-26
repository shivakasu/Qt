#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QMediaMetaData>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listView->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    now1=0;now2=0;total1=0;total2=0;volumn=0;playrate=1.0;loadinfo=false;

    tobgblue();

    player = new QMediaPlayer;
    musiclist = new QMediaPlaylist;
    musiclist->setPlaybackMode(QMediaPlaylist::Sequential);
    player->setPlaylist(musiclist);
    player->setVolume(30);
    ui->songProgress->setRange(0,0);

    disableplay();

    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(durationChanged(qint64)));
    connect(player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(Loadinfo(QMediaPlayer::MediaStatus)));
    connect(musiclist,SIGNAL(mediaInserted(int,int)),this,SLOT(disableplay()));
    connect(musiclist,SIGNAL(mediaRemoved(int,int)),this,SLOT(disableplay()));

    connect(ui->actionImport_local_files,SIGNAL(triggered()),this,SLOT(importsongs()));
    connect(ui->actionPlay,SIGNAL(triggered()),this,SLOT(toPlay()));
    connect(ui->actionPause,SIGNAL(triggered()),this,SLOT(toPause()));
    connect(ui->actionStop,SIGNAL(triggered()),this,SLOT(toStop()));
    connect(ui->actionNext,SIGNAL(triggered()),this,SLOT(on_nextButton_clicked()));
    connect(ui->actionPrevious,SIGNAL(triggered()),this,SLOT(on_previousButton_clicked()));
    connect(ui->actionFaster,SIGNAL(triggered()),this,SLOT(faster()));
    connect(ui->actionSlower,SIGNAL(triggered()),this,SLOT(slower()));
    connect(ui->actionAddprogress,SIGNAL(triggered()),this,SLOT(addProgress()));
    connect(ui->actionSubprograss,SIGNAL(triggered()),this,SLOT(subProgress()));
    connect(ui->actionbgblue,SIGNAL(triggered()),this,SLOT(tobgblue()));
    connect(ui->actionbgyellow,SIGNAL(triggered()),this,SLOT(tobgyellow()));
    connect(ui->actionbggreen,SIGNAL(triggered()),this,SLOT(tobggreen()));
    connect(ui->actionRandom,SIGNAL(triggered()),this,SLOT(torandom()));
    connect(ui->actionLoop,SIGNAL(triggered()),this,SLOT(toloop()));
    connect(ui->actionCurrentItemInLoop,SIGNAL(triggered()),this,SLOT(tooneloop()));
    connect(ui->actionSequential,SIGNAL(triggered()),this,SLOT(toseq()));
    connect(ui->actionClear_list,SIGNAL(triggered()),this,SLOT(clearlist()));

    connect(ui->action_Qt,SIGNAL(triggered()),this,SLOT(aboutQt()));
    connect(ui->actionGong,SIGNAL(triggered()),this,SLOT(aboutGong()));
    connect(ui->action_Auther,SIGNAL(triggered()),this,SLOT(aboutAuther()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


//volumn
void MainWindow::on_volumnBar_valueChanged(int position)
{
    player->setVolume(position);
    volumnMatch();
}

void MainWindow::volumnMatch()
{
    if(ui->volumnBar->value()==0)
        ui->volumnButton->setIcon(QIcon(":/image/novolumnimage"));
    else if(ui->volumnBar->value()>60)
        ui->volumnButton->setIcon(QIcon(":/image/bigvolumnimage"));
    else if(ui->volumnBar->value()>30)
        ui->volumnButton->setIcon(QIcon(":/image/middlevolumnimage"));
    else
        ui->volumnButton->setIcon(QIcon(":/image/smallvolumnimage"));
}

void MainWindow::on_volumnButton_clicked()
{
    if(ui->volumnBar->value()!=0){
        volumn = ui->volumnBar->value();
        ui->volumnBar->setValue(0);
    }else{
        ui->volumnBar->setValue(volumn);
    }
    player->setVolume(ui->volumnBar->value());
    volumnMatch();
}


//play
void MainWindow::toPlay()
{
    loadinfo=false;
    player->play();
    ui->playButton->setIcon(QIcon(":/image/waitimage"));
}

void MainWindow::toPause()
{
    player->pause();
    ui->playButton->setIcon(QIcon(":/image/playimage"));
}

void MainWindow::toStop()
{
    player->stop();
    ui->playButton->setIcon(QIcon(":/image/playimage"));
}

void MainWindow::on_playButton_clicked()
{
    loadinfo=false;
    if(player->state()!=QMediaPlayer::PlayingState)
        toPlay();
    else if(player->state()==QMediaPlayer::PlayingState)
        toPause();
}

void MainWindow::on_nextButton_clicked()
{
    loadinfo=false;
    if(musiclist->isEmpty())
        return;
    musiclist->next();
}

void MainWindow::on_previousButton_clicked()
{
    loadinfo=false;
    if(musiclist->isEmpty())
        return;
    musiclist->previous();
}


//progress
void MainWindow::positionChanged(qint64 position)
{
    ui->songProgress->setValue(position);
    now1 = position/1000/60;
    now2 = position/1000 - now1*60;
    UpdateTime();
}

void MainWindow::durationChanged(qint64 duration)
{
    ui->songProgress->setRange(0,duration);
    total1 = duration/1000/60;
    total2 = duration/1000 - total1*60;
    UpdateTime();
}

void MainWindow::UpdateTime(){
    QString time = QString::number(now1)+":"+QString::number(now2)+"\\"+QString::number(total1)+":"+QString::number(total2);
    ui->timelabel->setText(time);
}

//进度加5秒
void MainWindow::addProgress()
{
    qint64 position = player->position();
    if(player->duration()-position<5000)
        return;
    else
        player->setPosition(position+5000);
}

//进度减5秒
void MainWindow::subProgress()
{
    qint64 position = player->position();
    if(position<5000)
        return;
    else
        player->setPosition(position-5000);
}

//蓝色背景
void MainWindow::tobgblue()
{
    QPixmap pixmap = QPixmap(":/image/bgblue").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);
    ui->actionbgblue->setChecked(true);
    ui->actionbggreen->setChecked(false);
    ui->actionbgyellow->setChecked(false);
}

//黄色背景
void MainWindow::tobgyellow()
{
    QPixmap pixmap = QPixmap(":/image/bgyellow").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);
    ui->actionbgblue->setChecked(false);
    ui->actionbggreen->setChecked(false);
    ui->actionbgyellow->setChecked(true);
}

//绿色背景
void MainWindow::tobggreen()
{
    QPixmap pixmap = QPixmap(":/image/bggreen").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);
    ui->actionbgblue->setChecked(false);
    ui->actionbggreen->setChecked(true);
    ui->actionbgyellow->setChecked(false);
}

//下拉列表
void MainWindow::on_listButton_clicked()
{
    if(ui->listView->isHidden()){
        ui->listView->show();
        ui->listButton->setIcon(QIcon(":/image/listUp"));
    }
    else{
        ui->listView->hide();
        ui->listButton->setIcon(QIcon(":/image/listDown"));
    }
}

//列表循环
void MainWindow::toloop()
{
    musiclist->setPlaybackMode(QMediaPlaylist::Loop);
    ui->modelButton->setIcon(QIcon(":/image/loop"));
}

//顺序播放
void MainWindow::toseq()
{
    musiclist->setPlaybackMode(QMediaPlaylist::Sequential);
    ui->modelButton->setIcon(QIcon(":/image/seq"));
}

//单曲循环
void MainWindow::tooneloop()
{
    musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    ui->modelButton->setIcon(QIcon(":/image/oneloop"));
}

//随机播放
void MainWindow::torandom()
{
    musiclist->setPlaybackMode(QMediaPlaylist::Random);
    ui->modelButton->setIcon(QIcon(":/image/random"));
}

//选择播放模式
void MainWindow::on_modelButton_clicked()
{
    switch(musiclist->playbackMode()){
    case QMediaPlaylist::Random:
        toloop();
        break;
    case QMediaPlaylist::Loop:
        tooneloop();
        break;
    case QMediaPlaylist::CurrentItemInLoop:
        toseq();
        break;
    case QMediaPlaylist::Sequential:
        torandom();
        break;
    default:break;
    }
}

//关于本程序
void MainWindow::aboutGong()
{
   QMessageBox message(QMessageBox::NoIcon, "About Auther",tr("<h1>共产主义</h1>\
<p><font color=\"white\">----------------------------------------------------------------------------------------</font></p>\
<p>共产主义社会最根本的特征表现为三点。一是物质财富极大丰富，消费资料按需分配；二是社会关系高度和谐，人们精神境界极大提高；三是每个人自由而全面的发展，人类实现从必然王国向自由王国的飞跃。共产主义是人类最美好、最理想的社会，曾有无数志士仁人为之实现付出了自己最宝贵的生命，但它的最终实现仍是一个遥远而漫长的过程。共产主义社会的整体实现不是一蹴而就的事，但它一定会实现。只要我们志存高远，脚踏实地，发扬“愚公移山”的精神，总有那么一天，共产主义理想社会定会变成真真现实。</p>\
<p>共产主义是通过消灭生产资料私人占有制，去消除社会隔阂和阶级，以把全人类从压迫和贫困中解放的思想，并建立没有阶级制度、没有生产资料私有制、没有政府，以及集体生产的社会。</p>\
<p>马克思充分研究了人类的历史、经济和科技的发展，发现人类社会是以物质生产为基础的，现有生产力所决定的分工造成的不同人的经济地位决定了不同人的社会地位，人们之间的经济关系决定了整个社会的形态，法律、道德等上层建筑只是由经济地位决定的人们的社会地位的反映。</p>\
<p>“这个世界在最初并没有真实<br>\
也没有谎言<br>\
只有俨然存在的事实<br>\
可是,<br>\
存在于这个世界的所有事物<br>\
只会将对自己有利的\"事实\"误认为真实而活<br>\
因为不这么做<br>\
也没有其他生存的理由了<br>\
但实际上,对于占据了大半个世界的无力存在来说<br>\
不适合用来肯定自己的\"事实\"<br>\
才是所有的真实”</p>\
<p><font color=\"white\">-------------------------------------------</font>---马克思</p>"));
    message.setIconPixmap(QPixmap(":/image/Gong"));
    message.exec();
}

//关于作者
void MainWindow::aboutAuther()
{
    QMessageBox message(QMessageBox::NoIcon, "About Auther",tr("<h1>习近平</h1>\
<p><font color=\"white\">-------------------------------------------------------</font></p>\
<p>习近平，1953年6月生，陕西富平人，1969年1月参加工作，1974年1月加入中国共产党，清华大学人文社会学院马克思主义理论与思想政治教育专业毕业，在职研究生学历，法学博士学位。</p>\
<p>中共中央总书记，国家主席，中共中央军事委员会主席，国家中央军事委员会主席。</p>\
<p>第十五届中央候补委员，十六届、十七届、十八届中央委员，十七届中央政治局委员、常委、中央书记处书记，十八届中央政治局委员、常委、中央委员会总书记。第十一届全国人大第一次会议当选为中华人民共和国副主席。十七届五中全会增补为中共中央军事委员会副主席。第十一届全国人大常委会第十七次会议任命为中华人民共和国中央军事委员会副主席。十八届一中全会任中共中央军事委员会主席。第十二届全国人大第一次会议当选为中华人民共和国主席、中华人民共和国中央军事委员会主席。</p>\
<p>“我决定了，从今天起，我要选择一条不会让自己后悔的路。我要创造出属于自己的忍道！”</p>\
<p><font color=\"white\">-------------------------------------------</font>---习近平</p>"));
    message.setIconPixmap(QPixmap(":/image/auther"));
    message.exec();
}

//关于Qt
void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(NULL, "About Qt");
}

//导入本地歌曲
void MainWindow::importsongs()
{
    loadinfo=true;
    QString initialName=QDir::homePath();
    QStringList pathList=QFileDialog::getOpenFileNames(this, tr("选择文件"), initialName, tr("MP3 Files (*.mp3)"));
    for(int i=0; i<pathList.size(); ++i)
    {
        QString path=QDir::toNativeSeparators(pathList.at(i));

        if(!path.isEmpty())
        {
            musiclist->addMedia(QUrl::fromLocalFile(path));
        }
    }
    musiclist->setCurrentIndex(0);
}

//加载歌曲信息
void MainWindow::Loadinfo(QMediaPlayer::MediaStatus status)
{
    bool changed = true;
    if(status==QMediaPlayer::LoadedMedia){
        changed=false;
        QString info = player->metaData(QMediaMetaData::Title).toString()+" ---- "+player->metaData(QMediaMetaData::Author).toString();
        ui->songInfo->setText(info);
        if(loadinfo){
            bool exist = false;
            QString s1 = player->metaData(QMediaMetaData::Title).toString();
            for(int i=0;i<ui->listView->rowCount();i++)
            {
                if(ui->listView->item(i,0)->text() == s1){
                    exist = true;
                    break;
                }

            }
            if(!exist){
                int rownum=ui->listView->rowCount();//行数
                ui->listView->insertRow(rownum);//插入空行
                QString s2 = player->metaData(QMediaMetaData::Author).toString();
                QString s3 = player->metaData(QMediaMetaData::AlbumTitle).toString();
                QString s4 = player->metaData(QMediaMetaData::Year).toString();
                int s = player->metaData(QMediaMetaData::Duration).toInt();
                int min = s/1000/60;
                int sec = s/1000 - min*60;
                QString s5 = QString::number(min)+"\:"+QString::number(sec);
                ui->listView->setItem(rownum, 0, new QTableWidgetItem(s1));
                ui->listView->setItem(rownum, 1, new QTableWidgetItem(s2));
                ui->listView->setItem(rownum, 2, new QTableWidgetItem(s3));
                ui->listView->setItem(rownum, 3, new QTableWidgetItem(s4));
                ui->listView->setItem(rownum, 4, new QTableWidgetItem(s5));
                if(rownum%2==1){
                    for(int i=0;i<ui->listView->columnCount();i++){
                        QTableWidgetItem *item = ui->listView->item(rownum,i);
                        item->setBackgroundColor(QColor(240,255,240));
                    }
                }
            }
        }

    }
    if(loadinfo){
        if(changed)
            musiclist->setCurrentIndex(musiclist->currentIndex());
        else if(musiclist->currentIndex()<musiclist->mediaCount()-1){
            musiclist->next();
        }
    }
//    toPlay();
}

//播放速率增加
void MainWindow::faster()
{
    if(playrate>=3.0)
        return;
    else{
        playrate+=0.1;
        playratechange();
    }
}

//播放速率减小
void MainWindow::slower()
{
    if(playrate<=0)
        return;
    else{
        playrate-=0.1;
        playratechange();
    }
}

//显示播放速率
void MainWindow::playratechange()
{
    player->setPlaybackRate(playrate);
    if(playrate==1.0)
        ui->playrateLabel->setText("");
    else
        ui->playrateLabel->setText("  x"+QString::number(playrate));

}

//表单为空时禁用按钮
void MainWindow::disableplay()
{
    if(musiclist->mediaCount()==0){
        ui->playButton->setDisabled(true);
        ui->nextButton->setDisabled(true);
        ui->previousButton->setDisabled(true);
        ui->actionClear_list->setDisabled(true);
        ui->songInfo->setText("----");
    }
    else{
        ui->playButton->setEnabled(true);
        ui->nextButton->setEnabled(true);
        ui->previousButton->setEnabled(true);
        ui->actionClear_list->setEnabled(true);
    }
}

//表单双击选歌
void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    loadinfo=false;
    musiclist->setCurrentIndex(index.row());
}

//表单右键
void MainWindow::on_listView_customContextMenuRequested(const QPoint &pos)
{
    QMenu *popMenu = new QMenu(ui->listView);
    QAction *action_del = new QAction("移除",this);
    popMenu->addAction(action_del);
    connect(action_del,SIGNAL(triggered()),this,SLOT(deleterow()));
    popMenu->exec(QCursor::pos());

}

//删除一首歌
void MainWindow::deleterow()
{
    int rownow = ui->listView->currentRow();
    ui->listView->removeRow(rownow);
    musiclist->removeMedia(rownow);
}

//清空歌单
void MainWindow::clearlist()
{
    switch(QMessageBox::question(NULL, "清空列表", "确认清空整个歌单？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)){
    case QMessageBox::Yes:
        disableplay();
        musiclist->clear();
        ui->listView->clearContents();
        ui->listView->setRowCount(0);
        break;
    case QMessageBox::No:
        break;
    }
}
