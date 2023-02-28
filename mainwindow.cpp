#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QDockWidget>
#include <QGraphicsRectItem>
#include <QGraphicsOpacityEffect>
#include <QFile>
#include <QFileDialog>
#include <QString>

#include <fstream>
#include <iostream>
#include <string>
#include <QIODevice>
#include <QTextStream>

MainWindow::MainWindow(VulkanWindow *w)
    : m_window(w)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *wrapper = QWidget::createWindowContainer(w);
    ui->graphicsView->setViewport(wrapper);

    QDockWidget *dockHier = new QDockWidget(tr("Hierarchy"), this);
    dockHier->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockHier->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::RightDockWidgetArea, dockHier);
    formHier = new FormHier;
    dockHier->setWidget(formHier);

    QDockWidget *dockLayer = new QDockWidget(tr("Layer Information"), this);
    dockLayer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockLayer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::RightDockWidgetArea, dockLayer);
    formLayer = new FormLayer;
    dockLayer->setWidget(formLayer);

    QDockWidget *dockMap = new QDockWidget(tr("Map"), this);
    dockMap->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockMap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::LeftDockWidgetArea, dockMap);
    formMap = new FormMap;    
    dockMap->setWidget(formMap);

    QDockWidget *dockTop = new QDockWidget(tr("Topview"), this);
    dockTop->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockTop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::LeftDockWidgetArea, dockTop);
    formTop = new FormTop;    
    dockTop->setWidget(formTop);

    QDockWidget *dockInfo = new QDockWidget(tr("Info"), this);
    dockInfo->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::LeftDockWidgetArea, dockInfo);
    formInfo = new FormInfo;
    dockInfo->setWidget(formInfo);

    resizeDocks({dockInfo, dockTop, dockMap, dockLayer, dockHier}, {200,200}, Qt::Horizontal);
    resizeDocks({dockTop, dockMap}, {200,200}, Qt::Vertical);

    input_dataS = new all_data;


    /// connect ////////////////////
    QObject::connect(ui->actionOpen_file, SIGNAL(triggered()), this, SLOT(on_actionOpen_file_triggered));
    QObject::connect(this, SIGNAL(sendSelectFileName(QString)), input_dataS, SLOT(receiveSelectFileName(QString)));
    QObject::connect(input_dataS, SIGNAL(sendSplitData(int, int, const QVector <QVector <QString>> &)), formHier, SLOT(ReceiveSplitData(int, int, const QVector <QVector <QString>> &)));
    QObject::connect(input_dataS, SIGNAL(sendSplitData(int, int, const QVector <QVector <QString>> &)), formLayer, SLOT(ReceiveSplitData(int, int, const QVector <QVector <QString>> &)));
    QObject::connect(formLayer, SIGNAL(outputLayerStatus(QString)), this, SLOT(inputLayerStatus(QString)));

}

void MainWindow::shareGeo(QRect size)
{
    QRect windowSize = size;
    qDebug() << "shareGeo : " << windowSize;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotInfoText(QString funcName, float value)
{
    formInfo->slotInfoText(funcName,value);
    formMap->slotInfoText(funcName,value);
    qDebug() << "slotInfoText" << value;
///// temp //////
    QString text = funcName + " : " + QString::number(value);
    for (int i = text.size() ; i < 30 ; i++)
    {
        text.append(" ");
    }

    statusText.prepend(text);

    if (statusText.size() > 120)
        statusText.remove(120,statusText.size()-120);
    ui->statusbar->showMessage(statusText);

}


QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
    m_renderer = new VulkanRenderer(this);
    return m_renderer;
}

VulkanRenderer::VulkanRenderer(VulkanWindow *w)
    : TriangleRenderer(w)
{
}

void VulkanWindow::wheelEvent(QWheelEvent *e)
{
    //문제가 하나 있는데...
    //프로그램이 선택되어 있지 않아도 moveZoom 명령이 나감
    //근데 Ctrl 은 Vulkan Window 가 선택되었을 때만 됨
    //moveZoom 을 이 Level 까지 끌고 내려와야 함
    const float amount = e->angleDelta().y() / 8;

    if (keyCtrl == true)
    {
//      m_rendere->windowZoom(amount);
        QString funcName = "windowZoom";
        float value = amount;
        emit signalInfoText(funcName, value);
//        qDebug()<<funcName << " : " << value;
    }
    else
    {
//      m_renderer->moveZoom(amount);
        QString funcName = "moveZoom";
        float value = amount;
        emit signalInfoText(funcName, value);
//        qDebug()<<funcName << " : " << value;
    }
}

void VulkanWindow::mousePressEvent(QMouseEvent *e)
{
    m_mouseButton = e->buttons();
    m_lastPos = e->pos();
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_mouseButton = 0;
    QString funcName = "mouseRelease";
    float value = 0;
    emit signalInfoText(funcName, value);
}

void VulkanWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (m_mouseButton == 0)
        return;

    int dx = e->pos().x() - m_lastPos.x();
    int dy = e->pos().y() - m_lastPos.y();

    if (dx)
    {
        if (m_mouseButton == 2)
        {
//            m_renderer->rotateRenderY(dx / 10.0f);
            QString funcName = "rotateRenderX";
            float value = -dx / 10.0f;
            emit signalInfoText(funcName, value);
            qDebug()<< "mouseEvent "<<funcName << " : " << value;
        }
        else if (m_mouseButton == 4)
        {
//            m_renderer->moveRenderX(dx / 10.0f);
            QString funcName = "moveRenderX";
            float value = -dx / 10.0f;
            emit signalInfoText(funcName, value);
//            qDebug()<<funcName << " : " << value;
        }
    }

    if (dy)
    {
        if (m_mouseButton == 2)
        {
//            m_renderer->rotateRenderY(dy / 10.0f);
            QString funcName = "rotateRenderY";
            float value = dy / 10.0f;
            emit signalInfoText(funcName, value);
//            qDebug()<<funcName << " : " << value;
        }
        else if (m_mouseButton == 4)
        {
//            m_renderer->moveRenderY(dy / 10.0f);
            QString funcName = "moveRenderY";
            float value = dy / 10.0f;
            emit signalInfoText(funcName, value);
//            qDebug()<<funcName << " : " << value;
        }
    }
}

void VulkanWindow::keyPressEvent(QKeyEvent *e)
{
    const float amount = e->modifiers().testFlag(Qt::ShiftModifier) ? 1.0f : 0.1f;
    QString funcName;
    float value = 0;
    switch (e->key()) {
    case Qt::Key_Up:
        funcName = "moveGdsY";
        value = amount;
//        m_renderer->moveGdsY(value);
        break;
    case Qt::Key_Down:
        funcName = "moveGdsY";
        value = -amount;
//        m_renderer->moveGdsY(value);
        break;
    case Qt::Key_Right:
        funcName = "moveGdsX";
        value = amount;
//        m_renderer->moveGdsX(value);
        break;
    case Qt::Key_Left:
        funcName = "moveGdsX";
        value = -amount;
//        m_renderer->moveGdsX(value);
        break;
    case Qt::Key_PageUp:
        funcName = "moveGdsZ";
        value = amount;
//        m_renderer->moveGdsZ(value);
        break;
    case Qt::Key_PageDown:
        funcName = "moveGdsZ";
        value = -amount;
//        m_renderer->moveGdsZ(value);
        break;
    case Qt::Key_Control:
        keyCtrl = true;
        qDebug() << "Control";
        break;
    case Qt::Key_Alt:
        keyAlt = true;
        qDebug() << "Alt";
        break;
    case Qt::Key_Shift:
        keyShift = true;
        qDebug() << "Shift";
        break;
    default:
        break;
    }
    emit signalInfoText(funcName, value);
    qDebug()<<funcName << " : " << value;
}
void VulkanWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Control:
        keyCtrl = false;
        qDebug() << "Control off";
        break;
    case Qt::Key_Alt:
        keyAlt = false;
        qDebug() << "Alt off";
        break;
    case Qt::Key_Shift:
        keyShift = false;
        qDebug() << "Shift off";
        break;
    default:
        break;
    }
}

void MainWindow::on_actionOpen_Map_File_triggered()
{
    QString fileNameInfo = QFileDialog::getOpenFileName(this,
                                                        tr("Open map file"),
                                                        "C:/",
                                                        tr("text (*.txt)")
                                                        );
    FileDb *fileDb = new FileDb;
    QVector<QList<float>> vecList;
    float xMinSize = 0, yMinSize = 0, xMaxSize = 0, yMaxSize = 0;
    fileDb->openFile(fileNameInfo, vecList , xMinSize, yMinSize, xMaxSize, yMaxSize);

    int posScale = 1000, splitSize = 10; //posScale 이 1이면 um 단위, posScale 이 1000이면 nm 단위
    int n = (int(xMaxSize *posScale)-int(xMinSize *posScale))/splitSize +1;
    int m = (int(yMaxSize *posScale)-int(yMinSize *posScale))/splitSize +1;
    qDebug() << "int n / int m : " << n << " , " <<m;

    ////////////////////////////////////////////////
    QVector<QVector<QVector<QList<float>>>> mapFile(n, QVector<QVector<QList<float>>>(m, {{}}));
//    QVector<QList<float>> mapFile[n][m];
    // 구조 변경에 대해 생각할 필요 있음
    // 주형 구조 : struct(layer, r, g, b, z, thk, opacity, vector())

    mapFile[0][0][0].append({0, 0, xMinSize, yMinSize, xMaxSize, yMaxSize, 0, 0});

    for (auto & data : vecList)
    {
        if(data.size() == 8)
        {
            qDebug() << " " << data[2] << " "<< int(data[2]*100) << " "<< data[3] << " "<< int(data[3]*100);
            int in_n = 0, in_m = 0;
            if(data[2]<0)
            {in_n = n + int(data[2]*100)-1;}
            else
            {in_n = int(data[2]*100);}
            if(data[3]<0)
            {in_m = m + int(data[3]*100)-1;}
            else
            {in_m = int(data[3]*100);}
            mapFile[in_n][in_m].append(data);
        }
    }



    for(int i = 0 ; i < (int(mapFile[0][0][0][2]*1000)-int(mapFile[0][0][0][0]*1000))/10+1 ; i++)
    {
        QDebug oneLine = qDebug();
        for(int j = 0 ; j < (int(mapFile[0][0][0][3]*1000)-int(mapFile[0][0][0][1]*1000))/10+1 ; j++)
        {
            oneLine << "["<<i<<"]["<<j<<"] "<<mapFile[i][j];
        }
        qDebug() << "";
    }

    float zoomScale = std::max(xMaxSize-xMinSize,yMaxSize-yMinSize)/10;
    formMap->receiveSize(xMinSize,yMinSize,xMaxSize,yMaxSize,zoomScale);
    formTop->receiveFile(mapFile);

}

void MainWindow::on_actionOpen_file_triggered()
{

    QString file_name = QFileDialog::getOpenFileName(this, "파일 선택","C:\\","Files(*.*)");
    //qDebug() << file_name;

    emit sendSelectFileName(file_name);

}


void MainWindow::inputLayerStatus(QString text)
{
    ui->statusbar->showMessage(text);
}
