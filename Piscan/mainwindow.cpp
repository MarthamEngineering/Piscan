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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QDebug>
#include <QFileDialog>
//#include <QDesktopServices>
#include <QStatusBar>

//ScanDraiD Includes
#include <stdlib.h>
#include <libintl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <map>
#include "scandraid.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Load settings
    QSettings settings("FOSS", "ScanDraiD");
    if (settings.contains("CAMERA_HFOV") == false){
        qDebug() << "Write Settings\n";
        setSettings(true, "", "", "", "", "", "", "","");

        QStringList keys = settings.childKeys();
        qDebug() << keys;


        // connect(sender, SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(Qbject*)));
    }

    loadSettings();

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setSettings(bool defaultSettings, QVariant CAMERA_HFOV, QVariant CAMERA_VFOV, QVariant CAMERA_DISTANCE, QVariant LASER_OFFSET, QVariant HORIZ_AVG, QVariant VERT_AVG, QVariant FRAME_SKIP, QVariant LINE_SKIP){

    //Do some checking here to make sure we are using the proper types for the settings.

    //initialise the settings configuation
     QSettings settings("FOSS", "ScanDraiD");

     if (defaultSettings == false){
         qDebug() << "Write New Settings\n";
         //settings.beginGroup("scanner");
         settings.setValue("CAMERA_HFOV", CAMERA_HFOV);
         settings.setValue("CAMERA_VFOV", CAMERA_VFOV);
         settings.setValue("CAMERA_DISTANCE", CAMERA_DISTANCE);
         settings.setValue("LASER_OFFSET", LASER_OFFSET);
         settings.setValue("HORIZ_AVG", HORIZ_AVG);
         settings.setValue("VERT_AVG", VERT_AVG);
         settings.setValue("FRAME_SKIP", FRAME_SKIP);
         settings.setValue("LINE_SKIP", LINE_SKIP);
         //settings.endGroup();
     }else{
        qDebug() << "Clear Settings\n";

        settings.clear();
        loadSettings();
    }

     //settings.sync();

}

QVariant MainWindow::getSetting(QString setting){

    QSettings settings("FOSS", "ScanDraiD");
    QString returnSetting = "";

    if (setting == "CAMERA_HFOV"){
        returnSetting = settings.value("CAMERA_HFOV", 50).toString();
    }else if (setting == "CAMERA_VFOV") {
        returnSetting = settings.value("CAMERA_VFOV", 50*4.0/5.0).toString();
    }else if (setting == "CAMERA_DISTANCE") {
        returnSetting =  settings.value("CAMERA_DISTANCE", 265).toString();
    }else if (setting == "LASER_OFFSET") {
        returnSetting = settings.value("LASER_OFFSET", 15).toString();
    }else if (setting == "HORIZ_AVG") {
        returnSetting = settings.value("HORIZ_AVG", 10).toString();
    }else if (setting == "VERT_AVG") {
        returnSetting = settings.value("VERT_AVG", 10).toString();
    }else if (setting == "FRAME_SKIP") {
        returnSetting = settings.value("FRAME_SKIP", 1).toString();
    }else if (setting == "LINE_SKIP") {
        returnSetting = settings.value("LINE_SKIP", 1).toString();
    }

    return returnSetting;

}

void MainWindow::on_pushButtonResetSettings_clicked()
{
    setSettings(true, "", "", "", "", "", "", "","");
}

void MainWindow::on_pushButtonSaveSettings_clicked()
{
    QVariant CAMERA_HFOV = ui->lineEditHFOV->text();
    QVariant CAMERA_VFOV = ui->lineEditVFOV->text();
    QVariant CAMERA_DISTANCE = ui->lineEditDistance->text();
    QVariant LASER_OFFSET = ui->lineEditLaserOffset->text();
    QVariant HORIZ_AVG = ui->lineEditHorizontalAverage->text();
    QVariant VERT_AVG = ui->lineEditVerticalAverage->text();
    QVariant FRAME_SKIP = ui->lineEditFrameSkip->text();
    QVariant LINE_SKIP = ui->lineEditLineSkip->text();

    setSettings(false, CAMERA_HFOV, CAMERA_VFOV, CAMERA_DISTANCE, LASER_OFFSET, HORIZ_AVG, VERT_AVG, FRAME_SKIP, LINE_SKIP);
}

void MainWindow::loadSettings(){

    ui->lineEditHFOV->setText(getSetting("CAMERA_HFOV").toString());
    ui->lineEditVFOV->setText(getSetting("CAMERA_VFOV").toString());
    ui->lineEditDistance->setText(getSetting("CAMERA_DISTANCE").toString());
    ui->lineEditLaserOffset->setText(getSetting("LASER_OFFSET").toString());
    ui->lineEditHorizontalAverage->setText(getSetting("HORIZ_AVG").toString());
    ui->lineEditVerticalAverage->setText(getSetting("VERT_AVG").toString());
    ui->lineEditFrameSkip->setText(getSetting("FRAME_SKIP").toString());
    ui->lineEditLineSkip->setText(getSetting("LINE_SKIP").toString());

    updateStatusBar("Settings Loaded", 3000);

}

void MainWindow::on_pushButtonProcessScan_clicked()
{

    //TO DO - Use native dialog once bug is resolved with multiple windows and the file browser not closing

    QString inDir = QFileDialog::getExistingDirectory(NULL, "Open Directory",QDir::homePath(),QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);

    if (inDir != ""){

    std::string path2frames = inDir.toStdString();
    //qDebug() << "Path to image frames: " << inDir << "\n";
    //std::string fileType = "PLY";

    scanDraiD::ScanDraiD scanner;

    // Make a connection to allow the ScanDraiD scanner object return data.
    connect(&scanner, SIGNAL(percentageComplete(QString, int)), this, SLOT(updateStatusBar(QString,int)));
    connect(&scanner, SIGNAL(addPointScanView(float,float,float)), ui->GLScanWidgetui, SLOT(drawPoint(float,float,float)));

    scanner.setCameraHFov(getSetting("CAMERA_HFOV").toFloat(), true);
    scanner.setCameraVFov(getSetting("CAMERA_VFOV").toFloat());
    scanner.setCameraDistance(getSetting("CAMERA_DISTANCE").toFloat());
    scanner.setLaserOffset(getSetting("LASER_OFFSET").toFloat());
    scanner.setHorizontalAverage(getSetting("HORIZ_AVG").toInt());
    scanner.setVerticalAverage(getSetting("VERT_AVG").toInt());
    scanner.setFrameSkip(getSetting("FRAME_SKIP").toInt());
    scanner.setLineSkip(getSetting("LINE_SKIP").toInt());

    //setScannerParams(scanner, parameterMap);
    //std::stringstream scanResult;
    //scanner.processFrames(path2frames);//, scanResult, fileType);

    scanner.start(path2frames);

    //updateStatusBar("Writing scan to file", 0);
    //std::ofstream output3d(inDir.toUtf8() + "/scan.ply");
    //output3d << scanResult.str() << std::flush;
    //output3d.close();
    updateStatusBar("Scan Complete", 3000);




    }
}

void MainWindow::updateStatusBar(QString text, int timeOut){

    ui->statusBar->showMessage(text, timeOut);


}
