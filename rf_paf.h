#ifndef RF_PAF_H
#define RF_PAF_H

#include <QMainWindow>
#include "protocol.h"

namespace Ui {
class RF_Paf;
}

class RF_Paf : public QMainWindow
{
    Q_OBJECT

public:
    explicit RF_Paf(QWidget *parent = 0);
    ~RF_Paf();

private:
    Ui::RF_Paf *ui;
    protocol *protocol_inst;
    float BytesToFloat(QByteArray BA);
    double BytesToDouble(QByteArray BA);
    uint BytesToUint(QByteArray BA);
    quint8 BytesToquint8(QByteArray BA);
    QByteArray FloatToBytes(float f);
    QByteArray UintToBytes(uint u);


public slots:
    void portStart(void);
    void protStop(void);
    void protRefresh(void);
    void setInfo(QString infoTxt);
    void setError(QString infoTxt);
    void newFrameHandle(QByteArray frame);
    void powerMeasurement();
    void peakPower();
    void frequencyMeasurement();
    void maxPower();
    void kali();
    void serialNumSet();
    void serialNumGet();
    void firmVersion();

};

#endif // RF_PAF_H
