#include "dqmfiledownloader.h"
#include "ui_dqmfiledownloader.h"

#include "settingsdialog.h"
#include "settingsmanager.h"

#include <QFileDialog>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <TEnv.h>
#include <TFile.h>
#include <QtConcurrent/QtConcurrent>

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

bool DQMFileDownloader::download_tfile_from_url(QString download_path, QString url)
{
    TFile* f = TFile::Open(url.toStdString().c_str());
    if(f) {
        f->Cp(download_path.toStdString().c_str());
        f->Close();
    }
    return f;
}

void DQMFileDownloader::on_pushButton_clicked()
{
    setupCertificates();
    QString download_base_path = SettingsManager::getInstance().getSetting(SETTING::DOWNLOAD_PATH);

    for(auto& e : ui->listView->selectionModel()->selectedIndexes()) {
        auto real_idx = proxy_remote_files_model->mapToSource(e);
        QString name = remote_files_model->data(real_idx, Qt::DisplayRole).toString();
        QString url  = remote_files_model->getFilepath(real_idx);
        QString download_path =  download_base_path + "/" + name;

        QFuture<bool> future = QtConcurrent::run(DQMFileDownloader::download_tfile_from_url, download_path, url);
        qDebug() << name << " downloaded: " << future;
        if(!future) {
            ui->statusBar->showMessage("Could not download " + name);
        }
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
    SettingsDialog* settings_dialog = new SettingsDialog;
    settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
    settings_dialog->show();
}

void DQMFileDownloader::setupCertificates()
{
    auto& instance =  SettingsManager::getInstance();
    QString cert = instance.getSetting(SETTING::USER_CERTIFICATE_PATH);
    QString key = instance.getSetting(SETTING::USER_KEY_PATH);
    gEnv->SetValue("Davix.GSI.UserCert", cert.toStdString().c_str());
    gEnv->SetValue("Davix.GSI.UserKey", key.toStdString().c_str());
    gEnv->SetValue("Davix.GSI.GridMode", true);
    gEnv->SetValue("Davix.GSI.CACheck", false);
}
