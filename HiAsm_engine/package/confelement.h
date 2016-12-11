#pragma once
//Project
#include "types.h"

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
    MapConfPropGroup m_group;

    //Properties
    ConfPropList m_propList;

    //Points
    ConfPointList m_pointList;
    ConfPointList m_hiddenPointList;

public:
    explicit ConfElement(const QFileInfo &elInfo, QObject *parent);

private:
    void loadConf();
    void parseAbout(const QStringList &list);
    void parseTypes(const QStringList &list);
    void parseProperties(const QStringList &list);
    void parsePoints(const QStringList &list);
    SharedConfProp parsePropValue(const QString &sline, const SharedConfProp &inheritPropConf);
    void loadInheritElements(const QString &sec);
    SharedConfProp findInheritProp(const QString &name) const;

public:
    bool load();
    QString getName() const { return m_name; }
    Package *parent() { return qobject_cast<Package *>(QObject::parent()); }
    bool isSuccess() const { return m_isSuccess; }
    SharedConfProp getPropByName(const QString &name) const;
    bool containsProp(const QString &name) const;
    ConfPropList propList() const;
};
