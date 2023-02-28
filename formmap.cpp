#include "formmap.h"
#include "ui_formmap.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QVector>
#include <iostream>
#include <QDebug>
#include <QRect>
#include <QKeyEvent>

FormMap::FormMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormMap)
{
    ui->setupUi(this);

}

void FormMap::receiveSize(float &xMinSize,float &yMinSize,float &xMaxSize,float &yMaxSize, float &zoomScale)
{
    float scale=1;

    width = xMaxSize-xMinSize;
    height = yMaxSize-yMinSize;
    qDebug() << "scale : " << scale << "| width : " << width << "| height : " << height;
    scale = 178/std::max(width,height);
    width = scale * width;
    height = scale * height;
//    float rectScale = 176*zoomScale; 추후 변경

    qDebug() << "scale : " << scale << "| width : " << width << "| height : " << height;

    QGraphicsRectItem *rectItem = new QGraphicsRectItem;
    QGraphicsScene *scene = new QGraphicsScene(this);
    rectItem->setRect(-(width/2),-(height/2),width,height);
    rectItem->setBrush(QBrush(QColor(Qt::gray)));
    scene->addItem(rectItem);

    SuperItem *super = new SuperItem;
    scene->addItem(super);
    QObject::connect(this,&FormMap::signalMove,super,&SuperItem::slotMove);

    ui->graphicsView->setScene(scene);

}

void FormMap::slotInfoText(QString funcName, float value)
{

    if (funcName == "moveGdsX")
    {
        pointX = pointX + value;
        initPointX = pointX;
        emit signalMove(value,0,0,999,999);
    }
    if (funcName == "moveGdsY")
    {
        pointY = pointY + value;
        initPointY = pointY;
        emit signalMove(0, value,0,999,999);
    }
    if (funcName == "rotateRenderX")
    {
        infoRot = initRot + value;
        if (infoRot>360)
            infoRot = infoRot - 360;
        if (infoRot<0)
            infoRot = infoRot + 360;
        emit signalMove(0, 0 ,0,infoRot,999);
    }
    if (funcName == "rotateRenderY")
    {
        infoTilt = initTilt + value;
        if (infoTilt>90)
            infoTilt = 90;
        if (infoTilt<-90)
            infoTilt = -90;
        emit signalMove(0, 0 ,0,999,infoTilt);
    }
    if (funcName == "moveZoom")
    {
        infoZoom = infoZoom + infoZoom*value/1500;        //일정 높이를 100% 로 설정할 필요 있음, 결국 초기값은 GDS Size 에 따라 바뀔 수 있음.
        if (infoZoom < 0.01)
            infoZoom = 0.01;
        if (infoZoom > 100)
            infoZoom = 100;
        emit signalMove(0, 0, infoZoom, 999, 999);
    }
    if (funcName == "moveRenderX")
    {
        x = value;
    }
    if (funcName == "moveRenderY")
    {
        y = value;
    }
    if (funcName == "mouseRelease")
    {
        initTilt = infoTilt;
        initRot = infoRot;
        initPointX = pointX;
        initPointY = pointY;
        initPointZ = pointZ;
    }
}

FormMap::~FormMap()
{
    delete ui;
}

SuperItem::SuperItem(QGraphicsItem* parent) : QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
}



QRectF SuperItem::boundingRect() const{
    return QRectF(-88,-88,175,175);
}

void SuperItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::red);
    painter->drawRoundedRect(-88,-88,175,175,5,5);


}

void SuperItem::slotMove(float x, float y, float zoom, float rot, float tilt)
{
    QTransform trans;

    if (zoom != 0)
    {
        _zoom = zoom;
    }
    if (rot != 999)
    {
        _rot = rot;
    }
    if (tilt != 999)
    {
        _tilt = tilt;
    }
    trans.rotate(_rot);
    trans.scale(_zoom, _zoom*_tilt/90);
    setTransform(trans);

    if (x + y !=0)
    {
        moveBy(x, -y);
        qDebug() << "pos : " << pos();
    }






}

//void SuperItem::keyPressEvent(QKeyEvent *event){
//    switch(event->key()){
//    case Qt::Key_D:{
//        moveBy(30,0);
//        qDebug() << "press D";
//        break;
//        }
//    case Qt::Key_A:{
//        moveBy(-30,0);
//        break;
//        }
//    case Qt::Key_W:{
//        moveBy(0,-30);
//        break;
//        }
//    case Qt::Key_S:{
//        moveBy(0,30);
//        break;
//        }
//    }
//    update();

//}
