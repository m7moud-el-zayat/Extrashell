#include <qdbusabstractinterface.h>
#include <qdbusextratypes.h>
#include <qdbusmetatype.h>
#include <qdbuspendingcall.h>
#include <qdbuspendingreply.h>

#include "properties.h"

#include "properties_interface.h"

Q_LOGGING_CATEGORY(logDBusProperties, "extrashell.dbus.properties", QtWarningMsg)

namespace extrashell::dbus {

bool demarshallVariant(const QVariant& variant, const QMetaType& type, void* data) {
    const char* expectedSignature = QDBusMetaType::typeToSignature(type);

    if (!expectedSignature) {
        qCWarning(logDBusProperties).noquote()
            << "Failed to demarshall" << variant << "as target type" << type
            << "is not registered with QDBusMetaType.";

        return false;
    }

    const QMetaType variantType = variant.metaType();

    if (variantType == type) {
        if (!type.isCopyConstructible()) {
            qCWarning(logDBusProperties).noquote()
                << "Failed to demarshall" << variant << "into target type" << type
                << "as the target type is not copy-constructible.";

            return false;
        }

        type.construct(data, variant.constData());

        return true;
    }

    static const QMetaType dbusArgType = QMetaType::fromType<QDBusArgument>();

    if (variantType == dbusArgType) {
        const auto& dbusArg = variant.value<QDBusArgument>();
        const auto dbusArgSignature = dbusArg.currentSignature();

        if (dbusArgSignature != expectedSignature) {
            qCWarning(logDBusProperties).noquote()
                << "Failed to demarshall QDBusArgument: signature" << dbusArgSignature
                << "does not match target type" << type
                << "(expected signature:" << expectedSignature << ").";

            return false;
        }

        if (!type.isDefaultConstructible()) {
            qCWarning(logDBusProperties).noquote()
                << "Failed to demarshall" << variant << "into target type" << type
                << "as the target type is not default-constructible.";

            return false;
        }

        type.construct(data);

        if (!QDBusMetaType::demarshall(dbusArg, type, data)) {
            qCWarning(logDBusProperties).noquote()
                << "Failed to demarshall QDBusArgument with signature" << dbusArgSignature
                << "into target type" << type;

            type.destruct(data);

            return false;
        }

        return true;
    }

    qCWarning(logDBusProperties)
        << "Failed to demarshall" << variant << "as target type" << type
        << "(expected D-Bus signature:" << expectedSignature << ").";

    return false;
}

PropertiesGroup::PropertiesGroup(QObject* parent) : QObject(parent) {}

void PropertiesGroup::initialize(QDBusAbstractInterface* iface) {
    if (!iface->connection().isConnected()) {
        qCWarning(logDBusProperties).noquote()
            << "Failed to initialize PropertiesGroup for" << iface->path() << "in" << (iface->service() + ':')
            << "D-Bus connection" << iface->connection().name() << "is not connected.";

        return;
    }

    m_iface = iface;
    m_properties_iface = new DBusPropertiesInterface(iface->service(), iface->path(), iface->connection(), this);

    if (!isValid()) {
        qCWarning(logDBusProperties).noquote()
            << "Failed to connect PropertiesChanged signal for" << iface->path() << "in" << (iface->service() + ':')
            << "D-Bus properties interface is invalid.";

        return;
    }

    connect(m_properties_iface, &DBusPropertiesInterface::PropertiesChanged, this, &PropertiesGroup::onPropertiesChanged);
}

void PropertiesGroup::initAllFromObjectManager(const QVariantHash& initials) {
    if (initials.isEmpty()) {
        qCWarning(logDBusProperties).noquote()
            << "Failed to initialize all registered properties for" << m_iface->path() << "in" << (m_iface->service() + ':')
            << "D-Bus ObjectManager interface is invalid.";

        return;
    }

    setFromHash(initials);
}

void PropertiesGroup::initAllViaGetAll() {
    if (!isValid()) {
        qCWarning(logDBusProperties).noquote()
            << "Failed to initialize all registered properties for" << m_iface->path() << "in" << (m_iface->service() + ':')
            << "D-Bus properties interface is invalid.";

        return;
    }

    auto* watcher = new QDBusPendingCallWatcher(m_properties_iface->GetAll(m_iface->interface()), this);

    auto onFinished = [this](QDBusPendingCallWatcher* watcher) {
        QDBusPendingReply<QVariantHash> reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            qCWarning(logDBusProperties).noquote()
                << "Failed to initialize all registered properties for" << m_iface->path() << "in" << (m_iface->service() + ':')
                << reply.error().message();

            return;
        }

        setFromHash(reply.value());
    };

    connect(watcher, &QDBusPendingCallWatcher::finished, this, onFinished);
}

void PropertiesGroup::getPropertyViaGet(const QString& key) {
    if (!m_properties.contains(key))
        return;

    if (!isValid()) {
        qCWarning(logDBusProperties).noquote()
            << "Failed to get" << key << "for" << m_iface->path() << "in" << (m_iface->service() + ':')
            << "D-Bus properties interface is invalid.";

        return;
    }

    auto* watcher = new QDBusPendingCallWatcher(m_properties_iface->Get(m_iface->interface(), key), this);

    auto onFinished = [key, this](QDBusPendingCallWatcher* watcher) {
        QDBusPendingReply<QDBusVariant> reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            qCWarning(logDBusProperties).noquote()
                << "Failed to get" << key << "for" << m_iface->path() << "in" << (m_iface->service() + ':')
                << reply.error().message();

            return;
        }

        if (auto it = m_properties.constFind(key); it != m_properties.cend())
            (*it)->set(reply.value().variant(), true);
    };

    connect(watcher, &QDBusPendingCallWatcher::finished, this, onFinished);
}

void PropertiesGroup::setPropertyViaSet(const QString& key, const QVariant& value) {
    if (!m_properties.contains(key))
        return;

    if (!isValid()) {
        qCWarning(logDBusProperties).noquote()
            << "Failed to set" << key << "for" << m_iface->path() << "in" << (m_iface->service() + ':')
            << "D-Bus properties interface is invalid.";

        return;
    }

    auto* watcher = new QDBusPendingCallWatcher(m_properties_iface->Set(m_iface->interface(), key, QDBusVariant(value)), this);

    auto onFinished = [=, this](QDBusPendingCallWatcher* watcher) {
        QDBusPendingReply<> reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            qCWarning(logDBusProperties).noquote()
                << "Failed to set" << key << "for" << m_iface->path() << "in" << (m_iface->service() + ':')
                << reply.error().message();

            return;
        }

        if (auto it = m_properties.constFind(key); it != m_properties.cend())
            (*it)->set(value, true);
    };

    connect(watcher, &QDBusPendingCallWatcher::finished, this, onFinished);
}

void PropertiesGroup::onPropertiesChanged(const QString& iface_name, const QVariantHash& changed, const QStringList& invalidated) {
    if (iface_name != m_iface->interface())
        return;

    setFromHash(changed, true);
    updateFromList(invalidated);
}

void PropertiesGroup::registerProperty(QLatin1StringView key, PropertyBase* property) {
    if (m_properties.contains(key))
        qCFatal(logDBusProperties).noquote()
            << "Duplicate key" << key << "registration for" << m_iface->path() << "in" << (m_iface->service() + ':');

    m_properties.insert(key, property);
}

void PropertiesGroup::setFromHash(const QVariantHash& hash, bool isUpdate) {
    Qt::beginPropertyUpdateGroup();

    for (const auto& [key, value] : hash.asKeyValueRange()) {
        if (auto it = m_properties.constFind(key); it != m_properties.cend())
            (*it)->set(value, isUpdate);
    }

    Qt::endPropertyUpdateGroup();
}

void PropertiesGroup::updateFromList(const QStringList& list) {
    for (const auto& key : list)
        getPropertyViaGet(key);
}

bool PropertiesGroup::isValid() const { return m_properties_iface && m_properties_iface->isValid(); }

} // namespace extrashell::dbus

#include "moc_properties.cpp"