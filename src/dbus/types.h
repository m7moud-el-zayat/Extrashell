#pragma once

#include <qdbusextratypes.h>
#include <qhash.h>
#include <qstring.h>
#include <qvariant.h>

using DBusObjectManagerInterfaces = QHash<QString, QVariantHash>;
using DBusObjectManagerObjectPaths = QHash<QDBusObjectPath, DBusObjectManagerInterfaces>;