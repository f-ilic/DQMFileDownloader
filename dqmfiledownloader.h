#ifndef DQMFILEDOWNLOADER_H
#define DQMFILEDOWNLOADER_H

#include "remotefilesmodel.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>

namespace Ui {
class DQMFileDownloader;
}

class DQMFileDownloader : public QMainWindow
{
    Q_OBJECT
public:
    explicit DQMFileDownloader(QWidget *parent = 0);
    ~DQMFileDownloader();

    // this is static so that QtConcurrent accepts it without having to write a wrapper
    static bool download_tfile_from_url(QString download_path, QString url);

private slots:
    void on_pushButton_clicked();
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_pushButton_2_clicked();
    void on_actionPreferences_triggered();

private:
    void setupCertificates();

    Ui::DQMFileDownloader *ui;
    RemoteFilesModel      *remote_files_model;
    QSortFilterProxyModel *proxy_remote_files_model;
};

#endif // DQMFILEDOWNLOADER_H
