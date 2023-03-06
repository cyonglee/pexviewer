#include "formlayer.h"
#include "ui_formlayer.h"



FormLayer::FormLayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormLayer)
{
    ui->setupUi(this);
    QStringList defalutTableHeader;
    defalutTableHeader << "" << "color" << "LayerName" << "LayerNum" << "LayerType" << "Opacity";

    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnWidth(0,15);
//    ui->tableWidget->setColumnWidth(1,80);
//    ui->tableWidget->setColumnWidth(2,80);
    ui->tableWidget->setHorizontalHeaderLabels(defalutTableHeader);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#404040;color:#FFFFFF;}");

///////////////////////////////tableWidget Header 에 checkbox 넣고 위치 조정///////////////////////////////
    QHeaderView *horizontalHeader = ui->tableWidget->horizontalHeader();
    QHBoxLayout *layoutCheckBox = new QHBoxLayout(horizontalHeader);
    QCheckBox *HeaderCheckBox = new QCheckBox();
    HeaderCheckBox->setCheckState(Qt::Checked);
    layoutCheckBox->addWidget(HeaderCheckBox);
    layoutCheckBox->setContentsMargins(11,0,0,0);
    ui->tableWidget->setHorizontalHeader(horizontalHeader);

    QObject::connect(HeaderCheckBox, SIGNAL(stateChanged(int)), this, SLOT(tableWidget_checkBoxChanged()));

////////////////////////////////////////////////////////////////////////////////////////////////////////


}

FormLayer::~FormLayer()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief FormLayer::ReceiveSplitData
/// \param row
/// \param column
/// \param inputDataVector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FormLayer::ReceiveSplitData(int row, int column, const QVector <QVector <QString>> &inputDataVector)
{
// QTableWidget 에서 QStringList 로만 받기때문에 자료형변환
    QStringList vectorTOqstringlist;
    QStringList vectorTOqstringlistHoriLabels;
    QStringList vectorTOqstringcolorlist;
    QString LayerColorRed;
    QString LayerColorGreen;
    QString LayerColorBlue;
    QString LayerColorText;
    vectorTOqstringlistHoriLabels << " ";

// QTableWidget 크기 설정
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(row-1);
//    ui->tableWidget->setItemDelegateForColumn(5,new SliderDelegate);
    ui->tableWidget->setColumnWidth(5,150);


// Table Header Font size/bold change
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    font.setPointSize(10);
    ui->tableWidget->horizontalHeader()->setFont(font);

    //QSlider *slider = new QSlider(Qt::Horizontal);
    //slider->setRange(0, 100);
    //ui->tableWidget->setCellWidget(1, 5, slider);


// Table 채우기
    for (int i=0; i<row ; i++)
    {
        //Opacity 채우기
        QSlider *OpacitySlider = new QSlider(Qt::Horizontal);
        OpacitySlider->setRange(0,100);
        OpacitySlider->setFocusPolicy(Qt::StrongFocus);
//        OpacitySlider->setTickPosition(QSlider::TicksBothSides);
//        OpacitySlider->setTickInterval(10);
        ui->tableWidget->setCellWidget(i, 5, OpacitySlider);
        QObject::connect(OpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(handleOpacitySlider(int)));

        // table value 채우기
        for (int j=0; j<(column-4) ; j++)
        {
            vectorTOqstringlist << inputDataVector.value(i+1).value(j);
//            vectorTOqstringlistHoriLabels << inputDataVector.value(0).value(j+1);
            QTableWidgetItem *tableitem = new QTableWidgetItem;
            tableitem->setText(vectorTOqstringlist[i*(column-4)+j]);
            tableitem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i,j+2,tableitem);
        }

        // checkbox 채우기
        QCheckBox *checkBoxItem = new QCheckBox();
        checkBoxItem->setCheckState(Qt::Checked);
        QWidget *checkboxWidget = new QWidget();
        QHBoxLayout *checkboxLayout = new QHBoxLayout(checkboxWidget);
        checkboxLayout->addWidget(checkBoxItem);
        checkboxLayout->setAlignment(Qt::AlignCenter);
        checkboxLayout->setContentsMargins(0,0,0,0);
        checkboxWidget->setLayout(checkboxLayout);
        ui->tableWidget->setCellWidget(i,0,checkboxWidget);

        QWidget *widget = ui->tableWidget->cellWidget(i, 0);
        QCheckBox *checkboxInTable = widget->findChild<QCheckBox *>();
        QObject::connect(checkboxInTable, SIGNAL(stateChanged(int)), this, SLOT(tableWidget_checkBoxChanged()));

        // Layer color rgb value
        LayerColorRed = inputDataVector.value(i+1).value(3);
        LayerColorGreen = inputDataVector.value(i+1).value(4);
        LayerColorBlue = inputDataVector.value(i+1).value(5);

        LayerColorText = "background-color: rgb(" + LayerColorRed;
        LayerColorText = LayerColorText + "," + LayerColorGreen + "," + LayerColorBlue + ")";

        // color pushbutton 채우기
        QPushButton *colorbutton = new QPushButton();
        colorbutton->setStyleSheet(LayerColorText);
        ui->tableWidget->setCellWidget(i,1,colorbutton);
        QObject::connect(colorbutton, SIGNAL(clicked()), this, SLOT(on_colorbutton_clicked()));
    }
    ui->tableWidget->setHorizontalHeaderLabels(vectorTOqstringlistHoriLabels);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setColumnWidth(0,15);



    // Opacity 추가
//    QSlider *OpacitySlider = new QSlider(Qt::Horizontal);
//    OpacitySlider->setRange(0,100);
//    OpacitySlider->setGeometry(option.rect);
//    QWidget *OpacitySliderWidget = new QWidget();
//    QHBoxLayout *OpacitySliderLayout = new QHBoxLayout(OpacitySliderWidget);
//    OpacitySliderLayout->addWidget(OpacitySlider);
//    OpacitySliderWidget->setLayout(OpacitySliderLayout);

//    ui->tableWidget->setCellWidget(1,6,OpacitySliderWidget);
//    ui->tableWidget->setItemDelegate()


}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief FormLayer::on_tableWidget_itemChanged
/// \param checkBoxItem
/// description : checkbox 선택에 따라 Layer On/Off mainwindow status 창에 출력
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FormLayer::tableWidget_checkBoxChanged()
{
    QCheckBox *checkboxInTable = qobject_cast<QCheckBox*>(sender());
    if (!checkboxInTable)
        return;

    int checkboxrow = -1;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QWidget *widget = ui->tableWidget->cellWidget(i, 0);
        QCheckBox *checkbox = widget->findChild<QCheckBox*>();
        if (checkbox == checkboxInTable) {
            checkboxrow = i;
            break;
        }
    }

    if (checkboxrow == -1) {
        QString CommentAllCheckedCheckBox = "All Layer Checked";
        QString CommentAllUncheckedCheckBox = "All Layer Unchecked";

        if (checkboxInTable->isChecked()) {
            for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
                QWidget *widget = ui->tableWidget->cellWidget(i, 0);
                QCheckBox *checkbox = widget->findChild<QCheckBox*>();
                checkbox->setCheckState(Qt::Checked);
            }
            qDebug() << "All Layer Checkbox Checked.";
            emit outputLayerStatus(CommentAllCheckedCheckBox);

        } else {
                for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
                    QWidget *widget = ui->tableWidget->cellWidget(i, 0);
                    QCheckBox *checkbox = widget->findChild<QCheckBox*>();
                    checkbox->setCheckState(Qt::Unchecked);
                }
                qDebug() << "All Layer Checkbox Unchecked.";
                emit outputLayerStatus(CommentAllUncheckedCheckBox);
            }
        return;
    }

    QTableWidgetItem *readcheckbox = ui->tableWidget->item(checkboxrow,2);
    QString selectedLayer = readcheckbox->text();
    QString printLayer = "selectedLayer = " + selectedLayer;



    if (checkboxInTable->isChecked()) {
        printLayer = printLayer + "  Layer  On";
        emit outputLayerStatus(printLayer);
        qDebug() << "Checkbox at row" << checkboxrow << "is checked.";
    } else {
        printLayer = printLayer + "  Layer  Off";
        emit outputLayerStatus(printLayer);
        qDebug() << "Checkbox at row" << checkboxrow << "is unchecked.";
    }
}

void FormLayer::on_colorbutton_clicked()
{
    QString LayerColorname;
    QString LayerColorText;
    int colorbuttonRow = ui->tableWidget->currentRow();

    QColor color = QColorDialog::getColor(Qt::yellow, this);

    LayerColorname = color.name();
    LayerColorText = "background-color: " + LayerColorname;

//    qDebug() << ui->tableWidget->cellWidget(colorbuttonRow,1);

    ui->tableWidget->cellWidget(colorbuttonRow,1)->setStyleSheet(LayerColorText);
}

void FormLayer::handleOpacitySlider(int Opacity)
{

    int temprow=0;
    QSlider *OpacityInTalbe = qobject_cast<QSlider*>(sender());
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QWidget *widget = ui->tableWidget->cellWidget(i, 0);
        QSlider *Opacityslider = widget->findChild<QSlider*>();
        if (Opacityslider == OpacityInTalbe) {
            temprow = i;
            Opacityslider->setValue(Opacity);
            qDebug() << "Opacity = " << Opacity;
            break;
        }
    }

    //        slider->setValue(value);
    //        qDebug() << slider;
}


