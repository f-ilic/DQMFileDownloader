#include "remotefilesmodel.h"
#include <iostream>

RemoteFilesModel::RemoteFilesModel(QObject* parent)
{
    this->parent = parent;
    remote_files.push_back(FileContainer("file1","some/path/to/file1"));
    remote_files.push_back(FileContainer("file2","some/other/path/to/file2"));
    remote_files.push_back(FileContainer("file3","completely/different/path/to/file3"));

    // fill the remote_files here with:
    // read the db file line by line and fill it with with: Filename; FilePath
}


int RemoteFilesModel::rowCount(const QModelIndex& parent) const
{
    return remote_files.size();
}

QVariant RemoteFilesModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if (role == Qt::DisplayRole) {
        return remote_files[row].getName();
    }

    return QVariant();
}

QString RemoteFilesModel::getFilepath(const QModelIndex& index) const
{
    int row = index.row();
    return remote_files[row].getPath();
}

