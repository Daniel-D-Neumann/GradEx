#include "pagwindow.h"
#include "loadingwindow.h"
#include "SoundGenerator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //Connect QEnums etc


    QApplication a(argc, argv);

    LoadingWindow w;
    w.show();

    SoundGenerator* soundGen = new SoundGenerator();

    w.hide();

    PAGWindow n(soundGen);
    n.show();

    return QApplication::exec();
}
