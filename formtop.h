#ifndef FORMTOP_H
#define FORMTOP_H

#include <QDialog>

namespace Ui {
class FormTop;
}

class FormTop : public QDialog
{
    Q_OBJECT

public:
    explicit FormTop(QWidget *parent = nullptr);
    ~FormTop();
    void receiveFile(QVector<QVector<QVector<QList<float>>>>& mapFile);
//    void receiveFile(QVector<QList<float>> **mapFile);

private:
    Ui::FormTop *ui;
};

#endif // FORMTOP_H
