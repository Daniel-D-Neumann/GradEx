#include "pagwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PAGWindow w;
    w.show();
    return QApplication::exec();
}
