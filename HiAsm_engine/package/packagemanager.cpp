//Project
#include "packagemanager.h"
#include "package.h"

//STL

//Native

//Qt
#include <QtCore>

PackageManager::PackageManager(QObject *parent)
    : QObject(parent)
{
}

QString PackageManager::getPackagesPath()
{
    return QDir::currentPath() + QDir::separator() + c_PACKAGES_DIR;
}

Package *PackageManager::getPackage(const QString &packName)
{
    if (m_packages.contains(packName))
        return m_packages[packName];

    Package *pack = new Package(getPackagesPath() + QDir::separator() + packName);
    if (pack->getSuccess()) {
        m_packages.insert(packName, pack);
        return pack;
    } else {
        delete pack;
        return nullptr;
    }
}
