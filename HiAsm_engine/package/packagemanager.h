#pragma once

//Project
#include "package.h"

//STL

//Native

//Qt
#include <QtCore>

class PackageManager : public QObject {

private:
    const QString m_packagesDir = "packages";
    QString m_pathPackages;
    Packages m_packages;

public:
    explicit PackageManager(QObject *parent = 0);

public:
    Package *getPackage(const QString &namePack);
};
