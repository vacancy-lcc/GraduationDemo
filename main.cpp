#include "widget.h"
#include <QApplication>
#include "rfid.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    signal(SIGALRM, refresh);
    Widget w;
    w.show();

    return a.exec();
}
