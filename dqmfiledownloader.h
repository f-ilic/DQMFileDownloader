#ifndef DQMFILEDOWNLOADER_H
#define DQMFILEDOWNLOADER_H

#include <QMainWindow>
#include "remotefilesmodel.h"


namespace Ui {
class DQMFileDownloader;
}

class DQMFileDownloader : public QMainWindow
{
    Q_OBJECT

public:
    explicit DQMFileDownloader(QWidget *parent = 0);
    ~DQMFileDownloader();

private slots:
    void on_pushButton_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::DQMFileDownloader *ui;
    RemoteFilesModel     *model;
};

#endif // DQMFILEDOWNLOADER_H
