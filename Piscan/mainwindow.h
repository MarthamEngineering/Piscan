#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <string>
#include "ScanDraiD.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QVariant getSetting(QString setting);
    
private slots:
    void on_pushButtonResetSettings_clicked();

    void on_pushButtonSaveSettings_clicked();

    void on_pushButtonProcessScan_clicked();



public slots:
    void updateStatusBar(QString text, int timeOut);

private:
    Ui::MainWindow *ui;
    void processConfigFile(const std::string& confFile, std::map<std::string,std::string>& paramMap);
    void setScannerParams(scanDraiD::ScanDraiD& scanner, const std::map<std::string,std::string>& paramMap);
    void setSettings(bool defaultSettings, QVariant CAMERA_HFOV, QVariant CAMERA_VFOV, QVariant CAMERA_DISTANCE, QVariant LASER_OFFSET, QVariant HORIZ_AVG, QVariant VERT_AVG, QVariant FRAME_SKIP, QVariant LINE_SKIP);
    void loadSettings();
};

#endif // MAINWINDOW_H
