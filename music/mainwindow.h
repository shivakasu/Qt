#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlayList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void UpdateTime();
    void volumnMatch();
    void playratechange();

private slots:
    void on_playButton_clicked();
    void on_volumnBar_valueChanged(int position);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void on_listButton_clicked();
    void on_volumnButton_clicked();
    void on_modelButton_clicked();
    void importsongs();
    void on_nextButton_clicked();
    void on_previousButton_clicked();
    void Loadinfo(QMediaPlayer::MediaStatus);
    void toPlay();
    void toPause();
    void toStop();
    void faster();
    void slower();
    void addProgress();
    void subProgress();
    void disableplay();
    void tobgblue();
    void tobgyellow();
    void tobggreen();
    void toseq();
    void toloop();
    void tooneloop();
    void torandom();
    void aboutGong();
    void aboutAuther();
    void aboutQt();
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_listView_customContextMenuRequested(const QPoint &pos);
    void deleterow();
    void clearlist();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *musiclist;
    qint64 now1;
    qint64 now2;
    qint64 total1;
    qint64 total2;
    int volumn;
    double playrate;
    bool loadinfo;
};

#endif // MAINWINDOW_H
