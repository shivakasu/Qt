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
    //方块使用贴图
    //painter->drawPixmap(0, 0, 80, 80, QPixmap(":/images/box.gif"));
    painter->drawPixmap(0, 0, 80, 80, QPixmap(":/images/grid.png"));
    painter->setBrush(brushColor);
    QColor penColor = brushColor;
    //将颜色透明度减少
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
     path.addRect(1, 1, 78, 78); //防止相邻方块被检测为碰撞
    return path;
}


BoxGroup::BoxGroup()
{
    setFlags(QGraphicsItem::ItemIsFocusable); //设置此标志已使可以接收键盘事件控制
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
        //产生0和1的随机数，概率为3:1
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
        animation->setEasingCurve(QEasingCurve::OutBounce); //缓和曲线类型
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
        case Qt::Key_Down:{ //主要逻辑处理在向下处理这个，其他方向同理

            //在放入新方块前先判断当前方块有否继续移动，如果不能的话，则不能放入新方块
            //分为两种情况：某方块移动40后仍然未发生新的碰撞 2.移动方向上相邻两方块的number值相同
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
                 qDebug() << tr("不能再移动了");
                 if(i==16) downKey = true;
                 return;
             }

            //如果可以继续移动，则重置其他方向的键值
            upKey = false;
            leftKey = false;
            rightKey = false;

            for(int y=239; y>=79; y-=80)  //按行，全部下移，知道碰撞为止，发生碰撞则退回40, 40为方块尺寸的一半
            {

                list = this->scene()->items(9, y, 322, 82 , Qt::ContainsItemShape);
                //qDebug() << this->scene() << list;
                foreach(QGraphicsItem *item, list)
                {
                    while(item->collidingItems().count()<=1)
                        item->moveBy(0, 40);
                    item->moveBy(0, -40); //跳出while循环时说明与边框或方块之间发生碰撞，应退后一步
                }

            }
            int index = 0;//用于后面决定碰撞发生时上方的移动区域大小
            //处理碰撞, 在移动方向上选取2个方格大小的区域，进行判断
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

                        //没有以2为底的函数但是可以用换底公式解决:log2(N)=log10(N)/log10(2)
                        double num = box1->number*2; //这里num不能为int，否则结果出错

                        num = log10(num)/log10(2);
                       // qDebug() << "num= " << num;
                        //qDebug()  << box1->number << "log10(box1->number*2)/log10(2)-1:" << num-1;

                        sumScore += box1->number*2;
                        emit scoreChanged();
                        qDebug() << tr("碰撞") << sumScore;
                        foreach(QGraphicsItem *item, list)
                        {
                            item->hide(); //因为不确定彻底删除发生在什么时候，所以先将其隐藏掉，这样scene()->items()函数就不会再选中该方块，其只能选非隐藏的方块
                            removeFromGroup(item);
                            OneBox *box = (OneBox*)item;
                            box->deleteLater(); //该函数为QObject的函数，item本身不继承自QObject，故不能直接调用，需先转换

                        }
                        //QTimer::singleShot(400, this, SLOT());
                        createBox(QPointF(x+1, (y+1)+80), num-1);

                        //将发生碰撞的区域上方的方块下移
                        list = scene()->items(x, 79, 82, 162-80*index, Qt::ContainsItemShape);
                        foreach(QGraphicsItem *item, list)
                        {
                            item->moveBy(0, 80); //发生碰撞意味着产生一个方格大小的可移动空间
                        }

                    }
                  }
              }
              index++;
            }
            emit needNewBox();

        }break;
        case Qt::Key_Up:{
        //在放入新方块前先判断当前方块有否继续移动，如果不能的话，则不能放入新方块
        //分为两种情况：某方块移动40后仍然未发生新的碰撞 2.移动方向上相邻两方块的number值相同
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
             qDebug() << tr("不能再移动了");
             if(i==16) upKey = true;
             return;
         }

         //如果可以继续移动，则重置其他方向的键值
         downKey = false;
         leftKey = false;
         rightKey = false;

        for(int y=159; y<=319; y+=80)  //按行，全部上移，知道碰撞为止，发生碰撞则退回40, 40为方块尺寸的一半
        {

            list = this->scene()->items(9, y, 322, 82 , Qt::ContainsItemShape);
            foreach(QGraphicsItem *item, list)
            {
                while(item->collidingItems().count()<=1)
                    item->moveBy(0, -40);
                item->moveBy(0, 40); //跳出while循环时说明与边框或方块之间发生碰撞，应退后一步
            }

        }
        //处理碰撞
        int index = 0;//用于后面决定碰撞发生时上方的移动区域大小

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

                    //没有以2为底的函数但是可以用换底公式解决:log2(N)=log10(N)/log10(2)
                    double num = box1->number*2; //这里num不能为int，否则结果出错

                    num = log10(num)/log10(2);

                    sumScore += box1->number*2;
                    emit scoreChanged();
                    qDebug() << tr("碰撞") << sumScore;
                    foreach(QGraphicsItem *item, list)
                    {
                        item->hide(); //因为不确定彻底删除发生在什么时候，所以先将其隐藏掉，这样scene()->items()函数就不会再选中该方块，其只能选非隐藏的方块
                        removeFromGroup(item);
                        OneBox *box = (OneBox*)item;
                        box->deleteLater(); //该函数为QObject的函数，item本身不继承自QObject，故不能直接调用，需先转换
                    }
                    createBox(QPointF(x+1, y+1), num-1);

                    //将发生碰撞的区域下方的方块上移
                    list = scene()->items(x, y+160, 82, 162-80*index, Qt::ContainsItemShape);
                    foreach(QGraphicsItem *item, list)
                    {
                        item->moveBy(0, -80); //发生碰撞意味着产生一个方格大小的可移动空间
                    }

                }
              }
          }
          index++;
         }
         emit needNewBox();

        }break;
        case Qt::Key_Left:{
        //在放入新方块前先判断当前方块有否继续移动，如果不能的话，则不能放入新方块
        //分为两种情况：某方块移动40后仍然未发生新的碰撞 2.移动方向上相邻两方块的number值相同
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
             qDebug() << tr("不能再移动了");
             if(i==16) leftKey = true;
             return;
         }

         //如果可以继续移动，则重置其他方向的键值
         upKey = false;
         downKey = false;
         rightKey = false;

        for(int x=89; x<=249; x+=80)  //按列，全部左移，知道碰撞为止，发生碰撞则退回40, 40为方块尺寸的一半
        {

            list = this->scene()->items(x, 79, 82, 322, Qt::ContainsItemShape);
            //qDebug() << this->scene() << list;
            foreach(QGraphicsItem *item, list)
            {
                while(item->collidingItems().count()<=1)
                    item->moveBy(-40, 0);
                item->moveBy(40, 0); //跳出while循环时说明与边框或方块之间发生碰撞，应退后一步
            }

        }
        //处理碰撞
        int index = 0; //用于后面决定碰撞发生时上方的移动区域大小

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

                    //没有以2为底的函数但是可以用换底公式解决:log2(N)=log10(N)/log10(2)
                    double num = box1->number*2; //这里num不能为int，否则结果出错

                    num = log10(num)/log10(2);

                    sumScore += box1->number*2;
                    emit scoreChanged();
                    qDebug() << tr("碰撞") << sumScore;
                    foreach(QGraphicsItem *item, list)
                    {
                        item->hide(); //因为不确定彻底删除发生在什么时候，所以先将其隐藏掉，这样scene()->items()函数就不会再选中该方块，其只能选非隐藏的方块
                        removeFromGroup(item);
                        OneBox *box = (OneBox*)item;
                        box->deleteLater(); //该函数为QObject的函数，item本身不继承自QObject，故不能直接调用，需先转换
                    }
                    createBox(QPointF(x+1, y+1), num-1);

                    list = scene()->items(x+160, y, 162-80*index, 82,Qt::ContainsItemShape);
                    foreach(QGraphicsItem *item, list)
                    {
                        item->moveBy(-80, 0); //发生碰撞意味着产生一个方格大小的可移动空间
                    }

                }
              }
          }
         index++;
        }
        emit needNewBox();

        }break;

        case Qt::Key_Right:{
        //在放入新方块前先判断当前方块有否继续移动，如果不能的话，则不能放入新方块
        //分为两种情况：某方块移动40后仍然未发生新的碰撞 2.移动方向上相邻两方块的number值相同
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
             qDebug() << tr("不能再移动了");
             if(i==16) rightKey = true;
             return;
         }

         //如果可以继续移动，则重置其他方向的键值
         upKey = false;
         downKey = false;
         leftKey = false;

        for(int x=169; x>=9; x-=80)  //按列，全部右移，知道碰撞为止，发生碰撞则退回40, 40为方块尺寸的一半
        {

            list = this->scene()->items(x, 79, 82, 322, Qt::ContainsItemShape);
            foreach(QGraphicsItem *item, list)
            {
                while(item->collidingItems().count()<=1)
                    item->moveBy(40, 0);
                item->moveBy(-40, 0); //跳出while循环时说明与边框或方块之间发生碰撞，应退后一步
            }

        }

        //处理碰撞
        int index = 0; //用于后面决定碰撞发生时上方的移动区域大小
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

                    //没有以2为底的函数但是可以用换底公式解决:log2(N)=log10(N)/log10(2)
                    double num = box1->number*2; //这里num不能为int，否则结果出错

                    num = log10(num)/log10(2);

                    sumScore += box1->number*2;
                    emit scoreChanged();
                    qDebug() << tr("碰撞") << sumScore;
                    foreach(QGraphicsItem *item, list)
                    {
                        item->hide(); //因为不确定彻底删除发生在什么时候，所以先将其隐藏掉，这样scene()->items()函数就不会再选中该方块，其只能选非隐藏的方块
                        removeFromGroup(item);
                        OneBox *box = (OneBox*)item;
                        box->deleteLater(); //该函数为QObject的函数，item本身不继承自QObject，故不能直接调用，需先转换
                    }
                     createBox(QPointF(x+1+80, y+1), num-1);

                    //将发生碰撞的区域左方的方块右移
                    list = scene()->items(9, y, 162-80*index, 82,Qt::ContainsItemShape);
                    foreach(QGraphicsItem *item, list)
                    {
                        item->moveBy(80, 0); //发生碰撞意味着产生一个方格大小的可移动空间
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



//删除方块组中的小方块
/*使用QGraphicsItemGroup的removeFromGroup()函数将小方块从方块组中移动到场景中。这个函数还有一个参数，
如果设置该参数为真，那么不但要从方块组中移除小方块，还要将这些小方块销毁掉*/
void BoxGroup::clearBoxGroup(bool destroyBox)
{
    QList<QGraphicsItem *> itemList = this->childItems();
    QGraphicsItem *item;
    foreach(item, itemList)
    {
        removeFromGroup(item);//从方块组中移除，但相对于场景的位置和转换都保持不变
        if(destroyBox)
        {
            OneBox *box = (OneBox*)item;
            box->deleteLater(); //该函数为QObject的函数，item本身不继承自QObject，故不能直接调用，需先转换
        }
    }
}
