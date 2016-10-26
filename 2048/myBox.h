#ifndef MYBOX_H
#define MYBOX_H

#include <QGraphicsObject>
#include <QGraphicsTextItem>
#define randomNumber 11

class OneBox:public QGraphicsTextItem{
Q_OBJECT

public:
    OneBox(const QColor &color = Qt::red, int num=0, int zvalue=0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    int number;
private:
    QColor brushColor;

};

class BoxGroup:public QObject, public QGraphicsItemGroup{
Q_OBJECT

public:
    BoxGroup();
    QRectF boundingRect() const;
    //void createBox(const QPointF &point = QPointF(0,0), int numberBox = randomNumber);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int getScoreValue();
    void clearBoxGroup(bool destroyBox = false);

protected:
    void keyPressEvent(QKeyEvent *event);
public slots:
    void createBox(const QPointF &point = QPointF(0,0), int numberBox = randomNumber);


signals:
    void needNewBox();
    void gameFinished();
    void scoreChanged();
private:
    int sumScore;
    bool downKey, upKey, leftKey, rightKey; //在相应方向上全满且不能继续移动时为true
};

#endif // MYBOX_H
