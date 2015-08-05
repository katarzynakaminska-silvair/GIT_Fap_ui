#include "rf_paf.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RF_Paf w;
    w.show();

    return a.exec();
}
