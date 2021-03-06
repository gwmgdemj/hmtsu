/*
 * HMTsu
 * Copyright (C) 2013 Lcferrum
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <pwd.h>
#include <QCoreApplication>
#include "usersmodel.h"

UsersModel::UsersModel(QString user, bool skip):
    QAbstractListModel(NULL),
    AvailableUsers(), ini_idx(-1)
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole]="name";
    setRoleNames(roles);

    if (skip||Intercom->IfError()) return;

    passwd *pw;

    setpwent();
    while ((pw=getpwent()))
        AvailableUsers.append(QString::fromLocal8Bit(pw->pw_name));
    endpwent();

    AvailableUsers.sort();

    if ((ini_idx=AvailableUsers.indexOf(user))==-1)
        Intercom->AddError(QCoreApplication::translate("Messages", "__usersmodel_wronguser_err%1__").arg(user));
}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:AvailableUsers.count();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row()<0||index.row()>=AvailableUsers.count())
        return QVariant();

    if (role==Qt::DisplayRole)
        return AvailableUsers[index.row()];

    return QVariant();
}

QVariant UsersModel::get(int index)
{
    if (index<0||index>=AvailableUsers.count())
        return QVariant();

    QMap<QString, QVariant> ReturnItem;
    ReturnItem.insert("name", QVariant(AvailableUsers[index]));

    return QVariant(ReturnItem);
}

int UsersModel::GetInitialIndex()
{
    return ini_idx;
}
