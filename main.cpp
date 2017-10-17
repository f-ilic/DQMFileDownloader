
#include <QApplication>
#include <QDebug>
#include "dqmfiledownloader.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DQMFileDownloader dqm_file_dl;
    dqm_file_dl.show();
    return a.exec();
}
