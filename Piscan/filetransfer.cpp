#include <QDebug>
#include <QSerialPortInfo>

#include "filetransfer.h"

fileTransfer::fileTransfer(QString scanSource)
{

   m_scanSource = scanSource;
   // m_saveFileName = saveFileName;
   count = 0;


}


fileTransfer::~fileTransfer(){
        delete serialport;
}

void fileTransfer::getScan()
{

    serialport = new QSerialPort();
    serialport->setPortName(m_scanSource);
    serialport->open(QIODevice::ReadWrite);
    serialport->setBaudRate(QSerialPort::Baud115200);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setFlowControl(QSerialPort::NoFlowControl);

    //imageFile.setFileName(m_saveFileName);
    //imageFile.open(QIODevice::WriteOnly);
    //imageFile.write("P6\n 320 240 255\n");

    serialport->write("scan");

    connect(serialport, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    //connect(&m_timer, SIGNAL(timeout()), SLOT(handleTimeout()));

}


void fileTransfer::handleReadyRead()
{

    m_readData.append(serialport->readAll());

    //qDebug() << m_readData << endl;

    if(m_readData.contains('F'))
    {
        //count++;
        while(m_readData.at(0) != 'E')
        {


        //qDebug() << m_readData.at(0);
        int xIndex = m_readData.indexOf("X");
        int yIndex = m_readData.indexOf("Y");
        int zIndex = m_readData.indexOf("Z");
        int cIndex = m_readData.indexOf(":");
        //qDebug() << "X: " << m_readData.at(xIndex) << " Y: " << m_readData.at(yIndex) << " Z: " << m_readData.at(zIndex) << " : " << m_readData.at(cIndex) << endl;
        //qDebug() << cIndex << endl;
        //qDebug() << m_readData.at(0) << endl;
        //qDebug() << "m_readData length " << m_readData.size() << endl;

        QByteArray X = m_readData.mid(xIndex + 1, yIndex -1);
        QByteArray Y = m_readData.mid(yIndex + 1, zIndex - yIndex -1);
        QByteArray Z = m_readData.mid(zIndex + 1, cIndex - zIndex -1);
        emit addPointToCloud(X.toFloat(), Y.toFloat(), Z.toFloat());
        //qDebug() << "X: " << X << " Y: " << Y << " Z: " << Z << endl;
        m_readData.remove(0, cIndex + 1);
        }

        }


        if(m_readData.at(0) == 'E')
        {
            m_readData.clear();
            //qDebug() << "readdata cleared" << endl;
        }



        //qDebug() << "count " << count << endl;

}


void fileTransfer::handleTimeout()
{

        m_timer.stop();
        imageFile.write(m_readData);
        imageFile.close();
        serialport->close();
        qDebug() << "Done Reading File" << endl;
        emit finished();
}

