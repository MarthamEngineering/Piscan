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
#include "processscan.h"
#include "exportfile.h"
//#include "pointcloud.h"
#include "devicedialog.h"

#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QStatusBar>
#include <QSerialPortInfo>
#include <QThread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



  /*  //Load settings
    QSettings settings("MakerScan", "PiScan");
    if (settings.contains("CAMERA_HFOV") == false){
        qDebug() << "Write Settings\n";
        setSettings(true, "", "", "", "", "", "", "","","");
    }
*/
    loadSettings();
    //send the bed diamter to the ui
    ui->GLScanWidgetui->setBedSize((getSetting("BED_DIA")).toFloat());

}



MainWindow::~MainWindow()
{
    delete serialport;
    delete pointcloud;
    delete ui;

}


void MainWindow::setSettings(bool defaultSettings, QVariant CAMERA_HFOV, QVariant CAMERA_VFOV, QVariant CAMERA_DISTANCE,
                             QVariant LASER_OFFSET, QVariant HORIZ_AVG, QVariant VERT_AVG, QVariant FRAME_SKIP, QVariant LINE_SKIP, QVariant BED_DIA){

    //Do some checking here to make sure we are using the proper types for the settings.

    //initialise the settings configuation
     QSettings settings("MakerScan", "PiScan");

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
         settings.setValue("BED_DIA", BED_DIA);
         //settings.endGroup();
     }else{
        qDebug() << "Clear Settings\n";

        settings.clear();
        loadSettings();
    }

}

QVariant MainWindow::getSetting(QString setting){

    QSettings settings("MakerScan", "PiScan");
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
    }else if (setting == "BED_DIA") {
    returnSetting = settings.value("BED_DIA", 125).toString();
}

    return returnSetting;

}

void MainWindow::on_pushButtonResetSettings_clicked()
{
    setSettings(true, "", "", "", "", "", "", "","","");
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
    QVariant BED_DIA = ui->lineEditBedDiameter->text();
    //Add some error checking

    setSettings(false, CAMERA_HFOV, CAMERA_VFOV, CAMERA_DISTANCE, LASER_OFFSET, HORIZ_AVG, VERT_AVG, FRAME_SKIP, LINE_SKIP, BED_DIA);
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
    ui->lineEditBedDiameter->setText(getSetting("BED_DIA").toString());
    updateStatusBar("Settings Loaded", 3000);

}

void MainWindow::on_pushButtonProcessScan_clicked()
{

    //DeviceSelectionForm *scanpopup = new device(this);
    DeviceDialog dd(this);

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
       // qDebug() << "Name        : " << info.portName();
       // qDebug() << "Description : " << info.description();
       // qDebug() << "Manufacturer: " << info.manufacturer();
       // qDebug() << "Location: " << info.systemLocation();
       // qDebug() << "ID" << info.productIdentifier();

        if ((info.portName() != "") & (info.description() != "") & (info.manufacturer() != ""))
         {
            dd.addToList(info.systemLocation());
         }
     }

    QString imageFiles = "From Image Files";
    dd.addToList(imageFiles);

    if(dd.exec())
    {
        scanSource = dd.returnSelection();
        qDebug() << "Device selected: " << scanSource;

        if(scanSource != imageFiles)
        {
            getScanFromDevice();
        }
        else
        {
            getScanFromFile();
        }

    }

}

void MainWindow::getScanFromDevice()
{

    serialport = new QSerialPort(this);
    serialport->setPortName(scanSource);
    serialport->open(QIODevice::ReadWrite);
    serialport->setBaudRate(QSerialPort::Baud115200);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setFlowControl(QSerialPort::NoFlowControl);

   serialport->write("1");
   serialport->close();

}

void MainWindow::getScanFromFile()
    {

    //TO DO - Use native dialog once bug is resolved with multiple windows and the file browser not closing


    inDir = QFileDialog::getExistingDirectory(NULL, "Open Directory",QDir::homePath(),QFileDialog::DontUseNativeDialog | QFileDialog::DontResolveSymlinks);

    if (inDir != ""){

    ui->pushButtonSaveScan->setEnabled(false);

    //ProcessScan scanner(inDir);

    ProcessScan *scanner = new ProcessScan(inDir);

    //processscan::ProcessScan scanner;

    if (pointcloud != NULL)
        delete pointcloud;

    pointcloud = new pointCloud;

    // Make a connection to allow the ScanDraiD scanner object return data.
    connect(scanner, SIGNAL(percentageComplete(QString, int)), this, SLOT(updateStatusBar(QString,int)));
    connect(scanner, SIGNAL(percentageComplete(QString, int)), ui->GLScanWidgetui, SLOT(redraw()));

    connect(scanner, SIGNAL(addPointToCloud(float,float,float)), pointcloud, SLOT(addPoint(float,float,float)));

    scanner->setCameraHFov(getSetting("CAMERA_HFOV").toFloat(), true);
    scanner->setCameraVFov(getSetting("CAMERA_VFOV").toFloat());
    scanner->setCameraDistance(getSetting("CAMERA_DISTANCE").toFloat());
    scanner->setLaserOffset(getSetting("LASER_OFFSET").toFloat());
    scanner->setHorizontalAverage(getSetting("HORIZ_AVG").toInt());
    scanner->setVerticalAverage(getSetting("VERT_AVG").toInt());
    scanner->setFrameSkip(getSetting("FRAME_SKIP").toInt());
    scanner->setLineSkip(getSetting("LINE_SKIP").toInt());

    //QThread *process = new QThread;
    //scanner.moveToThread(process);

    //connect(process, SIGNAL(started()), &scanner, SLOT(start()));
    //process->start();


    QThread* thread = new QThread;
    //Worker* worker = new Worker();
    scanner->moveToThread(thread);
    //connect(scanner, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), scanner, SLOT(start()));
    connect(scanner, SIGNAL(finished()), thread, SLOT(quit()));
    connect(scanner, SIGNAL(finished()), scanner, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    //scanner->start();

    //Move this from here. it doesnt work because of the thread
    updateStatusBar("Scan Complete", 3000);
    ui->pushButtonSaveScan->setEnabled(true);

    //qDebug() << "Number of points: " << pointcloud->cloudSize();

    ui->GLScanWidgetui->setCloud(pointcloud);

    //pointcloud->refineCloud();


    }

}

void MainWindow::updateStatusBar(QString text, int timeOut){

    ui->statusBar->showMessage(text, timeOut);


}

void MainWindow::on_pushButtonSaveScan_clicked()
{

    QString fileExtension;
    QString savefileName = QFileDialog::getSaveFileName(this,"Save File", QString(QDir::homePath() + "/untitled"), ".ply;;.stl", &fileExtension, QFileDialog::DontUseNativeDialog );

    exportfile file(pointcloud);

    connect(&file, SIGNAL(saveComplete(QString, int)), this, SLOT(updateStatusBar(QString,int)));

    file.save(savefileName, fileExtension);

}
