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
    void handleOpacitySlider(int);

private:
    Ui::FormLayer *ui;

signals:
    void outputLayerStatus(QString printLayer);

};

//class SliderDelegate : public QItemDelegate
//{
//public:
//    SliderDelegate(QWidget *parent = nullptr) : QItemDelegate(parent){
////        SliderDelegate delegate(this);
//    }
//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override
//    {
//        QSlider *slider = new QSlider(Qt::Horizontal, parent);
//        slider->setRange(0, 100);
//        slider->setGeometry(option.rect);
//        slider->setFocusPolicy(Qt::StrongFocus);
//        slider->setTickPosition(QSlider::TicksBothSides);
//        slider->setTickInterval(10);
//        slider->show();
//        return slider;
//    }

//    void setEditorData(QWidget *editor, const QModelIndex &index) const override
//    {
//        int value = index.model()->data(index, Qt::EditRole).toInt();
//        QSlider *slider = static_cast<QSlider*>(editor);
//        slider->setValue(value);
//        qDebug() << slider;
//    }

//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//                      const QModelIndex &index) const override
//    {
//        QSlider *slider = static_cast<QSlider*>(editor);
//        int value = slider->value();
//        model->setData(index, value, Qt::EditRole);
//    }
//};


#endif // FORMLAYER_H
