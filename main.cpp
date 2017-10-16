#include "dqmfiledownloader.h"
#include <iostream>
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    qDebug() << "Applicatoin started";
    QApplication a(argc, argv);
    DQMFileDownloader dqm_file_dl;
    dqm_file_dl.show();
    return a.exec();
}
