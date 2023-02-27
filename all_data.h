#ifndef ALL_DATA_H
#define ALL_DATA_H

#include <QObject>
#include <QDebug>
#include <QVector>


class all_data : public QObject
{
    Q_OBJECT
//    Q_DECLARE_METATYPE(QVector <QVector <QString>>);

public:
    all_data(QObject *parent = nullptr);
    ~all_data();

public:
    QVector <QVector <QString>> inputDataVector;

public slots:
    void receiveSelectFileName(QString file_name);

signals:
    void sendSplitData(int row, int column, const QVector <QVector <QString>> &inputDataVector);


};

#endif // ALL_DATA_H
