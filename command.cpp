#include "command.h"


command::command(QString command_name, enum CMD_NUM command_num, int command_lenght)

{
        command_ID = command_num;
        cmd_name = command_name;
        cmd_len = command_lenght;
}

command::~command(){}

enum CMD_NUM command::get_command_ID()
{
    return command_ID;
}

QString command::get_button_name(void)
{
    return cmd_name;
}

int command::get_command_lenght(void)
{
    return cmd_len;
}
