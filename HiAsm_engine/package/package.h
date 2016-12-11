#pragma once

//Project
#include "types.h"

//STL

//Native

//Qt
#include <QObject>

class Package : public QObject
{
    Q_OBJECT

private:
    const QString c_INFO_FILE_NAME = "package";
    const QString c_CONF_DIR = "conf";
    const QString c_ICON_DIR = "icon";

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
    bool m_isSuccess = false;   //Информация успешно загружена

    //Compiler
    CompilerList m_compilerList; //Массив компиляторов

    //Project
    ProjectList m_projectList; //Массив проектов

    //Elements
    ConfElementMap m_confElementMap; //Массив элементов

public:
    explicit Package(const QString &packagePath, QObject *parent = 0);

private:
    bool loadPackageInfo();
    bool loadElements();

public:
    bool isSuccess() const { return m_isSuccess; }
    ConfElement *loadElement(const QString &name);
    bool contains(const QString &name);
    ConfElement *getElementByName(const QString &name);
};
