#pragma once

#include <qdbuspendingcall.h>
#include <qdbuspendingreply.h>
#include <qdebug.h>
#include <qjsengine.h>
#include <qlogging.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qvariant.h>

#include <functional>
#include <utility>

class QDBusAbstractInterface;

namespace extrashell::dbus {

bool isInterfaceValid(
    const QLoggingCategory& loggingCategory,
    QDBusAbstractInterface* iface,
    const QString& name
);

template <typename LoggingCategoryFn, typename Interface, typename Call, typename... Args>
void callAsync(
    LoggingCategoryFn loggingCategoryFn,
    Interface* iface,
    const QString& name,
    Call call,
    Args&&... args
) {
    if (!isInterfaceValid(loggingCategoryFn(), iface, name))
        return;

    auto* watcher = new QDBusPendingCallWatcher((iface->*call)(std::forward<Args>(args)...), iface);

    auto onFinished = [=](QDBusPendingCallWatcher* watcher) {
        QDBusPendingReply<> reply = *watcher;
        watcher->deleteLater();

        if (reply.isError())
            qCWarning(loggingCategoryFn).noquote()
                << "Failed to call method" << name << "for" << iface->path() << "in" << (iface->interface() + ':')
                << reply.error().message();
    };

    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, iface, onFinished);
}

class AsyncCall : public QObject {
    Q_OBJECT

  public:
    template <typename LoggingCategoryFn, typename Interface, typename Call, typename... Args>
    AsyncCall(
        LoggingCategoryFn loggingCategoryFn,
        Interface* iface,
        const QString& name,
        Call call,
        Args&&... args
    ) : QObject(nullptr) {
        if (!isInterfaceValid(loggingCategoryFn(), iface, name))
            return;
        
        auto result = std::invoke(call, iface, std::forward<Args>(args)...);
        auto* watcher = new QDBusPendingCallWatcher(result, iface);
        
        using ResultType = decltype(result);

        auto onFinished = [=, this](QDBusPendingCallWatcher* watcher) {
            ResultType reply = *watcher;
            watcher->deleteLater();

            if (reply.isError()) {
                qCWarning(loggingCategoryFn).noquote()
                    << "Failed to call method" << name << "for" << iface->path() << "in" << (iface->interface() + ':')
                    << reply.error().message();

                return;
            }

            emit finished(QVariant::fromValue(reply.value()));
        };

        connect(watcher, &QDBusPendingCallWatcher::finished, iface, onFinished);
        
        QJSEngine::setObjectOwnership(this, QJSEngine::JavaScriptOwnership);
    }

  signals:
    void finished(const QVariant& value);
};

} // namespace extrashell::dbus