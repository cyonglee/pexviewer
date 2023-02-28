#include "filedb.h"
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <QList>
#include <QDebug>

FileDb::FileDb()
{

}

void FileDb::openFile(QString fileNameInfo,QVector<QList<float>> &vecList,
                      float &xMinSize,float &yMinSize,float &xMaxSize,float &yMaxSize)
{    
    QFile file(fileNameInfo);

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
            vecList.push_back(data);
            qDebug()<< "strVector : " << data;

            if(data.size() == 8)
            {
                if(xMinSize > data[2]){xMinSize = data[2];}
                if(yMinSize > data[3]){yMinSize = data[3];}
                if(xMaxSize < data[4]){xMaxSize = data[4];}
                if(yMaxSize < data[5]){yMaxSize = data[5];}
            }
        }
        file.close();
    }
    else
    {        
        return;
    }

    return;

}
