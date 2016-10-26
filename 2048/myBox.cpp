#include "myBox.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <math.h>
#include <QStyleOptionGraphicsItem>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QTimer>

OneBox::OneBox(const QColor &color, int num, int index)
    :brushColor(color),number(num)
{

}

QRectF OneBox::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-penWidth/2, -penWidth/2, 80+penWidth, 80+penWidth);
}

void OneBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //����ʹ����ͼ
    //painter->drawPixmap(0, 0, 80, 80, QPixmap(":/images/box.gif"));
    painter->drawPixmap(0, 0, 80, 80, QPixmap(":/images/grid.png"));
    painter->setBrush(brushColor);
    QColor penColor = brushColor;
    //����ɫ͸���ȼ���
    penColor.setAlpha(20);
    painter->setPen(penColor);
    painter->setPen(Qt::gray);
    QRectF rectangle(0, 0, 80, 80);
    painter->drawRoundedRect(rectangle, 10, 10);

    painter->setPen(Qt::white);
    painter->setFont(QFont("Times", 20, QFont::Bold));
    painter->drawText(QRect(0, 0, 80, 80), Qt::AlignCenter, tr("%1").arg(number));

}

QPainterPath OneBox::shape() const
{
    QPainterPath path;
     path.addRect(1, 1, 78, 78); //��ֹ���ڷ��鱻���Ϊ��ײ
    return path;
}


BoxGroup::BoxGroup()
{
    setFlags(QGraphicsItem::ItemIsFocusable); //���ô˱�־��ʹ���Խ��ռ����¼�����
    sumScore = 0;
    downKey = false, upKey = false, leftKey = false, rightKey = false;
}

QRectF BoxGroup::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(10-penWidth/2, 80-penWidth/2, 330+penWidth, 390+penWidth);
}


void BoxGroup::createBox(const QPointF &point, int numberBox)
{
    static const QColor colorTable[11] = {
               QColor(200, 0, 0, 100),
               QColor(255, 200, 0, 100),
               QColor(0, 0, 200, 100),
               QColor(0, 200, 0, 100),
               QColor(0, 200, 255, 100),
               QColor(200, 0, 255, 100),
               QColor(150, 100, 100, 100),

               QColor(100, 200, 0, 100),
               QColor(100, 0, 255, 100),
               QColor(0, 100, 255, 100),
               QColor(150, 0, 100, 100)
    };

    static const int numberTable[11] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    int number = numberBox;
    if(numberBox == randomNumber)
    {
        //����0��1�������������Ϊ3:1
        int i = qrand()%4;
      //  qDebug() << i;
        number = i<3?0:1;
    }

    OneBox *temp = new OneBox(colorTable[number], numberTable[number], number);
    addToGroup(temp);
    temp->setPos(point);
/*
    if(numberBox == randomNumber)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(temp, "scale");
        animation->setEasingCurve(QEasingCurve::OutBounce); //������������
        animation->setDuration(50);
        animation->setStartValue(0.25);
        animation->setEndValue(1);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
*/

}

void BoxGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
     painter->drawPixmap(10, 80, 320, 320, QPixmap(":/images/boxGroupBackground.png"));
}

int BoxGroup::getScoreValue()
{
    return sumScore;
}


void BoxGroup::keyPressEvent(QKeyEvent *event)
{
    QList<QGraphicsItem *> list;

    if(downKey&&upKey&&leftKey&&rightKey)
        emit gameFinished();

    switch(event->key())
    {
        case Qt::Key_Down:{ //��Ҫ�߼����������´����������������ͬ��

            //�ڷ����·���ǰ���жϵ�ǰ�����з�����ƶ���������ܵĻ������ܷ����·���
            //��Ϊ���������ĳ�����ƶ�40����Ȼδ�����µ���ײ 2.�ƶ������������������numberֵ��ͬ
              int i;
              bool key = false;
             for(i=0; i<childItems().size(); i++)
             {
                  childItems().at(i)->moveBy(0, 40);
                  if(childItems().at(i)->collidingItems().count()<=1)
                  {
                      childItems().at(i)->moveBy(0, -40);
                      break;
                  }
                  childItems().at(i)->moveBy(0, -40);
             }
             for(int xx=10; xx<=250; xx+=80)
             {
                 if(key) break;
                 for(int yy=240; yy>=80; yy-=80)
                 {
                     list = scene()->items(xx-1, yy-1, 82, 164, Qt::ContainsItemShape) ;
                     if(list.size()==2)
                     {
                        OneBox *box1 = (OneBox *)list.at(0);
                        OneBox *box2 =(OneBox *)list.at(1);
                        if(box1->number == box2->number)
                        {
                            key = true;
                            break;
                        }
                     }
                 }
             }
             if(i==childItems().size() && !key)
             {
                 qDebug() << tr("�������ƶ���");
                 if(i==16) downKey = true;
                 return;
             }

            //������Լ����ƶ�����������������ļ�ֵ
            upKey = false;
            leftKey = false;
            rightKey = false;

            for(int y=239; y>=79; y-=80)  //���У�ȫ�����ƣ�֪����ײΪֹ��������ײ���˻�40, 40Ϊ����ߴ��һ��
            {

                list = this->scene()->items(9, y, 322, 82 , Qt::ContainsItemShape);
                //qDebug() << this->scene() << list;
                foreach(QGraphicsItem *item, list)
                {
                    while(item->collidingItems().count()<=1)
                        item->moveBy(0, 40);
                    item->moveBy(0, -40); //����whileѭ��ʱ˵����߿�򷽿�֮�䷢����ײ��Ӧ�˺�һ��
                }

            }
            int index = 0;//���ں��������ײ����ʱ�Ϸ����ƶ������С
            //������ײ, ���ƶ�������ѡȡ2�������С�����򣬽����ж�
            for(int y=239; y>=79; y-=80)
            {
              for(int x=9; x<=249; x+=80)
              {
                  list = scene()->items(x, y, 82, 162, Qt::ContainsItemShape);
                  if(list.size()==2)
                  {
                    OneBox *box1 = (OneBox *)list.at(0);
                    OneBox *box2 = (OneBox *)list.at(1);
                    if(box1->number == box2->number)
                    {

                        //û����2Ϊ�׵ĺ������ǿ����û��׹�ʽ���:log2(N)=log10(N)/log10(2)
                        double num = box1->number*2; //����num����Ϊint������������

                        num = log10(num)/log10(2);
                       // qDebug() << "num= " << num;
                        //qDebug()  << box1->number << "log10(box1->number*2)/log10(2)-1:" << num-1;

                        sumScore += box1->number*2;
                        emit scoreChanged();
                        qDebug() << tr("��ײ") << sumScore;
                        foreach(QGraphicsItem *item, list)
                        {
                            item->hide(); //��Ϊ��ȷ������ɾ��������ʲôʱ�������Ƚ������ص�������scene()->items()�����Ͳ�����ѡ�и÷��飬��ֻ��ѡ�����صķ���
                            removeFromGroup(item);
                            OneBox *box = (OneBox*)item;
                            box->deleteLater(); //�ú���ΪQObject�ĺ�����item�����̳���QObject���ʲ���ֱ�ӵ��ã�����ת��

                        }
                        //QTimer::singleShot(400, this, SLOT());
                        createBox(QPointF(x+1, (y+1)+80), num-1);

                        //��������ײ�������Ϸ��ķ�������
                        list = scene()->items(x, 79, 82, 162-80*index, Qt::ContainsItemShape);
                        foreach(QGraphicsItem *item, list)
                        {
                            item->moveBy(0, 80); //������ײ��ζ�Ų���һ�������С�Ŀ��ƶ��ռ�
                        }

                    }
                  }
              }
              index++;
            }
            emit needNewBox();

        }break;
        case Qt::Key_Up:{
        //�ڷ����·���ǰ���жϵ�ǰ�����з�����ƶ���������ܵĻ������ܷ����·���
        //��Ϊ���������ĳ�����ƶ�40����Ȼδ�����µ���ײ 2.�ƶ������������������numberֵ��ͬ
          int i;
          bool key = false;
         for(i=0; i<childItems().size(); i++)
         {
              childItems().at(i)->moveBy(0, -40);
              if(childItems().at(i)->collidingItems().count()<=1)
              {
                  childItems().at(i)->moveBy(0, 40);
                  break;
              }
              childItems().at(i)->moveBy(0, 40);
         }
         for(int xx=10; xx<=250; xx+=80)
         {
             if(key) break;
             for(int yy=80; yy<=240; yy+=80)
             {
                 list = scene()->items(xx-1, yy-1, 82, 164, Qt::ContainsItemShape)      ;
                 if(list.size()==2)
                 {
                    OneBox *box1 = (OneBox *)list.at(0);
                    OneBox *box2 =(OneBox *)list.at(1);
                    if(box1->number == box2->number)
                    {
                        key = true;
                        break;
                    }
                 }
             }
         }
         if(i==childItems().size() && !key)
         {
             qDebug() << tr("�������ƶ���");
             if(i==16) upKey = true;
             return;
         }

         //������Լ����ƶ�����������������ļ�ֵ
         downKey = false;
         leftKey = false;
         rightKey = false;

        for(int y=159; y<=319; y+=80)  //���У�ȫ�����ƣ�֪����ײΪֹ��������ײ���˻�40, 40Ϊ����ߴ��һ��
        {

            list = this->scene()->items(9, y, 322, 82 , Qt::ContainsItemShape);
            foreach(QGraphicsItem *item, list)
            {
                while(item->collidingItems().count()<=1)
                    item->moveBy(0, -40);
                item->moveBy(0, 40); //����whileѭ��ʱ˵����߿�򷽿�֮�䷢����ײ��Ӧ�˺�һ��
            }

        }
        //������ײ
        int index = 0;//���ں��������ײ����ʱ�Ϸ����ƶ������С

        for(int y=79; y<=239; y+=80)
        {
          for(int x=9; x<=249; x+=80)
          {
              list = scene()->items(x, y, 82, 162, Qt::ContainsItemShape);
              if(list.size()==2)
              {
                OneBox *box1 = (OneBox *)list.at(0);
                OneBox *box2 = (OneBox *)list.at(1);
                if(box1->number == box2->number)
                {

                    //û����2Ϊ�׵ĺ������ǿ����û��׹�ʽ���:log2(N)=log10(N)/log10(2)
                    double num = box1->number*2; //����num����Ϊint������������

                    num = log10(num)/log10(2);

                    sumScore += box1->number*2;
                    emit scoreChanged();
                    qDebug() << tr("��ײ") << sumScore;
                    foreach(QGraphicsItem *item, list)
                    {
                        item->hide(); //��Ϊ��ȷ������ɾ��������ʲôʱ�������Ƚ������ص�������scene()->items()�����Ͳ�����ѡ�и÷��飬��ֻ��ѡ�����صķ���
                        removeFromGroup(item);
                        OneBox *box = (OneBox*)item;
                        box->deleteLater(); //�ú���ΪQObject�ĺ�����item�����̳���QObject���ʲ���ֱ�ӵ��ã�����ת��
                    }
                    createBox(QPointF(x+1, y+1), num-1);

                    //��������ײ�������·��ķ�������
                    list = scene()->items(x, y+160, 82, 162-80*index, Qt::ContainsItemShape);
                    foreach(QGraphicsItem *item, list)
                    {
                        item->moveBy(0, -80); //������ײ��ζ�Ų���һ�������С�Ŀ��ƶ��ռ�
                    }

                }
              }
          }
          index++;
         }
         emit needNewBox();

        }break;
        case Qt::Key_Left:{
        //�ڷ����·���ǰ���жϵ�ǰ�����з�����ƶ���������ܵĻ������ܷ����·���
        //��Ϊ���������ĳ�����ƶ�40����Ȼδ�����µ���ײ 2.�ƶ������������������numberֵ��ͬ
          int i;
          bool key = false;
         for(i=0; i<childItems().size(); i++)
         {
              childItems().at(i)->moveBy(-40, 0);
              if(childItems().at(i)->collidingItems().count()<=1)
              {
                  childItems().at(i)->moveBy(40, 0);
                  break;
              }
              childItems().at(i)->moveBy(40, 0);
         }
         for(int xx=10; xx<=170; xx+=80)
         {
             if(key) break;
             for(int yy=320; yy>=80; yy-=80)
             {
                 list = scene()->items(xx-1, yy-1, 162, 82, Qt::ContainsItemShape)      ;
                 if(list.size()==2)
                 {
                    OneBox *box1 = (OneBox *)list.at(0);
                    OneBox *box2 =(OneBox *)list.at(1);
                    if(box1->number == box2->number)
                    {
                        key = true;
                        break;
                    }
                 }
             }
         }


         if(i==childItems().size() && !key)
         {
             qDebug() << tr("�������ƶ���");
             if(i==16) leftKey = true;
             return;
         }

         //������Լ����ƶ�����������������ļ�ֵ
         upKey = false;
         downKey = false;
         rightKey = false;

        for(int x=89; x<=249; x+=80)  //���У�ȫ�����ƣ�֪����ײΪֹ��������ײ���˻�40, 40Ϊ����ߴ��һ��
        {

            list = this->scene()->items(x, 79, 82, 322, Qt::ContainsItemShape);
            //qDebug() << this->scene() << list;
            foreach(QGraphicsItem *item, list)
            {
                while(item->collidingItems().count()<=1)
                    item->moveBy(-40, 0);
                item->moveBy(40, 0); //����whileѭ��ʱ˵����߿�򷽿�֮�䷢����ײ��Ӧ�˺�һ��
            }

        }
        //������ײ
        int index = 0; //���ں��������ײ����ʱ�Ϸ����ƶ������С

         for(int x=9; x<=169; x+=80)
        {
          for(int y=319; y>=79; y-=80)
          {
              list = scene()->items(x, y,164, 82, Qt::ContainsItemShape);
              if(list.size()==2)
              {
                OneBox *box1 = (OneBox *)list.at(0);
                OneBox *box2 = (OneBox *)list.at(1);
                if(box1->number == box2->number)
                {

                    //û����2Ϊ�׵ĺ������ǿ����û��׹�ʽ���:log2(N)=log10(N)/log10(2)
                    double num = box1->number*2; //����num����Ϊint������������

                    num = log10(num)/log10(2);

                    sumScore += box1->number*2;
                    emit scoreChanged();
                    qDebug() << tr("��ײ") << sumScore;
                    foreach(QGraphicsItem *item, list)
                    {
                        item->hide(); //��Ϊ��ȷ������ɾ��������ʲôʱ�������Ƚ������ص�������scene()->items()�����Ͳ�����ѡ�и÷��飬��ֻ��ѡ�����صķ���
                        removeFromGroup(item);
                        OneBox *box = (OneBox*)item;
                        box->deleteLater(); //�ú���ΪQObject�ĺ�����item�����̳���QObject���ʲ���ֱ�ӵ��ã�����ת��
                    }
                    createBox(QPointF(x+1, y+1), num-1);

                    list = scene()->items(x+160, y, 162-80*index, 82,Qt::ContainsItemShape);
                    foreach(QGraphicsItem *item, list)
                    {
                        item->moveBy(-80, 0); //������ײ��ζ�Ų���һ�������С�Ŀ��ƶ��ռ�
                    }

                }
              }
          }
         index++;
        }
        emit needNewBox();

        }break;

        case Qt::Key_Right:{
        //�ڷ����·���ǰ���жϵ�ǰ�����з�����ƶ���������ܵĻ������ܷ����·���
        //��Ϊ���������ĳ�����ƶ�40����Ȼδ�����µ���ײ 2.�ƶ������������������numberֵ��ͬ
          int i;
          bool key = false;
         for(i=0; i<childItems().size(); i++)
         {
              childItems().at(i)->moveBy(40, 0);
              if(childItems().at(i)->collidingItems().count()<=1)
              {
                  childItems().at(i)->moveBy(-40, 0);
                  break;
              }
              childItems().at(i)->moveBy(-40, 0);
         }
         for(int xx=10; xx<=170; xx+=80)
         {
             if(key) break;
             for(int yy=320; yy>=80; yy-=80)
             {
                 list = scene()->items(xx-1, yy-1, 162, 82, Qt::ContainsItemShape)      ;
                 if(list.size()==2)
                 {
                    OneBox *box1 = (OneBox *)list.at(0);
                    OneBox *box2 =(OneBox *)list.at(1);
                    if(box1->number == box2->number)
                    {
                        key = true;
                        break;
                    }
                 }
             }
         }


         if(i==childItems().size() && !key)
         {
             qDebug() << tr("�������ƶ���");
             if(i==16) rightKey = true;
             return;
         }

         //������Լ����ƶ�����������������ļ�ֵ
         upKey = false;
         downKey = false;
         leftKey = false;

        for(int x=169; x>=9; x-=80)  //���У�ȫ�����ƣ�֪����ײΪֹ��������ײ���˻�40, 40Ϊ����ߴ��һ��
        {

            list = this->scene()->items(x, 79, 82, 322, Qt::ContainsItemShape);
            foreach(QGraphicsItem *item, list)
            {
                while(item->collidingItems().count()<=1)
                    item->moveBy(40, 0);
                item->moveBy(-40, 0); //����whileѭ��ʱ˵����߿�򷽿�֮�䷢����ײ��Ӧ�˺�һ��
            }

        }

        //������ײ
        int index = 0; //���ں��������ײ����ʱ�Ϸ����ƶ������С
        for(int x=169; x>=9; x-=80)
        {
          for(int y=319; y>=79; y-=80)
          {
              list = scene()->items(x, y,162, 82, Qt::ContainsItemShape);
              if(list.size()==2)
              {
                OneBox *box1 = (OneBox *)list.at(0);
                OneBox *box2 = (OneBox *)list.at(1);
                if(box1->number == box2->number)
                {

                    //û����2Ϊ�׵ĺ������ǿ����û��׹�ʽ���:log2(N)=log10(N)/log10(2)
                    double num = box1->number*2; //����num����Ϊint������������

                    num = log10(num)/log10(2);

                    sumScore += box1->number*2;
                    emit scoreChanged();
                    qDebug() << tr("��ײ") << sumScore;
                    foreach(QGraphicsItem *item, list)
                    {
                        item->hide(); //��Ϊ��ȷ������ɾ��������ʲôʱ�������Ƚ������ص�������scene()->items()�����Ͳ�����ѡ�и÷��飬��ֻ��ѡ�����صķ���
                        removeFromGroup(item);
                        OneBox *box = (OneBox*)item;
                        box->deleteLater(); //�ú���ΪQObject�ĺ�����item�����̳���QObject���ʲ���ֱ�ӵ��ã�����ת��
                    }
                     createBox(QPointF(x+1+80, y+1), num-1);

                    //��������ײ�������󷽵ķ�������
                    list = scene()->items(9, y, 162-80*index, 82,Qt::ContainsItemShape);
                    foreach(QGraphicsItem *item, list)
                    {
                        item->moveBy(80, 0); //������ײ��ζ�Ų���һ�������С�Ŀ��ƶ��ռ�
                    }
                }
              }
          }
          index++;
        }
        emit needNewBox();

        }break;
        default:break;
    }

}



//ɾ���������е�С����
/*ʹ��QGraphicsItemGroup��removeFromGroup()������С����ӷ��������ƶ��������С������������һ��������
������øò���Ϊ�棬��ô����Ҫ�ӷ��������Ƴ�С���飬��Ҫ����ЩС�������ٵ�*/
void BoxGroup::clearBoxGroup(bool destroyBox)
{
    QList<QGraphicsItem *> itemList = this->childItems();
    QGraphicsItem *item;
    foreach(item, itemList)
    {
        removeFromGroup(item);//�ӷ��������Ƴ���������ڳ�����λ�ú�ת�������ֲ���
        if(destroyBox)
        {
            OneBox *box = (OneBox*)item;
            box->deleteLater(); //�ú���ΪQObject�ĺ�����item�����̳���QObject���ʲ���ֱ�ӵ��ã�����ת��
        }
    }
}
