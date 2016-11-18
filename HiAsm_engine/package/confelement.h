#pragma once
//Project
#include "core/types.h"

//STL

//Native

//Qt
#include <QtCore>

class ConfElement;
typedef QVector<ConfElement *> ConfElementList;

class ConfElement : public QObject
{
private:
    enum TypeSection {
        ts_undefine,
        ts_abouts,
        ts_types,
        ts_edit,
        ts_properties,
        ts_points
    };

private:
    //Self
    QString m_pathConf;
    QString m_name;
    bool m_isSuccess = false;

    //QIcon m_icon;

    //Inherited
    bool m_isInherited = false;

    //About
    QString m_version;
    QString m_author;
    QString m_mail;

    //Type
    ElementClass m_class;
    QStringList m_inherit;
    ConfElementList m_inheritList;
    QString m_sub;
    QString m_info;
    QString m_propIcon;
    QString m_view;
    QString m_tab;
    QString m_interfaces;

    //Edit
    QString m_editClass;

    //Group
    MapPropGroup m_group;

    //Properties
    PropConfList m_propList;

    //Points
    PointConfList m_pointList;
    PointConfList m_hiddenPointList;

public:
    explicit ConfElement(const QString &pathConf, QObject *parent);

private:
    void initConfElement();
    void loadConf();
    void parseAbout(const QStringList &list);
    void parseTypes(const QStringList &list);
    void parsePropValue(const QString &sline, SharedPropConf conf);
    void parseProperties(const QStringList &list);
    void parsePoints(const QStringList &list);

public:
    QString getName() const { return m_name; }

    //Inherits
    void addInheritElements(Package *pack);
    bool isSuccess() const { return m_isSuccess; }
};
