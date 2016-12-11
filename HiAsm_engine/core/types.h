#pragma once
//Project
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QVariant>

typedef QVector<class ConfElement *> ConfElementList;
typedef QMap<QString, class ConfElement *> ConfElementMap;

struct ArrayItem {
    QString name;
    QVariant data;
};
typedef QSharedPointer<ArrayItem> SharedArrayItem;
Q_DECLARE_METATYPE(SharedArrayItem)

class ArrayValue : public QList<SharedArrayItem>
{
private:
    DataType m_type = data_null;

public:
    explicit ArrayValue(DataType type)
        : m_type(type)
    {
    }

    DataType getType() const { return m_type; }
    void setType(DataType type) { m_type = type; }
};
typedef QSharedPointer<ArrayValue> SharedArrayValue;
Q_DECLARE_METATYPE(SharedArrayValue)

struct LinkedElementInfo {
    class Element *element{};
    QString interface;
};
typedef QSharedPointer<LinkedElementInfo> SharedLinkedElementInfo;
Q_DECLARE_METATYPE(SharedLinkedElementInfo)

struct ValueFont { //-V802
    QString name;
    int size{};
    int color{};
    uchar style{};
    uchar charset{};

};
typedef QSharedPointer<ValueFont> SharedValueFont;
Q_DECLARE_METATYPE(SharedValueFont)

class Value : public QVariant
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
    SharedArrayItem getArrayItemByIndex(int index) const;
    QString getArrayItemName(int index) const;

    //Font
    SharedValueFont toFont() const;

    //LinkedElementInfo
    SharedLinkedElementInfo toLinkedElementInfo() const;
};

// Конфиг
struct PointConf {
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
typedef QSharedPointer<PointConf> SharedPointConf;
typedef QList<SharedPointConf> PointConfList;

struct PropConf { //-V802
    QString name;
    QString desc;
    Value value;
    int defLine{};
    QString group;
    DataType type{};
    bool makePoint = false;
    bool activated = false;
};
typedef QSharedPointer<PropConf> SharedPropConf;
typedef QList<SharedPropConf> PropConfList;
typedef QMap<QString, QString> MapPropGroup;

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
