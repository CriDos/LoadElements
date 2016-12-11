//Project
#include "confelement.h"
#include "package.h"
#include "cgt/CGTShare.h"
#include "core/types.h"

//STL

//Native

//Qt

ConfElement::ConfElement(const QFileInfo &elInfo, QObject *parent)
    : QObject(parent)
{
    m_confPath = elInfo.absoluteFilePath();
    m_name = elInfo.baseName();
}

void ConfElement::loadConf()
{
    QFile file(m_confPath);
    file.open(QIODevice::ReadOnly);
    const QString data = QString::fromLocal8Bit(file.readAll());
    file.close();

    TypeSection section = ts_undefine;
    QStringList secAbouts;
    QStringList secTypes;
    QStringList secProperties;
    QStringList secPoints;

    for (QString line : data.split("\r\n")) {
        line = line.trimmed();
        int size = line.size();
        if (size < 2)
            continue;

        if (line.at(0) == QLatin1Char('[')) {
            const QChar c = line.at(1).toLower();

            if (c == QLatin1Char('a')) {
                section = ts_abouts;
            } else if (c == QLatin1Char('t')) {
                section = ts_types;
            } else if (c == QLatin1Char('e')) {
                section = ts_edit;
            } else if (c == QLatin1Char('p')) {
                section = ts_properties;
            } else if (c == QLatin1Char('m')) {
                section = ts_points;
            } else {
                section = ts_undefine;
            }

            continue;
        }

        switch (section) {
        case ts_abouts:
            secAbouts << line;
            break;
        case ts_types:
            secTypes << line;
            break;
        case ts_edit: {
            if (!m_editClass.isEmpty())
                continue;
            if (line.section('=', 0, 0).toLower() == QLatin1String("class"))
                m_editClass = line.section('=', 1, 1);
            break;
        }
        case ts_properties:
            secProperties << line;
            break;
        case ts_points:
            secPoints << line;
            break;
        default: {
            qWarning(qUtf8Printable("При загрузке конфигурации элемента, найден неизвестный параметр."));
            qWarning("Файл: \"%s\"", qUtf8Printable(m_confPath));
            qWarning("Параметр: \"%s\"", qUtf8Printable(line));
            break;
        }
        }
    }

    parseAbout(secAbouts);
    parseTypes(secTypes);
    parseProperties(secProperties);
    parsePoints(secPoints);

    m_isSuccess = true;
}

void ConfElement::parseAbout(const QStringList &list)
{
    for (const QString &line : list) {
        const QString &name = line.section(QLatin1Char('='), 0, 0).toLower();
        const QString &value = line.section(QLatin1Char('='), 1, 1).toLower();

        if (name == QLatin1String("version")) {
            m_version = value;

        } else if (name == QLatin1String("author")) {
            m_author = value;

        } else if (name == QLatin1String("mail")) {
            m_mail = value;
        }
    }
}

void ConfElement::parseTypes(const QStringList &list)
{
    for (const QString &line : list) {
        QString sec0 = line.section("=", 0, 0).toLower();
        QString sec1 = line.section("=", 1, 1);
        if (sec0 == QLatin1String("class")) {
            m_class = ElementClassString.value(sec1);
        } else if (sec0 == QLatin1String("inherit")) {
            loadInheritElements(sec1);
        } else if (sec0 == QLatin1String("sub")) {
            m_sub = sec1;
        } else if (sec0 == QLatin1String("info")) {
            m_info = sec1;
        } else if (sec0 == QLatin1String("icon")) {
            m_propIcon = sec1;
        } else if (sec0 == QLatin1String("view")) {
            m_view = sec1;
        } else if (sec0 == QLatin1String("tab")) {
            m_tab = sec1;
        } else if (sec0 == QLatin1String("interfaces")) {
            m_interfaces = sec1;
        }
    }
}

void ConfElement::parseProperties(const QStringList &list)
{
    //##имя_группы=описание
    //## закрываем группу

    //+ activated - свойсво по-умолчанию, т.е. открывается редактор
    //при двойном клике.

    //@ makePoint - Позволяет активировать свойство в виде метода с префиксом do*

    //PropGroup
    QString nameGroup;

    //Parser
    bool beginGroup = false;

    for (const QString &line : list) {
        //ConfProp
        QString name;
        QString desc;
        bool makePoint = false;
        bool activated = false;

        //PropGroup
        QString descGroup;

        //Parser
        bool equalSign = false;
        bool beginGroupLine = false;
        uchar countSharp = 0;

        int outIndex = line.size();
        for (int i = 0; i <= outIndex; ++i) {
            if (i == outIndex) {
                if (beginGroupLine) {
                    m_group.insert(nameGroup, descGroup);
                    continue;
                }
                if (countSharp == 2) {
                    if (beginGroup) {
                        beginGroup = false;
                        nameGroup.clear();
                        continue;
                    }
                }
                break;
            }
            const QChar &c = line[i];

            if (i < 2) {
                if (c == QLatin1Char('#')) {
                    ++countSharp;
                    continue;
                } else if (c == QLatin1Char('@')) {
                    makePoint = true;
                    continue;
                } else if (c == QLatin1Char('+')) {
                    activated = true;
                    continue;
                }
            }

            if (countSharp == 2) {
                if (!beginGroup) {
                    beginGroup = true;
                    beginGroupLine = true;
                }
            }

            if (c == QLatin1Char('=')) {
                equalSign = true;
                continue;
            }

            if (beginGroupLine) {
                if (!equalSign)
                    nameGroup += c; //Имя группы
                else
                    descGroup += c; //Описание

                continue;
            }

            if (!equalSign) {
                name += c; //Имя свойства
                continue;
            }

            if (c == QLatin1Char('|')) {
                const SharedConfProp inheritProp = findInheritProp(name);
                SharedConfProp propConf = parsePropValue(line.right(outIndex - i - 1), inheritProp);
                propConf->name = name;
                propConf->activated = activated;
                propConf->makePoint = makePoint;

                if ((desc.isEmpty() || desc == QLatin1String(" ")) && !inheritProp.isNull())
                    desc = inheritProp->desc;
                propConf->desc = desc;

                if (beginGroup)
                    propConf->group = nameGroup;

                //m_propList << prop;
                break;
            }

            desc += c; //Описание свойства
        }
    }
}

void ConfElement::parsePoints(const QStringList &list)
{
    //* - скрытая точка
    for (const QString &line : list) {
        //ConfPoint
        QString name;
        QString desc;
        QString dataType;
        QString pointType;
        bool hidden = false;

        //Parser
        bool propBegin = false;
        bool propEnd = false;
        bool equalSign = false;
        uchar countPipe = 0;

        const int outIndex = line.size();
        for (int i = 0; i <= outIndex; ++i) {
            if (i == outIndex) {
                SharedConfPoint point = SharedConfPoint::create();
                point->name = name;
                point->desc = desc;
                point->pointType = PointType(pointType.toInt());
                point->dataType = DataType(dataType.toInt());

                if (hidden)
                    m_hiddenPointList << point;
                else
                    m_pointList << point;

                break;
            }
            const QChar &c = line[i];

            if (i == 0 && c == QLatin1Char('*')) {
                hidden = true;
                continue;
            }
            if (c == QLatin1Char('%')) {
                if (!propBegin)
                    propBegin = true;
                else
                    propEnd = true;

                name += c;
                continue;
            }
            if (c == QLatin1Char('=')) {
                equalSign = true;
                continue;
            }
            if (c == QLatin1Char('|')) {
                ++countPipe;
                continue;
            }
            if (propBegin && !propEnd) {
                name += c;
                continue;
            }
            if (countPipe) {
                if (countPipe == 1) //Тип точки
                    pointType += c;
                if (countPipe == 2) //Тип данных
                    dataType += c;

                continue;
            }
            if (!equalSign)
                name += c; //Имя точки
            else
                desc += c; //Описание точки
        }
    }
}

SharedConfProp ConfElement::parsePropValue(const QString &sline, const SharedConfProp &inheritPropConf)
{
    const QString notImplemented = QString("Загрузка свойств с типом %1 не реализована.");
    SharedConfProp propConf = SharedConfProp::create();
    ConfValue &value = propConf->value;

    if (sline.isEmpty())
        return propConf;

    QStringList list = sline.split(QLatin1Char('|'));
    if (list.isEmpty())
        return propConf;

    DataType type = data_null;
    const QString &typeValue = list[0];
    if (typeValue.isEmpty() || typeValue == QLatin1String(" ")) {
        for (const ConfElement *elConf : m_inheritList) {
            if (elConf->containsProp(propConf->name)) {
                const SharedConfProp &prop = elConf->getPropByName(propConf->name);
                type = prop->type;
            } else
                type = DataType(list[0].toInt());
        }
    } else {
        type = DataType(list[0].toInt());
    }

    propConf->value.setType(type);
    list.removeFirst();

    switch (type) {
    case data_null:
        break;
    case data_int: {
        if (!list.isEmpty())
            propConf->value.setValue(list[0].toInt());
        else
            propConf->value.setValue(0);
        break;
    }
    case data_real: {
        double dr = 0.0;
        if (!list.isEmpty())
            dr = list.first().toDouble();
        propConf->value.setValue(dr);
        break;
    }
    case data_str: {
        QString ds;
        if (!list.isEmpty())
            ds = list.first();
        propConf->value.setValue(ds);
        break;
    }
    case data_combo:
    case data_comboEx: {
        if (list.size() < 2) {
            qWarning() << "К-во параметров свойства меньше двух.";
            break;
        }
        propConf->defLine = list.first().toInt();
        propConf->value.setValue(list.last().split(','));
        break;
    }

    case data_color: {
        QString ds;
        if (!list.isEmpty())
            ds = list.first();
        propConf->value.setValue(ds);
        break;
    }

    case data_font: {
        auto font = SharedConfValueFont::create();
        if (list.isEmpty()) {
            font->name = "MS Sans Serif";
            font->size = 8;
            font->color = 0;
            font->style = 0;
            font->charset = 1;

        } else {
            QStringList fontParams = list[0].split(QLatin1Char(','));
            if (fontParams.count() >= 5) {
                font->name = fontParams[0].trimmed();
                font->size = fontParams[1].toInt();
                font->color = fontParams[2].toInt();
                font->style = static_cast<uchar>(fontParams[3].toInt());
                font->charset = static_cast<uchar>(fontParams[4].toInt());
            }
        }

        value.setValue(font);
        break;
    }
    case data_flags:
        if (list.size() < 2) {
            qWarning() << "К-во параметров свойства меньше двух.";
            break;
        }

        propConf->defLine = list[0].toInt();
        propConf->value.setValue(list[1].split(QLatin1Char(',')));
        break;
    case data_jpeg:
    case data_icon:
    case data_bitmap:
    case data_stream:
    case data_wave:
    case data_array:
    case data_list:
    case data_data:
        break;

    case data_script:
    case data_element:
    //qInfo() << "test";
    //break;
    case data_matr:

    case data_menu:

    case data_code:

    case data_object:
        qWarning().noquote() << notImplemented.arg(DataTypeMap[type]);
        //qWarning().noquote() << notImplemented.arg(DataTypeMap[_propType]);
    }

    return propConf;
}

void ConfElement::loadInheritElements(const QString &sec)
{
    m_inheritList.clear();
    Package *pack = parent();
    const QStringList inheritList = sec.split(QLatin1Char(','), QString::SkipEmptyParts);
    for (const QString &name : inheritList) {
        ConfElement *e = nullptr;

        if (!pack->contains(name))
            e = pack->loadElement(name);
        else
            e = pack->getElementByName(name);

        if (e->isSuccess() && !m_inheritList.contains(e)) {
            m_inheritList.append(e);
            // e->propList();
        }
    }
}

SharedConfProp ConfElement::findInheritProp(const QString &name) const
{
    for (const ConfElement *i : m_inheritList) {
        if (i->containsProp(name)) {
            return i->getPropByName(name);
        }
    }

    return SharedConfProp();
}

SharedConfProp ConfElement::getPropByName(const QString &name) const
{
    for (const SharedConfProp &prop : m_propList) {
        if (prop->name == name) {
            return prop;
        }
    }

    return SharedConfProp();
}

bool ConfElement::containsProp(const QString &name) const
{
    for (const SharedConfProp &prop : m_propList) {
        if (prop->name == name) {
            return true;
        }
    }
    return false;
}

ConfPropList ConfElement::propList() const
{
    return m_propList;
}

bool ConfElement::load()
{
    if (!m_loaded) {
        loadConf();
    }

    return true;
}
