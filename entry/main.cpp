//Project
#include "cgt/cgt.h"
#include "logger.h"
#include "package/packagemanager.h"

//STL

//Qt
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    logger::initialize();

    QCoreApplication a(argc, argv);

    //Задаём информацию о приложении
    QCoreApplication::setOrganizationName(APP_COMPANY);
    QCoreApplication::setOrganizationDomain(APP_COPYRIGHT);
    QCoreApplication::setApplicationName(APP_PRODUCT);
    QCoreApplication::setApplicationVersion(APP_VERSION);

    PackageManager *manager = new PackageManager();
    qInfo() << manager->getPackage("delphi");

    return a.exec();
}
