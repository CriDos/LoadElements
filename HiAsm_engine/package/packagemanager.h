#pragma once

//Project
#include "package.h"

//STL

//Native

//Qt
#include <QtCore>

class PackageManager : public QObject
{
    //constants...
public:
    const QString c_PACKAGES_DIR = "packages";

    //private variables
private:
    Packages m_packages;

public:
    explicit PackageManager(QObject *parent = 0);

public:
    QString getPackagesPath();
    Package *getPackage(const QString &packName);
};
