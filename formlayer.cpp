#include "formlayer.h"
#include "ui_formlayer.h"



FormLayer::FormLayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormLayer)
{
    ui->setupUi(this);
    QStringList defalutTableHeader;
    defalutTableHeader << "" << "color" << "LayerName" << "LayerNum" << "LayerType";

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnWidth(0,15);
//    ui->tableWidget->setColumnWidth(1,80);
//    ui->tableWidget->setColumnWidth(2,80);
    ui->tableWidget->setHorizontalHeaderLabels(defalutTableHeader);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#404040;color:#FFFFFF;}");


//    QHeaderView *horizontalHeader = ui->tableWidget->horizontalHeader();
//    QWidget *checkBoxWidget = new QWidget();
//    QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
//    QCheckBox *checkBox = new QCheckBox();

//    layoutCheckBox->addWidget(checkBox);
//    layoutCheckBox->setAlignment(Qt::AlignCenter);
//    layoutCheckBox->setContentsMargins(0,0,0,0);
//    checkBoxWidget->setLayout(layoutCheckBox);
//    QModelIndex checkboxIndex = ui->tableWidget->model()->index(0,0,QModelIndex());

//    horizontalHeader->setIndexWidget(checkboxIndex, checkBoxWidget);


    QHeaderView *horizontalHeader = ui->tableWidget->horizontalHeader();
    QHBoxLayout *layoutCheckBox = new QHBoxLayout(horizontalHeader);
    QCheckBox *HeaderCheckBox = new QCheckBox();
    HeaderCheckBox->setCheckState(Qt::Checked);
    layoutCheckBox->addWidget(HeaderCheckBox);
    layoutCheckBox->setContentsMargins(11,0,0,0);
    ui->tableWidget->setHorizontalHeader(horizontalHeader);

    QObject::connect(HeaderCheckBox, SIGNAL(stateChanged(int)), this, SLOT(tableWidget_checkBoxChanged()));

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
    ui->tableWidget->setColumnCount(column-2);
    ui->tableWidget->setRowCount(row-1);

// Table Header Font size/bold change
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    font.setPointSize(10);
    ui->tableWidget->horizontalHeader()->setFont(font);

// Table 채우기
    for (int i=0; i<row ; i++)
    {
        // table value 채우기
        for (int j=0; j<(column-4) ; j++)
        {
            vectorTOqstringlist << inputDataVector.value(i+1).value(j);
//            vectorTOqstringlistHoriLabels << inputDataVector.value(0).value(j+1);
            ui->tableWidget->setItem(i,j+2,new QTableWidgetItem(vectorTOqstringlist[i*(column-4)+j]));
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

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief FormLayer::on_tableWidget_itemChanged
/// \param checkBoxItem
/// description : checkbox 선택에 따라 Layer On/Off mainwindow status 창에 출력
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FormLayer::tableWidget_checkBoxChanged()
{
    qDebug() << "xxxxxxxxxxxxxxxxxxxxxxxzzzzzzzzzzzzzzzzzzzzz";
    //ui->tableWidget->connect
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
        qDebug() << "Failed to find row and column for checkbox.";

        if (checkboxInTable->isChecked()) {
            for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
                QWidget *widget = ui->tableWidget->cellWidget(i, 0);
                QCheckBox *checkbox = widget->findChild<QCheckBox*>();
                checkbox->setCheckState(Qt::Checked);
            }
        } else {
                for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
                    QWidget *widget = ui->tableWidget->cellWidget(i, 0);
                    QCheckBox *checkbox = widget->findChild<QCheckBox*>();
                    checkbox->setCheckState(Qt::Unchecked);
                }
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


//void FormLayer::on_tableWidget_itemChanged()
//{
//    int clickedRow = ui->tableWidget->currentRow();
//    qDebug() << clickedRow;
//    QTableWidgetItem *item = ui->tableWidget->item(clickedRow,2);

//    if (item != nullptr) {
//        QString selectedLayer = item->text();
//        QString printLayer = "selectedLayer = " + selectedLayer;

//        bool LayerOnOff = ui->tableWidget->cellWidget(clickedRow,0)->windowState();
//        qDebug() << LayerOnOff;
//        if (LayerOnOff==true){
//            printLayer = printLayer + "  Layer  On";
//            emit outputLayerStatus(printLayer);
//            qDebug() << printLayer;
//        } else {
//            printLayer = printLayer + "  Layer  Off";
//            emit outputLayerStatus(printLayer);
//            qDebug() <<  printLayer;
//        }

//    } else {
//        qDebug() << "No item";
//    }
//}

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
