#ifndef DEVICEDIALOG_H
#define DEVICEDIALOG_H

#include <QDialog>

namespace Ui {
class DeviceDialog;
}

class DeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceDialog(QWidget *parent = 0);
    ~DeviceDialog();

public slots:
    void addToList(QString item);
    QString returnSelection();

private slots:
    void on_listWidget_doubleClicked();

    void on_pushButtonSelect_clicked();

private:
    Ui::DeviceDialog *ui;
    QString selection;
};

#endif // DEVICEDIALOG_H
