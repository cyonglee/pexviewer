#include "filedb.h"
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <QList>
#include <QDebug>

FileDb::FileDb()
{

}

QVector<QVector<QVector<QList<float>>>> FileDb::openFile(QString fileNameInfo)
{
    qDebug() << "fileDb.h : " <<fileNameInfo;
    QFile file(fileNameInfo);

    float minX = 0, minY = 0, maxX = 0, maxY =0;
    QVector<QList<float>> strVector;

    if (file.open(QIODevice::ReadOnly) | (QIODevice::Text))
    {
        QTextStream openFile(&file);
        while(!openFile.atEnd())
        {
            auto dataString = openFile.readLine().split(",");
            QList<float> data;
            for (auto &tempdata : dataString)
            {
                 data.append(tempdata.toFloat());
            }
            strVector.append(data);
            qDebug()<< "strVector : " << data;
//            if(minX > data[2].toFloat()){minX = data[2].toFloat();}
//            if(maxX < data[4].toFloat()){maxX = data[4].toFloat();}
//            if(minY > data[3].toFloat()){minY = data[3].toFloat();}
//            if(maxY < data[5].toFloat()){maxY = data[5].toFloat();}
            if(data.size() == 8)
            {
                if(minX > data[2]){minX = data[2];}
                if(maxX < data[4]){maxX = data[4];}
                if(minY > data[3]){minY = data[3];}
                if(maxY < data[5]){maxY = data[5];}
            }
        }
        file.close();
    }
    else
    {
        QVector<QVector<QVector<QList<float>>>> mapFile{{{}}};
        return mapFile;
    }
    int n = (int(maxX*1000)-int(minX*1000))/10 +1;
    int m = (int(maxY*1000)-int(minY*1000))/10 +1;
    qDebug() << "int n / int m : " << n << " , " <<m;

    QVector<QVector<QVector<QList<float>>>> mapFile(n, QVector<QVector<QList<float>>>(m, {{}}));
    mapFile[0][0][0].append({minX, minY, maxX, maxY});

    for (auto & data : strVector)
    {
        if(data.size() == 8)
        {
            qDebug() << " " << data[2] << " "<< int(data[2]*100) << " "<< data[3] << " "<< int(data[3]*100);
            int in_n = 0, in_m = 0;
            if(int(data[2]*100)<0)
            {in_n = n + int(data[2]*100)-1;}
            else
            {in_n = int(data[2]*100)+1;}
            if(int(data[3]*100)<0)
            {in_m = m + int(data[3]*100)-1;}
            else
            {in_m = int(data[3]*100)+1;}
            mapFile[in_n][in_m].append(data);
        }
    }
    return mapFile;

}
