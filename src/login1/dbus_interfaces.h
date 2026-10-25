#pragma once

#include <qlatin1stringview.h>
#include <qstring.h>
#include <qstringfwd.h>

namespace extrashell::login1 {
    
using namespace Qt::StringLiterals;

inline constexpr QLatin1StringView LOGIN1_SERVICE = "org.freedesktop.login1"_L1;

inline constexpr QLatin1StringView LOGIN1_OBJECT = "/org/freedesktop/login1"_L1;

} // namespace extrashell::udisks2