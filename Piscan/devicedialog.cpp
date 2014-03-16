#include "devicedialog.h"
#include "ui_devicedialog.h"

#include <QDebug>

DeviceDialog::DeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceDialog)
{
    ui->setupUi(this);
}

DeviceDialog::~DeviceDialog()
{
    delete ui;
}

void DeviceDialog::addToList(QString item)
{
    ui->listWidget->addItem(item);
}

void DeviceDialog::on_listWidget_doubleClicked()
{
    selection = ui->listWidget->currentItem()->text();
    //qDebug() << selection;
    this->accept();
}


QString DeviceDialog::returnSelection()
{
    return selection;
}

void DeviceDialog::on_pushButtonSelect_clicked()
{
    selection = ui->listWidget->currentItem()->text();
}
