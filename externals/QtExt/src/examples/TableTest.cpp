#include <QtGui/QApplication>
#include "TableTestMain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TableTestMain w;
    w.show();
    return a.exec();
}
