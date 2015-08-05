#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "command.h"
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QSerialPort>
#include <QTimer>



class protocol: public QObject
{
    Q_OBJECT
public:
    protocol(QObject *parent = 0);
    ~protocol();

signals:
    void new_module_data(QByteArray frme);
    void new_mesh_data(QByteArray frme);
    void portInfo(QString Info);
    void portError(QString error);
    void newFrameReceived(QByteArray frame);

public slots:
    bool start(QString name, quint32 baud);
    bool stop(void);
    bool isStarted(void);
    void newDataAvailable(void);
    void sendData(enum CMD_NUM command, QByteArray additional_data);
    void timeoutResponseExpired(void);
private:
    QSerialPort *port;
    QByteArray *frame;
    int frame_len;
    QTimer *timeoutResponse;

public:
    QList <command *> commands;
};

#endif // PROTOCOL_H
