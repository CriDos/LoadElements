#pragma once
//Project
#include "core/types.h"

//STL

//Native

//Qt
#include <QtCore>

class ConfElement : public QObject
{
public:
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
    QString m_confPath;
    QString m_name;
    bool m_isSuccess = false;
    bool m_loaded = false;

    //QIcon m_icon;

    //About
    QString m_version;
    QString m_author;
    QString m_mail;

    //Type
    ElementClass m_class;
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
    explicit ConfElement(const QFileInfo &elInfo, QObject *parent);

private:
    void loadConf();
    void parseAbout(const QStringList &list);
    void parseTypes(const QStringList &list);
    void parseProperties(const QStringList &list);
    void parsePoints(const QStringList &list);
    void parsePropValue(const QString &sline, SharedPropConf conf);
    SharedPropConf getPropByName(const QString &name) const;

public:
    bool load();
    QString getName() const { return m_name; }
    Package *parent() { return qobject_cast<Package *>(QObject::parent()); }
    bool isSuccess() const { return m_isSuccess; }
};
