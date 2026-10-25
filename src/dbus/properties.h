#pragma once

#include <qcompilerdetection.h>
#include <qcontainerfwd.h>
#include <qdbusargument.h>
#include <qhash.h>
#include <qlatin1stringview.h>
#include <qlogging.h>
#include <qloggingcategory.h>
#include <qmetatype.h>
#include <qobject.h>
#include <qoverload.h>
#include <qproperty.h>
#include <qstring.h>
#include <qstringfwd.h>
#include <qtclasshelpermacros.h>
#include <qtmetamacros.h>
#include <qvariant.h>

#include <cstddef>
#include <memory>
#include <utility>

#include "../core/utils.h"

Q_DECLARE_LOGGING_CATEGORY(logDBusProperties)

class QDBusAbstractInterface;
class DBusPropertiesInterface;

namespace extrashell::dbus {

bool demarshallVariant(const QVariant& variant, const QMetaType& type, void* data);

class PropertiesGroup;

struct PropertyBase {
    PropertyBase() = default;
    Q_DISABLE_COPY_MOVE(PropertyBase)
    virtual ~PropertyBase() = default;

    virtual void set(const QVariant& variant, bool isUpdate) = 0;
};

template <
    typename Class,
    typename PropertyType,
    auto Offset,
    PropertyType Class::*property,
    utils::Utf8StringLiteral Key,
    PropertiesGroup Class::*group
>
class Property : public PropertyBase {
    using Type = utils::PropertyTraits<PropertyType>::value_type;

  public:
    Property() { (owner()->*group).registerProperty(Key, this); }

    void set(const QVariant& variant, bool isUpdate) override {
        static constexpr QMetaType expectedType = QMetaType::fromType<Type>();

        alignas(Type) std::byte storage[sizeof(Type)];
        void* memory = static_cast<void*>(storage);
        
        if (!demarshallVariant(variant, expectedType, memory))
            return;
    
        auto* parent = owner();
        auto& target = parent->*property;
        auto* value = static_cast<Type*>(memory);

        target = std::move(*value);

        std::destroy_at(value);

        qCDebug(logDBusProperties).noquote()
            << QLatin1StringView(Key) << "in" << parent << (isUpdate ? "updated" : "initialized")
            << "to" << utils::property_value(target);

    }

  private:
    Class* owner() { return reinterpret_cast<Class*>(reinterpret_cast<std::byte*>(this) - Offset()); }
};

class PropertiesGroup : public QObject {
    Q_OBJECT

    template <
        typename Class,
        typename PropertyType,
        auto Offset,
        PropertyType Class::*property,
        utils::Utf8StringLiteral Key,
        PropertiesGroup Class::*group
    >
    friend class Property;

  public:
    explicit PropertiesGroup(QObject* parent = nullptr);
    
    void initialize(QDBusAbstractInterface* iface);

    void initAllFromObjectManager(const QVariantHash& initials);
    void initAllViaGetAll();

    void getPropertyViaGet(const QString& key);
    void setPropertyViaSet(const QString& key, const QVariant& value);
    
  private slots:
    void onPropertiesChanged(const QString& iface_name, const QVariantHash& changed, const QStringList& invalidated);

  private:
    QHash<QLatin1StringView, PropertyBase*> m_properties;

    QDBusAbstractInterface* m_iface = nullptr;
    DBusPropertiesInterface* m_properties_iface = nullptr;

    void registerProperty(QLatin1StringView key, PropertyBase* property);
    void setFromHash(const QVariantHash& hash, bool isUpdate = false);
    void updateFromList(const QStringList& list);

    bool isValid() const;
};

} // namespace extrashell::dbus

#define ES_DBUS_PROPERTIES_GROUP(group)                                                      \
    dbus::PropertiesGroup group { this };

#define ES_DBUS_CONSTANT_PROPERTY_IMPL(Class, Type, name, key, group)                        \
    static constexpr size_t _es_property_##name##_offset() {                                 \
        return Q_OFFSETOF(Class, _es_##name);                                                \
    }                                                                                        \
    dbus::Property<                                                                          \
        Class,                                                                               \
        Type,                                                                                \
        &Class::_es_property_##name##_offset,                                                \
        &Class::name,                                                                        \
        key,                                                                                 \
        &Class::group                                                                        \
    >                                                                                        \
    _es_##name {};

#define ES_DBUS_CONSTANT_PROPERTY(Class, Type, name, key, group)                             \
    Type name {};                                                                            \
    ES_DBUS_CONSTANT_PROPERTY_IMPL(Class, Type, name, key, group)

#define ES_DBUS_CONSTANT_PROPERTY_WITH_ARGS(Class, Type, name, value, key, group)            \
    Type name = value;                                                                       \
    ES_DBUS_CONSTANT_PROPERTY_IMPL(Class, Type, name, key, group)

#define ES_DBUS_BINDABLE_PROPERTY_IMPL(Class, name, key, group)                              \
    static constexpr size_t _es_property_##name##_offset() {                                 \
        return Q_OFFSETOF(Class, _es_##name);                                                \
    }                                                                                        \
    dbus::Property<                                                                          \
        Class,                                                                               \
        utils::member_value_type_t<decltype(&Class::name)>,                                  \
        &Class::_es_property_##name##_offset,                                                \
        &Class::name,                                                                        \
        key,                                                                                 \
        &Class::group                                                                        \
    >                                                                                        \
    _es_##name {};

#define ES_DBUS_BINDABLE_PROPERTY_5(Class, Type, name, key, group)                           \
    QT_OBJECT_BINDABLE_PROPERTY_3(Class, Type, name)                                         \
    ES_DBUS_BINDABLE_PROPERTY_IMPL(Class, name, key, group)

#define ES_DBUS_BINDABLE_PROPERTY_6(Class, Type, name, Signal, key, group)                   \
    QT_OBJECT_BINDABLE_PROPERTY_4(Class, Type, name, &Class::Signal)                         \
    ES_DBUS_BINDABLE_PROPERTY_IMPL(Class, name, key, group)

#define ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS_6(Class, Type, name, value, key, group)          \
    QT_OBJECT_BINDABLE_PROPERTY_WITH_ARGS_4(Class, Type, name, value)                        \
    ES_DBUS_BINDABLE_PROPERTY_IMPL(Class, name, key, group)

#define ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS_7(Class, Type, name, value, Signal, key, group)  \
    QT_OBJECT_BINDABLE_PROPERTY_WITH_ARGS_5(Class, Type, name, value, &Class::Signal)        \
    ES_DBUS_BINDABLE_PROPERTY_IMPL(Class, name, key, group)

#define ES_DBUS_BINDABLE_PROPERTY(...)                                                       \
    QT_OVERLOADED_MACRO(ES_DBUS_BINDABLE_PROPERTY, __VA_ARGS__)

#define ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS(...)                                             \
    QT_OVERLOADED_MACRO(ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS, __VA_ARGS__)
