#pragma once

//Project
#include "core/types.h"
#include "confelement.h"

//STL

//Native

//Qt
#include <QtCore>

namespace PackageInfo
{
static const QString INFO_FILE_NAME = "package";
static const QString CONF_DIR = "conf";
static const QString ICON_DIR = "icon";
}

class Package : public QObject
{
    Q_OBJECT

private:
    //Информация о пакете
    QString m_packagePath;      //Путь к пакету
    QString m_packageFileInfo;  //Путь к файлу информации о пакете
    QString m_confPath;         //Путь к каталогу с конфигурациями элементов
    QString m_name;             //Название пакета
    QString m_shortDescription; //Короткое описание пакета
    QString m_description;      //Полное описание пакета
    bool m_visible = true;      //Видимость пакета
    bool m_base = false;        //Базовый пакет
    bool m_success = false;     //Информация успешно загружена

    //Compiler
    CompilerList m_compilerList; //Массив компиляторов

    //Project
    ProjectList m_projectList; //Массив проектов

    //Compiler
    ConfElementList m_confElementList; //Массив компиляторов

public:
    explicit Package(const QString &packagePath, QObject *parent = 0);

private:
    bool loadPackageInfo();
    bool loadElements();
    void addInheritElements();

public:
    bool getSuccess() const;
    void setSuccess(bool success);

    //Element
    ConfElement *getElementByName(const QString &name);
};
