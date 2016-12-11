#pragma once
//Project
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QVariant>

typedef QVector<class ConfElement *> ConfElementList;
typedef QMap<QString, class ConfElement *> ConfElementMap;

struct ConfArrayItem {
    QString name;
    QVariant data;
};
typedef QSharedPointer<ConfArrayItem> SharedConfArrayItem;
Q_DECLARE_METATYPE(SharedConfArrayItem)

class ConfArrayValue : public QList<SharedConfArrayItem>
{
private:
    DataType m_type = data_null;

public:
    explicit ConfArrayValue(DataType type)
        : m_type(type)
    {
    }

    DataType getType() const { return m_type; }
    void setType(DataType type) { m_type = type; }
};
typedef QSharedPointer<ConfArrayValue> SharedConfArrayValue;
Q_DECLARE_METATYPE(SharedConfArrayValue)

struct ConfLinkedElementInfo {
    class Element *element{};
    QString interface;
};
typedef QSharedPointer<ConfLinkedElementInfo> SharedConfLinkedElementInfo;
Q_DECLARE_METATYPE(SharedConfLinkedElementInfo)

struct ConfValueFont { //-V802
    QString name;
    int size{};
    int color{};
    uchar style{};
    uchar charset{};

};
typedef QSharedPointer<ConfValueFont> SharedConfValueFont;
Q_DECLARE_METATYPE(SharedConfValueFont)

class ConfValue : public QVariant
{
    //Self
    DataType m_type = data_null;

public:
    //Self
    void setType(DataType type);
    DataType getType() const;

    //Byte
    uchar toByte() const;

    //Int
    int toInt() const;

    //Real
    qreal toReal() const;

    //String
    QString toString() const;

    //Array
    DataType getTypeArrayItem() const;
    int getArraySize() const;
    SharedConfArrayItem getArrayItemByIndex(int index) const;
    QString getArrayItemName(int index) const;

    //Font
    SharedConfValueFont toFont() const;

    //LinkedElementInfo
    SharedConfLinkedElementInfo toLinkedElementInfo() const;
};

// Конфиг
struct ConfPoint {
    /// Название точки.
    /// %prop% - название свойства, значение которого будет использоваться в имени точки.
    QString name;

    /// Описание точки.
    QString desc;

    /// Тип точки.
    PointType pointType;

    /// Тип данных точки.
    DataType dataType;
};
typedef QSharedPointer<ConfPoint> SharedConfPoint;
typedef QList<SharedConfPoint> ConfPointList;

struct ConfProp { //-V802
    QString name;
    QString desc;
    ConfValue value;
    int defLine{};
    QString group;
    DataType type{};
    bool makePoint = false;
    bool activated = false;
};
typedef QSharedPointer<ConfProp> SharedConfProp;
typedef QList<SharedConfProp> ConfPropList;
typedef QMap<QString, QString> MapConfPropGroup;

struct ProjectInfo {
    QString name;
    QString shortDescription;
    QString description;
    QString entryElement;
    QString ext;
    QString make;
};
typedef QSharedPointer<ProjectInfo> SharedProjectInfo;
typedef QList<SharedProjectInfo> ProjectList;

struct CompilerInfo {
    QString name;
    QString path;
    QString command_line;
    QString ext;
};
typedef QSharedPointer<CompilerInfo> SharedCompilerInfo;
typedef QList<SharedCompilerInfo> CompilerList;
typedef QMap<QString, class Package *> Packages;
