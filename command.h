#ifndef COMMAND_H
#define COMMAND_H
#include <QDialog>

namespace Ui {
    class command;
}

enum CMD_NUM
{

        CMD_POWERMEASUREMENT = 0x01,
        CMD_POWER_MEASUREMENT_WITH_CALI = 0x02,
        CMD_FREQUENCY_MEASUREMENT  = 0x03,
        CMD_ATTENUATION_SET = 0x04,
        CMD_REGLINM_SET  =0x05,
        CMD_SERIALNUM_SET = 0x06,
        CMD_SERIALNUM_GET = 0x07,
        CMD_FIRMWARE_WER_GET = 0x08,
        CMD_PEAK_POWER = 0x09,
        CMD_REGLINP_ADD_POINT_FOR_AUTOSET = 0x0A,
        CMD_REGLINP_SET_AUTO = 0x0B
};

class command: public QDialog
{
    Q_OBJECT

public:
    command(QString command_name, enum CMD_NUM command_num, int command_lenght);
    ~command();
    enum CMD_NUM get_command_ID(void);
    QString get_button_name(void);
    int get_command_lenght(void);

public slots:

signals:
private:
    enum CMD_NUM command_ID;
    QString cmd_name;
    int cmd_len;


};

#endif // COMMAND_H
