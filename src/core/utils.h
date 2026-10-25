#pragma once

#include <qlatin1stringview.h>
#include <qproperty.h>
#include <qqmllist.h>
#include <qstring.h>
#include <qtypes.h>

#include <algorithm>

namespace extrashell::utils {

template <qsizetype Size>
struct Utf8StringLiteral {
    constexpr Utf8StringLiteral(const char (&str)[Size]) {
        std::copy_n(str, Size, data);
    }

    constexpr operator QLatin1StringView() const {
        return {data, Size - 1};
    }

    operator QString() const {
        return QString::fromUtf8(data, Size - 1);
    }

    char data[Size];
};

template <typename T>
struct PropertyTraits {
    static constexpr bool isBindableProperty = false;
    using value_type = T;
};

template <typename Class, typename T, auto Offset, auto Signal>
struct PropertyTraits<QObjectBindableProperty<Class, T, Offset, Signal>> {
    static constexpr bool isBindableProperty = true;
    using value_type = T;
};

template <typename T>
struct member_value_type;

template <typename Class, typename Member>
struct member_value_type<Member Class::*> { using type = Member; };

template <typename T>
using member_value_type_t = typename member_value_type<T>::type;

template <typename T>
decltype(auto) property_value(T& property) {
    if constexpr (PropertyTraits<T>::isBindableProperty) return property.value();
    else return (property);
}

template <typename Element, typename Owner, typename Container>
qsizetype objectCount(QQmlListProperty<Element>* list, Container Owner::*member) {
    return property_value(static_cast<Owner*>(list->object)->*member).size();
}

template <typename Element, typename Owner, typename Container>
Element* objectAt(QQmlListProperty<Element>* list, qsizetype index, Container Owner::*member) {
    return property_value(static_cast<Owner*>(list->object)->*member).at(index);
}

} // namespace extrashell::utils
