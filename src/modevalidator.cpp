#include <unistd.h>
#include <QCoreApplication>
#include "modevalidator.h"

ModeValidator::ModeValidator(RunModes::QmlEnum mode, bool skip):
    QAbstractListModel(NULL)
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole]="name";
    roles[Qt::UserRole]="mode";
    setRoleNames(roles);

    if (skip||Intercom->IfError()) return;

    if (!access("/bin/devel-su", R_OK))
        AvailableModes.append(ModePair("devel-su", RunModes::SU));
    if (!access("/usr/bin/sudo", R_OK))
        AvailableModes.append(ModePair("sudo", RunModes::SUDO));
    if (!access("/usr/bin/ariadne", R_OK))
        AvailableModes.append(ModePair("ariadne", RunModes::ARIADNE));

    if (AvailableModes.count()<=0)
        Intercom->AddError(QCoreApplication::translate("Messages", "__modevalidator_nomodes_err__"));

    if ((ini_idx=AvailableModes.indexOf(ModePair("", mode)))==-1)
        Intercom->AddError(QCoreApplication::translate("Messages", "__modevalidator_wrongmode_err__"));
}

int ModeValidator::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:AvailableModes.count();
}

QVariant ModeValidator::data(const QModelIndex &index, int role) const
{
    if (index.row()<0||index.row()>=AvailableModes.count())
        return QVariant();

    if (role==Qt::DisplayRole)
        return AvailableModes[index.row()].first;
    else if (role == Qt::UserRole)
        return AvailableModes[index.row()].second;

    return QVariant();
}

QVariant ModeValidator::get(int index)
{
    if (index<0||index>=AvailableModes.count())
        return QVariant();

    QMap<QString, QVariant> ReturnItem;
    ReturnItem.insert("name", QVariant(AvailableModes[index].first));
    ReturnItem.insert("mode", QVariant(AvailableModes[index].second));

    return QVariant(ReturnItem);
}

int ModeValidator::GetInitialIndex()
{
    return ini_idx;
}
