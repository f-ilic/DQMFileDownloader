#include "dqmfiledownloader.h"
#include "ui_dqmfiledownloader.h"

#include <QFileDialog>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <TEnv.h>
#include <TFile.h>

DQMFileDownloader::DQMFileDownloader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DQMFileDownloader)
{
    ui->setupUi(this);
    remote_files_model = new RemoteFilesModel(this);

    proxy_remote_files_model = new QSortFilterProxyModel(this);
    proxy_remote_files_model->setSourceModel(remote_files_model);

    ui->listView->setModel(proxy_remote_files_model);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->statusBar->showMessage("Ready!");
}

DQMFileDownloader::~DQMFileDownloader()
{
    delete ui;
}

void DQMFileDownloader::on_pushButton_clicked()
{
    // TODO: error handling if connection is refused.
    QString cert = "/home/fil/Documents/usercert.pem"; //TODO: QSettings
    QString key = "/home/fil/Documents/userkey.pem";   //TODO: QSettings
    gEnv->SetValue("Davix.GSI.UserCert", cert.toStdString().c_str());
    gEnv->SetValue("Davix.GSI.UserKey", key.toStdString().c_str());
    gEnv->SetValue("Davix.GSI.GridMode", true);
    gEnv->SetValue("Davix.GSI.CACheck", false);

    qDebug() << "Number of selected items: " << ui->listView->selectionModel()->selectedIndexes().size();

    // TODO: Make multithreaded
    // because it blocks the UI
    // might also avoid bottlenecking write speed
    for(auto& e : ui->listView->selectionModel()->selectedIndexes()) {
        auto real_idx = proxy_remote_files_model->mapToSource(e);
        QString name = remote_files_model->data(real_idx, Qt::DisplayRole).toString();
        QString url  = remote_files_model->getFilepath(real_idx);

        qDebug() << "Item: " << name << " => " << url;

        TFile* f = TFile::Open(url.toStdString().c_str());
        QString download_base_path = "/home/fil/projects/DQMFileDownloader/"; //TODO: QSettings
        QString download_path =  download_base_path + name;

        ui->statusBar->showMessage("Downloading... " + name);
        f->Cp(download_path.toStdString().c_str());
        ui->statusBar->showMessage("Ready!");
    }
}

void DQMFileDownloader::on_listView_doubleClicked(const QModelIndex &index)
{
    qDebug() << "Item double clicked";
}

void DQMFileDownloader::on_pushButton_2_clicked()
{
    QString filter = ui->lineEdit->text();
    proxy_remote_files_model->setFilterRegExp(filter);
}

void DQMFileDownloader::on_actionPreferences_triggered()
{
    qDebug() << "Settings clicked";
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select"), "./", tr("PEM certificate (*.pem)"));
}
