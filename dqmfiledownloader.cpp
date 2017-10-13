#include "dqmfiledownloader.h"
#include "ui_dqmfiledownloader.h"
#include <iostream>

DQMFileDownloader::DQMFileDownloader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DQMFileDownloader)
{
    ui->setupUi(this);
    model = new RemoteFilesModel(this);

    ui->listView->setModel(model);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // model for the dropdown
    QStringList module_filters;
    module_filters << "SiStrip" << "other stuff";
    ui->comboBox->addItems(module_filters);


    ui->statusBar->showMessage("Ready");
}

DQMFileDownloader::~DQMFileDownloader()
{
    delete ui;
}

void DQMFileDownloader::on_pushButton_clicked()
{
    std::cout << "Number of selected items: " << ui->listView->selectionModel()->selectedIndexes().size() << std::endl;
    for(auto& e : ui->listView->selectionModel()->selectedIndexes()) {
        std::cout << model->data(e, Qt::DisplayRole).toString().toStdString()
                  << " | "
                  << model->getFilepath(e).toStdString()
                  << std::endl;
    }
}

void DQMFileDownloader::on_listView_doubleClicked(const QModelIndex &index)
{
    ui->statusBar->showMessage(model->data(index, Qt::DisplayRole).toString() +
                               " | " +
                               model->getFilepath(index));
}
