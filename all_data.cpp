#include "all_data.h"
#include <QString>
#include <QTextStream>
#include <QFileDialog>
#include <QVector>

all_data::all_data(QObject *parent) :
    QObject(parent)
{

}
/////////////
all_data::~all_data()
{

}


void all_data::receiveSelectFileName(QString file_name)
{

//    qDebug() << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
//    qDebug() << file_name;

    int row = 0;
    int column = 0;
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString getLine;
    QStringList list;
    QTextStream fileStream(&file);
    QRegExp rx("[,]");


    while (!fileStream.atEnd()) {
            getLine.append(fileStream.readLine());
            getLine.append(",");
            list << (getLine.split(rx));
//            for (int column=0;column<17;column++)
//            {
//                split_data[row][column] << list[row][column];
//                //emit sendSplitData(split_data[row][column]);
//                //qDebug() << "row = " << row << split_data[row][column];
//            }
            row++;
//            column = list.count();
            getLine.clear();
    }   // while End

    column = list.count()/row;

//    qDebug() << list;


    for(int i=0 ; i<row ; i++)
    {
           QVector <QString> tempvector;
           for(int j=0 ; j<column ; j++)
           {
               int num1;
               num1 = j + column*i;
               tempvector.append(list[num1]);

//               qDebug() << "list " << num1 << list[num1];


           }
           inputDataVector.append(tempvector);
//           qDebug() << "inputDataVector " << inputDataVector;

    }
//    qDebug() << "1st" << inputDataVector.value(0);
//    qDebug() << "All" << inputDataVector;
//    qDebug() << "2nd" << inputDataVector.value(1);
//    qDebug() << "3rd" << inputDataVector.value(2);
//    qDebug() << "inputDataVector" << ": " << inputDataVector;

//    QVector <QVector <QVector <int> > > myVector;

//    int massive[4][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16} };

//    QVector <QVector <int> > matrix;

//    for(int i = 0; i < 4; i++)
//    {
//        QVector<int> tempVector;

//        for(int j = 0; j < 4; j++)
//        {
//            tempVector.push_back(massive[i][j]);
//            qDebug() << "Value " << j << ": " << tempVector.value(j);
//        }
//        matrix.push_back(tempVector);
//        qDebug() << "matrix row " << i << ": " << matrix.value(i);
//    }

//    myVector.push_back(matrix);

//    qDebug() << "myVector: " << myVector.value(0);




    emit sendSplitData(row, column, inputDataVector);


}
