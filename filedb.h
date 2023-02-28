#ifndef FILEDB_H
#define FILEDB_H

#include <QVector>


class FileDb
{
public:
    FileDb();
    void openFile(QString fileNameInfo,QVector<QList<float>> &vecList,
                  float &xMinSize,float &yMinSize,float &xMaxSize,float &yMaxSize);
};

#endif // FILEDB_H
