//Project
#include "package.h"
#include "confelement.h"

//STL

//Native

//Qt
#include <QDebug>
#include <QDir>
#include <QJsonDocument>

Package::Package(const QString &packagePath, QObject *parent)
    : QObject(parent)
    , m_packagePath(packagePath)
{
    //!!! Назначение путей !!!

    //Задаём путь к файлу информации о пакете
    m_packageFileInfo = m_packagePath + QDir::separator() + c_INFO_FILE_NAME;
    if (!QFile::exists(m_packageFileInfo)) {
        return;
    }

    //Задаём путь к папке с элементами
    m_confPath = m_packagePath + QDir::separator() + c_CONF_DIR;

    //!!! Загружаем информацию о пакете/элементах !!!

    //Если не удалось по каким либо причинам прочитать информацию о пакете
    if (!loadPackageInfo()) {
        return;
    }

    //Если не удалось найти и загрузить информацию об элементах
    if (!loadElements()) {
        return;
    }

    //Добавляем информацию из наследуемых элементов
    m_isSuccess = true;
}

//ru Получаем информацию об пакете и его проектах
//ru из файла описания проекта: package
bool Package::loadPackageInfo()
{
    QFile packageFileInfo(m_packageFileInfo);
    packageFileInfo.open(QIODevice::ReadOnly);
    const QByteArray dataPackageInfo = packageFileInfo.readAll();
    packageFileInfo.close();

    const QJsonDocument info = QJsonDocument::fromJson(dataPackageInfo);
    if (!info.isNull()) {
        const QJsonObject infoPackage = info.object();

        //!ru Получаем информацию о пакете

        //ru Название пакета - обязательно заполнять.
        if (infoPackage.contains("name")) {
            m_name = infoPackage["name"].toString();
        } else {
            return false;
        }

        //ru Короткое описание пакета - необязательно заполнять.
        if (infoPackage.contains("shortDescription")) {
            m_shortDescription = infoPackage["shortDescription"].toString();
        }

        //ru Полное описание пакета - необязательно заполнять.
        if (infoPackage.contains("description")) {
            m_description = infoPackage["description"].toString();
        }

        //ru Видимость пакета - необязательный параметр.
        //ru По умолчанию - видимый.
        if (infoPackage.contains("visible"))
            m_visible = infoPackage["visible"].toBool();

        //ru Является ли пакет базовым - необязательный параметр
        //ru По умолчанию - нет
        if (infoPackage.contains("base"))
            m_base = infoPackage["base"].toBool();

        //!ru Получаем информацию о компиляторах
        for (auto json : infoPackage["compilers"].toArray()) {
            QJsonObject object = json.toObject();

            //ru Создаём объект проекта и заполняем его.
            SharedCompilerInfo compiler = SharedCompilerInfo::create();

            //ru Добавляем проект в массив проектов текущего пакета.
            m_compilerList.append(compiler);

            //ru Название проекта - обязательно заполнять.
            if (object.contains("name")) {
                compiler->name = object["name"].toString();
            } else {
                return false;
            }

            //ru Путь к файлу компилятора - обязательно заполнять
            if (object.contains("path")) {
                compiler->path = object["path"].toString();
            } else {
                return false;
            }

            //ru Командная строка, которая будет передана компилятору - обязательно заполнять
            if (object.contains("command_line")) {
                compiler->command_line = object["command_line"].toString();
            } else {
                return false;
            }

            //ru Расширение целевого проекта - обязательно заполнять
            if (object.contains("ext")) {
                compiler->ext = object["ext"].toString();
            } else {
                return false;
            }
        }

        //!!!ru Получаем информацию о проектах !!!
        QJsonArray projects = infoPackage["projects"].toArray();
        for (auto json : projects) {
            QJsonObject object = json.toObject();

            //ru Создаём объект проекта и заполняем его.
            SharedProjectInfo project = SharedProjectInfo::create();

            //ru Добавляем проект в массив проектов текущего пакета.
            m_projectList.append(project);

            //ru Название проекта - обязательно заполнять.
            if (object.contains("name")) {
                project->name = object["name"].toString();
            } else {
                return false;
            }

            //ru Короткое описание проекта - необязательно заполнять
            if (object.contains("shortDescription")) {
                project->shortDescription = object["shortDescription"].toString();
            } else {
                return false;
            }

            //ru Полное описание проекта - необязательно заполнять
            if (object.contains("description")) {
                project->description = object["description"].toString();
            } else {
                return false;
            }

            //ru Название начального элемента - обязательно заполнять
            if (object.contains("entryElement")) {
                project->entryElement = object["entryElement"].toString();
            } else {
                return false;
            }

            //ru Расширение целевого файла - обязательно заполнять
            if (object.contains("ext")) {
                project->ext = object["ext"].toString();
            } else {
                return false;
            }

            //ru Имя make библиотеки - обязательно заполнять
            if (object.contains("make")) {
                project->make = object["make"].toString();
            } else {
                return false;
            }
        }

        return true;
    }

    return false;
}

bool Package::loadElements()
{
    QDir elementsPath(m_confPath);
    elementsPath.setFilter(QDir::Files);
    QFileInfoList listElements = elementsPath.entryInfoList();

    for (const QFileInfo &elInfo : listElements) {
        if (m_confElementMap.contains(elInfo.baseName()))
            continue;

        ConfElement *e = new ConfElement(elInfo, this);
        if (e->load())
            m_confElementMap.insert(e->getName(), e);
        else
            return false;
    }

    return true;
}

ConfElement *Package::loadElement(const QString &name)
{
    QString elementConfPath = m_confPath + QDir::separator() + name + ".ini";
    if (contains(name))
        return m_confElementMap[name];
    else {
        ConfElement *e = new ConfElement(elementConfPath, this);
        if (e->load()) {
            m_confElementMap.insert(e->getName(), e);
            return e;
        }
    }
    return nullptr;
}

bool Package::contains(const QString &name)
{
    return m_confElementMap.contains(name);
}

ConfElement *Package::getElementByName(const QString &name)
{
    if (contains(name))
        return m_confElementMap[name];

    return nullptr;
}
