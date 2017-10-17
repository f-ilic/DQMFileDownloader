#include "dqmfiledownloader.h"
#include "ui_dqmfiledownloader.h"

#include "settingsdialog.h"
#include "settingsmanager.h"

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

    setupCertificates();
    QString download_base_path = SettingsManager::getInstance().getSetting(SETTING::DOWNLOAD_PATH);

    // TODO: Make multithreaded
    // because it blocks the UI
    // might also avoid bottlenecking write speed
    for(auto& e : ui->listView->selectionModel()->selectedIndexes()) {
        auto real_idx = proxy_remote_files_model->mapToSource(e);
        QString name = remote_files_model->data(real_idx, Qt::DisplayRole).toString();
        QString url  = remote_files_model->getFilepath(real_idx);

        qDebug() << "Item: " << name << " => " << url;

        TFile* f = TFile::Open(url.toStdString().c_str());
        QString download_path =  download_base_path + "/" + name;

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
