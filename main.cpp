#include "qpad.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qPad w;
    w.show();

    return a.exec();
}
