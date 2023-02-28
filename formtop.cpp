#include "formtop.h"
#include "ui_formtop.h"
#include <QGraphicsItem>
#include <QScrollBar>
#include <QVector>
#include <QMap>
#include <iostream>
#include <QDebug>

FormTop::FormTop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormTop)
{
    ui->setupUi(this);
}

//void FormTop::receiveFile(QVector<QList<float>> **mapFile)
void FormTop::receiveFile(QVector<QVector<QVector<QList<float>>>>& mapFile)
{
//    QString temp;
//    temp = strVector[1][3];
//    float fTemp = temp.toFloat()*1000000;
//    int iTemp = int(fTemp);
    float minX=0, maxX=0, minY=0, maxY=0;
    float posX = 0, posY =0;
    int areaX = 10, areaY = 10;
    int beginX = 0, beginY = 0, endX = mapFile.size(), endY = mapFile[0].size();
    if (posX - areaX > 0){beginX=posX - areaX;}
    if (posY - areaY > 0){beginY=posY - areaY;}
    if (posX + areaX < mapFile.size()){endX = posX + areaX;}
    if (posY + areaY < mapFile[0].size()){endY = posY + areaY;}



    QMap<int, QColor> mapColor;
    mapColor.insert(0,Qt::white);
    mapColor.insert(15,Qt::red);
    mapColor.insert(16,Qt::yellow);
    mapColor.insert(17,Qt::green);
    mapColor.insert(18,Qt::blue);
    mapColor.insert(19,Qt::magenta);
    mapColor.insert(714,Qt::red);
    mapColor.insert(177,Qt::red);

    qDebug() << "mapColor" <<mapColor;

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(QColor(Qt::white)));
    //scene->setSceneRect(0,0,200,200);
    QPen mPen;
    mPen.setWidth(5);
    mPen.setColor(Qt::red);
// 기존에 있는 vector 를 남겨놔야 함 : delete 를 위해
    //            for(auto cur_item : rectItemList){
    //                delete cur_item;
    //            }
    //            rectItemList.clear();


    QVector<QGraphicsRectItem*> rectItemList;

    QGraphicsLineItem *zeroX = new QGraphicsLineItem;
    QGraphicsLineItem *zeroY = new QGraphicsLineItem;
    QTransform trans;
    trans.scale(1,-1);

    QVector<QList<float>> vecList;
    for (int i = beginX ; i < endX ; i++)
    {
        for (int j = beginY ; j < endY ; j++)
        {
            vecList.append(mapFile[i][j]);
        }
    }

    for (auto &data : vecList)
    {

        if (data.size() == 8)
        {
            QGraphicsRectItem *rectItem = new QGraphicsRectItem;
            rectItemList.push_back(rectItem);


            float x = 500*(data[2]);
            float y = 500*(data[3]);
            float w = 500*(data[4] - data[2]);
            float h = 500*(data[5] - data[3]);

            if (x<minX){minX = x;}
            if (x>maxX){maxX = x;}
            if (y<minY){minY = y;}
            if (y>maxY){maxY = y;}
            rectItem->setRect(x,y,w,h);
            rectItem->setBrush(QBrush(QColor(mapColor[int(data[0])])));
            rectItem->setOpacity(0.5);
            rectItem->setZValue(data[6]);

            rectItem->setTransform(trans);
            scene->addItem(rectItem);
        }
    }
    zeroX->setLine(0,0,maxX,0);
    zeroY->setLine(0,0,0,maxY);
    zeroX->setPen(QPen(QColor(Qt::red)));
    zeroY->setPen(QPen(QColor(Qt::red)));
    zeroX->setZValue(400);
    zeroY->setZValue(400);
    zeroY->setTransform(trans);
    scene->addItem(zeroX);
    scene->addItem(zeroY);
    ui->graphicsView->setScene(scene);
//    float dx = 100-(maxX-minX)/2;
//    float dy = 100-(maxY-minY)/2;



//    ui->graphicsView->horizontalScrollBar()->setValue(ui->graphicsView->horizontalScrollBar()->value()+dx);
//    ui->graphicsView->verticalScrollBar()->setValue(ui->graphicsView->verticalScrollBar()->value()+dy);
//    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


}




FormTop::~FormTop()
{
    delete ui;
}
