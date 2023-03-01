#ifndef FORMLAYER_H
#define FORMLAYER_H

#include <QDialog>
#include <QColorDialog>
#include <QtWidgets/QCheckBox>
#include <QtWidgets>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QPushButton>
#include <QtWidgets/QSlider>
#include <QDebug>


namespace Ui {
class FormLayer;
}

class FormLayer : public QDialog
{
    Q_OBJECT

public slots:
    void ReceiveSplitData(int row, int column, const QVector <QVector <QString>> &inputDataVector);

public:
    explicit FormLayer(QWidget *parent = nullptr);
    ~FormLayer();
    int i;
    int row;
    int column;

//    QTableWidgetItem *checkBoxItem[3];


private slots:
    void tableWidget_checkBoxChanged();
    void on_colorbutton_clicked();

private:
    Ui::FormLayer *ui;

signals:
    void outputLayerStatus(QString printLayer);

};

#endif // FORMLAYER_H
