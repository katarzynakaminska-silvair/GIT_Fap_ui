#include "protocol.h"
#include <QThread>

protocol::protocol(QObject *parent)
{
    port = new QSerialPort();
    frame = NULL;
    commands.append(new command(tr("Power measurement"),CMD_POWERMEASUREMENT, 19));
    commands.append(new command(tr("Power measurement with cali"),CMD_POWER_MEASUREMENT_WITH_CALI,19 ));
    commands.append(new command(tr("Frequency measurement"),CMD_FREQUENCY_MEASUREMENT, 23 ));
    commands.append(new command(tr("Attenuation set"),CMD_ATTENUATION_SET,2 ));
    commands.append(new command(tr("Reglinp set"),CMD_REGLINM_SET,2 ));
    commands.append(new command(tr("Serialnum set"),CMD_SERIALNUM_SET, 2 ));
    commands.append(new command(tr("Serialnum get"),CMD_SERIALNUM_GET,5 ));
    commands.append(new command(tr("Firmware wersion get"),CMD_FIRMWARE_WER_GET, 4));
    commands.append(new command(tr("Peak power measurement"),CMD_PEAK_POWER, 19 ));
    commands.append(new command(tr("Add reglinp point"),CMD_REGLINP_ADD_POINT_FOR_AUTOSET, 1));
    commands.append(new command(tr("Reglinp set auto"),CMD_REGLINP_SET_AUTO, 1 ));

    timeoutResponse = new QTimer();
    timeoutResponse->setSingleShot(true);
    connect(port,SIGNAL(readyRead()),this,SLOT(newDataAvailable()));
    connect(timeoutResponse,SIGNAL(timeout()),this,SLOT(timeoutResponseExpired()));
}

protocol::~protocol()
{
    delete port;
}

bool protocol::start(QString name, quint32 baud)
{
#ifdef __APPLE__
    port->setPortName("/dev/tty." + name);
#else
    port->setPortName(name);
#endif
    port->setBaudRate(baud);
    port->setParity(QSerialPort::NoParity);
    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setStopBits(QSerialPort::OneStop);
    if(port->open(QIODevice::ReadWrite))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool protocol::stop()
{
    if(port->isOpen())
    {
        port->close();
        return true;
    }
    else
    {
        return false;
    }
}

bool protocol::isStarted()
{
    return port->isOpen();
}

void protocol::timeoutResponseExpired(void)
{
    emit portError("Response timeout expired!");
}

void protocol::newDataAvailable()
{
        char rx;
        while(port->read(&rx,1) > 0)
        {
            if(frame == NULL)
            {
                frame = new QByteArray();

                foreach(command* c, commands)
                {
                    if(c->get_command_ID() == rx)
                    {
                        frame_len = c->get_command_lenght();
                        break;
                    }
                }
            }
            frame->append(rx);
            if(frame->length() == frame_len)
            {
                timeoutResponse->stop();
                qDebug() << "[PROT] Full frame recieved. {0x" << frame->toHex() << "}"  ;
                portInfo("Full frame recieved. 0x" + frame->toHex());
                emit newFrameReceived(*frame);
                delete frame;
                frame = NULL;
            }
        }
}

void protocol::sendData(CMD_NUM command, QByteArray additional_data)
{
    QByteArray command_frame;
    command_frame.append(command).append(additional_data);
    if(port->isOpen())
    {
        if(!timeoutResponse->isActive())
        {
            port->write(command_frame);
            timeoutResponse->start(1500);
            emit portInfo("Command sent");
        }
        else
        {
            emit portError("Interface is busy!");
        }
    }
    else
    {
        emit portError("Device is stopped!");
    }
}
