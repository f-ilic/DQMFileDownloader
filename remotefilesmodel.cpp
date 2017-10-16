#include <QFile>
#include <QTextStream>
#include "remotefilesmodel.h"
#include <iostream>


RemoteFilesModel::RemoteFilesModel(QObject* parent)
{
    this->parent = parent;

    QString files_list_path = "/home/fil/projects/DQMFileDownloader/remote_files.txt"; //TODO: QSettings
    QFile file(files_list_path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    in.setCodec("UTF-8");

    //TODO: fix std::string => QString
    // use QStringRef for filename
    while(!in.atEnd()) {
        std::string path = in.readLine().toStdString();

        int beginIdx = path.rfind('/');
        std::string display_name = path.substr(beginIdx + 1);

        remote_files.push_back(FileContainer(display_name, path));
    }
}


int RemoteFilesModel::rowCount(const QModelIndex& parent) const
{
    return remote_files.size();
}

QVariant RemoteFilesModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(role == Qt::DisplayRole) {
        return remote_files[row].getName();
    }
    if(role == Qt::ToolTipRole) {
        return remote_files[row].getPath();
    }

    return QVariant();
}

QString RemoteFilesModel::getFilepath(const QModelIndex& index) const
{
    int row = index.row();
    return remote_files[row].getPath();
}
