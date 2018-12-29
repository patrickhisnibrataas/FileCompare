#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>
#include <QMetaEnum>
#include <type_traits>

namespace Enums
{
    Q_NAMESPACE

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    static int length()
    {
        return QMetaEnum::fromType<T>().keyCount();
    }

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    static QString toString(int index)
    {
        return QMetaEnum::fromType<T>().key(index);
    }

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    static QString toString(T enumerator)
    {
        return QMetaEnum::fromType<T>().key(static_cast<int>(enumerator));
    }

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    static QStringList toStringList()
    {
        QStringList stringList;

        const auto length = Enums::length<T>();
        for (auto i = 0; i < length; i++)
            stringList.append(toString<T>(static_cast<T>(i)));

        return stringList;
    }

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    static T toEnum(int index)
    {
        return static_cast<T>(QMetaEnum::fromType<T>().value(index));
    }

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    static T toEnum(const QString& enumString)
    {
        return static_cast<T>(QMetaEnum::fromType<T>().keyToValue(enumString.toStdString().c_str()));
    }

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    static T isValid(int index)
    {
        return static_cast<T>(QMetaEnum::fromType<T>().value(index)) == -1 ? false : true;
    }

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    static T isValid(const QString& enumsString)
    {
        return static_cast<T>(QMetaEnum::fromType<T>().keyToValue(enumsString.toStdString().c_str())) == -1 ? false : true;
    }

    enum class SortBy {
        DateTaken,
        DateModified,
        IosFilename
    };
    Q_ENUM_NS(SortBy)
};

#endif // ENUMS_H
