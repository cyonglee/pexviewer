#ifndef FILEDB_H
#define FILEDB_H

#include <QVector>


class FileDb
{
public:
    FileDb();
    QVector<QVector<QVector<QList<float>>>> openFile(QString fileNameInfo);
};

#endif // FILEDB_H
