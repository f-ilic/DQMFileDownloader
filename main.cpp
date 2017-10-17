
#include <QApplication>
#include <QDebug>
#include "dqmfiledownloader.h"
#include "settingsmanager.h"



int main(int argc, char *argv[])
{
    qDebug() << "Applicatoin started";

    qDebug() << "Download_path: "  << SettingsManager::getSetting("download_path");
    qDebug() << "User_cert_path: " << SettingsManager::getSetting("user_cert_path");
    qDebug() << "User_key_path: "  << SettingsManager::getSetting("user_key_path");

    QApplication a(argc, argv);
    DQMFileDownloader dqm_file_dl;
    dqm_file_dl.show();
    return a.exec();
}
