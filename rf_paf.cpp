#include "rf_paf.h"
#include "ui_rf_paf.h"
#include <QSerialPortInfo>

RF_Paf::RF_Paf(QWidget *parent) :
    QMainWindow(parent),
    protocol_inst(new protocol()),
    ui(new Ui::RF_Paf)
{
    ui->setupUi(this);


    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->CB_comList->addItem(info.portName());
        qDebug() <<info.portName() << info.description() << info.manufacturer() << info.serialNumber();
    }
    foreach (const quint32 baud, QSerialPortInfo::standardBaudRates()) {
        ui->CB_baudList->addItem(QString::number(baud));
        ui->CB_baudList->setCurrentText("57600");
    }
    QStringList SL_attenuation;
    SL_attenuation    << "0.0" << "0.5" << "1.0" << "1.5" << "2.0" << "2.5" << "3.0" << "3.5" << "4.0" << "3.5"<< "5.0" << "5.5" << "6.0" << "6.5" << "7.0" << "7.5" << "8.0" << "8.5" << "9.0"
                    << "9.5" << "10.0" << "10.5" << "11.0" << "11.5" << "12.0" << "12.5" << "13.0" << "13.5"<< "14.0" << "14.5" << "15.0" << "15.5" << "16.0" << "16.5" << "17.0" << "17.5" << "18.0"
                    << "18.5" << "19.0" << "19.5" << "20.0" << "20.5" << "21.0" << "21.5" << "22.0" << "22.5"<< "23.0" << "23.5" << "24.0" << "24.5" << "25.0" << "25.5" << "26.0" << "26.5" << "27.0"
                    << "27.5" << "28.0" << "28.5" << "29.0" << "29.5" << "30.0" << "30.5" << "31.0" << "31.5";
    ui->CB_kali_atten->addItems(SL_attenuation);
    ui->CB_kali_atten->setCurrentText("10.0");
    ui->L_logs->setStyleSheet("QLabel { color: red }");


    connect(ui->PB_start,SIGNAL(clicked()),this,SLOT(portStart()));
    connect(ui->PB_stop,SIGNAL(clicked()),this,SLOT(protStop()));
    connect(ui->PB_refresh,SIGNAL(clicked()),this,SLOT(protRefresh()));
    connect(protocol_inst, SIGNAL(portInfo(QString)), this, SLOT(setInfo(QString)));
    connect(protocol_inst, SIGNAL(portError(QString)), this, SLOT(setError(QString)));
    connect(protocol_inst, SIGNAL(newFrameReceived(QByteArray)), this, SLOT(newFrameHandle(QByteArray)));

    connect(ui->PB_powerMeasurement, SIGNAL(clicked()), this, SLOT(powerMeasurement()));
    connect(ui->PB_peakPower, SIGNAL(clicked()), this, SLOT(peakPower()));
    connect(ui->PB_frequencyMeasurement, SIGNAL(clicked()), this, SLOT(frequencyMeasurement()));
    connect(ui->PB_maxPower, SIGNAL(clicked()), this, SLOT(maxPower()));
    connect(ui->PB_kali, SIGNAL(clicked()), this, SLOT(kali()));
    connect(ui->PB_serialNumSet, SIGNAL(clicked()), this, SLOT(serialNumSet()));
    connect(ui->PB_serialNumGet, SIGNAL(clicked()), this, SLOT(serialNumGet()));
    connect(ui->PB_firmVersion, SIGNAL(clicked()), this, SLOT(firmVersion()));

}

RF_Paf::~RF_Paf()
{
    delete ui;
}

void RF_Paf::portStart(void)
{
    if(protocol_inst->start(ui->CB_comList->currentText(),ui->CB_baudList->currentText().toUInt()))
    {
        setInfo("Device started.");
    }
    else
    {
        setError("Device not started.");
    }
}

void RF_Paf::protStop(void)
{
    if(protocol_inst->stop())
    {
        setInfo("Device stopped.");
    }
    else
    {
        setError("Device is already stopped.");
    }
}

void RF_Paf::protRefresh(void)
{
    if(!protocol_inst->isStarted())
    {
        ui->CB_comList->clear();
        ui->CB_baudList->clear();
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            ui->CB_comList->addItem(info.portName());
        }
        foreach (const quint32 baud, QSerialPortInfo::standardBaudRates()) {
            ui->CB_baudList->addItem(QString::number(baud));
        }
        ui->CB_baudList->setCurrentText("57600");
        setInfo("Device list refreshed.");
    }
    else
    {
        setError("Can't refresh device list.");
    }
}


void RF_Paf::setInfo(QString infoTxt)
{
       ui->L_logs->setStyleSheet("QLabel {color : green; }");
       ui->L_logs->setText(infoTxt);
}

void RF_Paf::setError(QString infoTxt)
{
       ui->L_logs->setStyleSheet("QLabel {color : red; }");
       ui->L_logs->setText(infoTxt);
}

void RF_Paf::newFrameHandle(QByteArray frame)
{
    QString tempString;
    switch((int)frame.at(0))
    {
        case CMD_POWERMEASUREMENT:
        ui->LE_powerMeasurement->setText(QString::number((double)BytesToFloat(frame.mid(1,4))));
        ui->LE_uPTemp->setText(QString::number((double)BytesToFloat(frame.mid(5,4))));
        ui->LE_refVoltage->setText(QString::number((double)BytesToFloat(frame.mid(9,4))));
        ui->LE_detectorTemp->setText(QString::number((double)BytesToFloat(frame.mid(13,4))));
        ui->L_errors->setText(QString::number((BytesToquint8(frame.mid(17,1))),2));
        break;
        case CMD_POWER_MEASUREMENT_WITH_CALI:
        ui->LE_powerMeasurement->setText(QString::number((double)BytesToFloat(frame.mid(1,4))));
        ui->LE_uPTemp->setText(QString::number((double)BytesToFloat(frame.mid(5,4))));
        ui->LE_refVoltage->setText(QString::number((double)BytesToFloat(frame.mid(9,4))));
        ui->LE_detectorTemp->setText(QString::number((double)BytesToFloat(frame.mid(13,4))));
        ui->L_errors->setText(QString::number((BytesToquint8(frame.mid(17,1))),2));
        break;
        case CMD_FREQUENCY_MEASUREMENT:
        ui->LE_frequencyMeasurement->setText(QString::number((double)BytesToDouble(frame.mid(1,8)), 'g', 8));
        ui->LE_uPTemp->setText(QString::number((double)BytesToFloat(frame.mid(9,4))));
        ui->LE_refVoltage->setText(QString::number((double)BytesToFloat(frame.mid(13,4))));
        ui->LE_detectorTemp->setText(QString::number((double)BytesToFloat(frame.mid(17,4))));
        ui->L_errors->setText(QString::number((BytesToquint8(frame.mid(21,1))),2));
        break;
        case CMD_ATTENUATION_SET:
        setInfo("Attenuation Set done.");
        break;
        case CMD_REGLINM_SET:
        setInfo("Reglin Set done.");
        break;
        case CMD_SERIALNUM_SET:
        setInfo("Serialnum Set done.");
        break;
        case CMD_SERIALNUM_GET:
        ui->LE_serialnum->setText(QString::number((double)BytesToUint(frame.mid(1,4))));
        break;
        case CMD_FIRMWARE_WER_GET:
        tempString.append(QString::number(BytesToquint8(frame.mid(1,1))).append("."));
        tempString.append(QString::number(BytesToquint8(frame.mid(2,1))).append("."));
        tempString.append(QString::number(BytesToquint8(frame.mid(3,1))));
        ui->LE_firmVersion->setText(tempString);
        break;
        case CMD_PEAK_POWER:
        ui->LE_peakPower->setText(QString::number((double)BytesToFloat(frame.mid(1,4))));
        ui->LE_uPTemp->setText(QString::number((double)BytesToFloat(frame.mid(5,4))));
        ui->LE_refVoltage->setText(QString::number((double)BytesToFloat(frame.mid(9,4))));
        ui->LE_detectorTemp->setText(QString::number((double)BytesToFloat(frame.mid(13,4))));
        ui->L_errors->setText(QString::number((BytesToquint8(frame.mid(17,1))),2));
        break;
        case CMD_REGLINP_ADD_POINT_FOR_AUTOSET:
        break;
        case CMD_REGLINP_SET_AUTO:
        break;
    }
}

float RF_Paf::BytesToFloat(QByteArray BA)
{
    float *float_buf;
    float_buf = (float*)BA.data();
    return *float_buf;
}

double RF_Paf::BytesToDouble(QByteArray BA)
{
    double *double_buf;
    double_buf = (double*)BA.data();
    return *double_buf;
}

uint RF_Paf::BytesToUint(QByteArray BA)
{
    uint *uint_buf;
    uint_buf = (uint*)BA.data();
    return *uint_buf;
}

quint8 RF_Paf::BytesToquint8(QByteArray BA)
{
    quint8 *quint8_buf;
    quint8_buf = (quint8*)BA.data();
    return *quint8_buf;
}

QByteArray RF_Paf::FloatToBytes(float f)
{
    QByteArray array(reinterpret_cast<const char*>(&f), sizeof(f));
    return array;
}

QByteArray RF_Paf::UintToBytes(uint u)
{
    QByteArray array(reinterpret_cast<const char*>(&u), sizeof(u));
    return array;
}

void RF_Paf::powerMeasurement()
{
    protocol_inst->sendData(CMD_POWERMEASUREMENT, NULL);
}

void RF_Paf::peakPower()
{
    protocol_inst->sendData(CMD_PEAK_POWER, NULL);
}

void RF_Paf::frequencyMeasurement()
{
    protocol_inst->sendData(CMD_FREQUENCY_MEASUREMENT, NULL);
}

void RF_Paf::maxPower()
{
    QByteArray frame;
    if(ui->LE_maxPower->text().isEmpty())
    {
        setError(tr("Set correct value of Max power field"));
    }
    else
    {
        frame.append(FloatToBytes(ui->LE_maxPower->text().toFloat()));
        protocol_inst->sendData(CMD_ATTENUATION_SET, frame);
    }
}

void RF_Paf::kali()
{
    QByteArray frame;
    if(ui->LE_kaliA->text().isEmpty() | ui->LE_kaliB->text().isEmpty() | ui->LE_kaliVcc->text().isEmpty())
    {
        setError("Set all calibration values!");
    }
    else
    {
        frame.append(FloatToBytes(ui->LE_kaliA->text().toFloat()));
        frame.append(FloatToBytes(ui->LE_kaliB->text().toFloat()));
        frame.append(FloatToBytes(ui->LE_kaliVcc->text().toFloat()));
        frame.append(FloatToBytes(ui->CB_kali_atten->currentText().toFloat()));
        protocol_inst->sendData(CMD_REGLINM_SET, frame);
    }

}

void RF_Paf::serialNumSet()
{
    QByteArray frame = UintToBytes(ui->LE_serialnum->text().toUInt());
    protocol_inst->sendData(CMD_SERIALNUM_SET, frame);
}

void RF_Paf::serialNumGet()
{
    protocol_inst->sendData(CMD_SERIALNUM_GET, NULL);
}

void RF_Paf::firmVersion()
{
    protocol_inst->sendData(CMD_FIRMWARE_WER_GET, NULL);
}
