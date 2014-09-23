#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
#include <QSerialPort>
#include <QFile>
#include <QTimer>



class fileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit fileTransfer(QString scanSource = "" );
    ~fileTransfer();

signals:
    void finished();
    void addPointToCloud(float, float, float);

public slots:
    void getScan();

private slots:
    void handleReadyRead();
    void handleTimeout();


private:

    QSerialPort *serialport;

    QFile imageFile;

    QByteArray  m_readData;

    QTimer m_timer;

    QString m_saveFileName;

    QString m_scanSource;

    int count;

};

#endif // FILETRANSFER_H
