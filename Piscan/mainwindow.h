/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <string>
#include "scandraid.h"


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
