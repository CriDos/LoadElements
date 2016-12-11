//Project
#include "types.h"
#include "cgt/CGTShare.h"

//STL

//Qt

void ConfValue::setType(DataType type)
{
    m_type = type;
}

DataType ConfValue::getType() const
{
    return m_type;
}

uchar ConfValue::toByte() const
{
    if (!canConvert<uchar>())
        return uchar();

    return value<uchar>();
}

int ConfValue::toInt() const
{
    if (!canConvert<int>())
        return int();

    return value<int>();
}

qreal ConfValue::toReal() const
{
    if (!canConvert<qreal>())
        return qreal();

    return value<qreal>();
}

QString ConfValue::toString() const
{
    if (!canConvert<QString>())
        return QString();

    return value<QString>();
}

DataType ConfValue::getTypeArrayItem() const
{
    if (!canConvert<SharedConfArrayValue>())
        return data_null;

    return value<SharedConfArrayValue>()->getType();
}

int ConfValue::getArraySize() const
{
    if (!canConvert<SharedConfArrayValue>())
        return -1;

    return value<SharedConfArrayValue>()->size();
}

SharedConfArrayItem ConfValue::getArrayItemByIndex(int index) const
{
    if (!canConvert<SharedConfArrayValue>())
        return SharedConfArrayItem();

    const SharedConfArrayValue &arrayValues = value<SharedConfArrayValue>();
    if (index < arrayValues->size())
        return arrayValues->at(index);

    return SharedConfArrayItem();
}

QString ConfValue::getArrayItemName(int index) const
{
    const SharedConfArrayItem arrItem = getArrayItemByIndex(index);
    if (!arrItem)
        return QString();

    return arrItem->name;
}

SharedConfValueFont ConfValue::toFont() const
{
    if (!canConvert<SharedConfValueFont>())
        return SharedConfValueFont();

    return value<SharedConfValueFont>();
}

SharedConfLinkedElementInfo ConfValue::toLinkedElementInfo() const
{
    if (!canConvert<SharedConfLinkedElementInfo>())
        return SharedConfLinkedElementInfo();

    return value<SharedConfLinkedElementInfo>();
}
