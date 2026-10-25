#pragma once

#include <qcontainerfwd.h>
#include <qloggingcategory.h>
#include <qmap.h>
#include <qobject.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <qvariant.h>

#include "../dbus/properties.h"

Q_DECLARE_LOGGING_CATEGORY(logUDisks2DriveAta)

class UDisks2DriveAtaInterface;

namespace extrashell::dbus {

class AsyncCall;

} // namespace extrashell::dbus

namespace extrashell::udisks2 {

class DriveAta : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2DriveAta)
    QML_UNCREATABLE("DriveAta must be acquired from the UDisks2Drive.")

    Q_PROPERTY(QString smartSelftestStatus READ default NOTIFY smartSelftestStatusChanged BINDABLE bindableSmartSelftestStatus)
    Q_PROPERTY(qulonglong smartPowerOnSeconds READ default NOTIFY smartPowerOnSecondsChanged BINDABLE bindableSmartPowerOnSeconds)
    Q_PROPERTY(qulonglong smartUpdated READ default NOTIFY smartUpdatedChanged BINDABLE bindableSmartUpdated)
    Q_PROPERTY(qlonglong smartNumBadSectors READ default NOTIFY smartNumBadSectorsChanged BINDABLE bindableSmartNumBadSectors)
    Q_PROPERTY(double smartTemperature READ default NOTIFY smartTemperatureChanged BINDABLE bindableSmartTemperature)
    Q_PROPERTY(int aamVendorRecommendedValue READ default NOTIFY aamVendorRecommendedValueChanged BINDABLE bindableAamVendorRecommendedValue)
    Q_PROPERTY(int securityEnhancedEraseUnitMinutes READ default NOTIFY securityEnhancedEraseUnitMinutesChanged BINDABLE bindableSecurityEnhancedEraseUnitMinutes)
    Q_PROPERTY(int securityEraseUnitMinutes READ default NOTIFY securityEraseUnitMinutesChanged BINDABLE bindableSecurityEraseUnitMinutes)
    Q_PROPERTY(int smartNumAttributesFailedInThePast READ default NOTIFY smartNumAttributesFailedInThePastChanged BINDABLE bindableSmartNumAttributesFailedInThePast)
    Q_PROPERTY(int smartNumAttributesFailing READ default NOTIFY smartNumAttributesFailingChanged BINDABLE bindableSmartNumAttributesFailing)
    Q_PROPERTY(int smartSelftestPercentRemaining READ default NOTIFY smartSelftestPercentRemainingChanged BINDABLE bindableSmartSelftestPercentRemaining)
    Q_PROPERTY(bool aamEnabled READ default NOTIFY aamEnabledChanged BINDABLE bindableAamEnabled)
    Q_PROPERTY(bool aamSupported READ default NOTIFY aamSupportedChanged BINDABLE bindableAamSupported)
    Q_PROPERTY(bool apmEnabled READ default NOTIFY apmEnabledChanged BINDABLE bindableApmEnabled)
    Q_PROPERTY(bool apmSupported READ default NOTIFY apmSupportedChanged BINDABLE bindableApmSupported)
    Q_PROPERTY(bool pmEnabled READ default NOTIFY pmEnabledChanged BINDABLE bindablePmEnabled)
    Q_PROPERTY(bool pmSupported READ default NOTIFY pmSupportedChanged BINDABLE bindablePmSupported)
    Q_PROPERTY(bool readLookaheadEnabled READ default NOTIFY readLookaheadEnabledChanged BINDABLE bindableReadLookaheadEnabled)
    Q_PROPERTY(bool readLookaheadSupported READ default NOTIFY readLookaheadSupportedChanged BINDABLE bindableReadLookaheadSupported)
    Q_PROPERTY(bool securityFrozen READ default NOTIFY securityFrozenChanged BINDABLE bindableSecurityFrozen)
    Q_PROPERTY(bool smartEnabled READ default NOTIFY smartEnabledChanged BINDABLE bindableSmartEnabled)
    Q_PROPERTY(bool smartFailing READ default NOTIFY smartFailingChanged BINDABLE bindableSmartFailing)
    Q_PROPERTY(bool smartSupported READ default NOTIFY smartSupportedChanged BINDABLE bindableSmartSupported)
    Q_PROPERTY(bool writeCacheEnabled READ default NOTIFY writeCacheEnabledChanged BINDABLE bindableWriteCacheEnabled)
    Q_PROPERTY(bool writeCacheSupported READ default NOTIFY writeCacheSupportedChanged BINDABLE bindableWriteCacheSupported)

  public:
    explicit DriveAta(const QString& path, const QVariantHash& initials, QObject* parent = nullptr);

    [[nodiscard]] QBindable<QString> bindableSmartSelftestStatus() const { return &b_smartSelftestStatus; }
    [[nodiscard]] QBindable<qulonglong> bindableSmartPowerOnSeconds() const { return &b_smartPowerOnSeconds; }
    [[nodiscard]] QBindable<qulonglong> bindableSmartUpdated() const { return &b_smartUpdated; }
    [[nodiscard]] QBindable<qlonglong> bindableSmartNumBadSectors() const { return &b_smartNumBadSectors; }
    [[nodiscard]] QBindable<double> bindableSmartTemperature() const { return &b_smartTemperature; }
    [[nodiscard]] QBindable<int> bindableAamVendorRecommendedValue() const { return &b_aamVendorRecommendedValue; }
    [[nodiscard]] QBindable<int> bindableSecurityEnhancedEraseUnitMinutes() const { return &b_securityEnhancedEraseUnitMinutes; }
    [[nodiscard]] QBindable<int> bindableSecurityEraseUnitMinutes() const { return &b_securityEraseUnitMinutes; }
    [[nodiscard]] QBindable<int> bindableSmartNumAttributesFailedInThePast() const { return &b_smartNumAttributesFailedInThePast; }
    [[nodiscard]] QBindable<int> bindableSmartNumAttributesFailing() const { return &b_smartNumAttributesFailing; }
    [[nodiscard]] QBindable<int> bindableSmartSelftestPercentRemaining() const { return &b_smartSelftestPercentRemaining; }
    [[nodiscard]] QBindable<bool> bindableAamEnabled() const { return &b_aamEnabled; }
    [[nodiscard]] QBindable<bool> bindableAamSupported() const { return &b_aamSupported; }
    [[nodiscard]] QBindable<bool> bindableApmEnabled() const { return &b_apmEnabled; }
    [[nodiscard]] QBindable<bool> bindableApmSupported() const { return &b_apmSupported; }
    [[nodiscard]] QBindable<bool> bindablePmEnabled() const { return &b_pmEnabled; }
    [[nodiscard]] QBindable<bool> bindablePmSupported() const { return &b_pmSupported; }
    [[nodiscard]] QBindable<bool> bindableReadLookaheadEnabled() const { return &b_readLookaheadEnabled; }
    [[nodiscard]] QBindable<bool> bindableReadLookaheadSupported() const { return &b_readLookaheadSupported; }
    [[nodiscard]] QBindable<bool> bindableSecurityFrozen() const { return &b_securityFrozen; }
    [[nodiscard]] QBindable<bool> bindableSmartEnabled() const { return &b_smartEnabled; }
    [[nodiscard]] QBindable<bool> bindableSmartFailing() const { return &b_smartFailing; }
    [[nodiscard]] QBindable<bool> bindableSmartSupported() const { return &b_smartSupported; }
    [[nodiscard]] QBindable<bool> bindableWriteCacheEnabled() const { return &b_writeCacheEnabled; }
    [[nodiscard]] QBindable<bool> bindableWriteCacheSupported() const { return &b_writeCacheSupported; }

    
    Q_INVOKABLE dbus::AsyncCall* pmGetState(const QVariantMap& options = {});
    Q_INVOKABLE void pmStandby(const QVariantMap& options = {});
    Q_INVOKABLE void pmWakeup(const QVariantMap& options = {});
    Q_INVOKABLE void securityEraseUnit(const QVariantMap& options = {});
    Q_INVOKABLE dbus::AsyncCall* smartGetAttributes(const QVariantMap& options = {});
    Q_INVOKABLE void smartSelftestAbort(const QVariantMap& options = {});
    Q_INVOKABLE void smartUpdate(const QVariantMap& options = {});
    Q_INVOKABLE void smartSelftestStart(const QString& type, const QVariantMap& options = {});
    Q_INVOKABLE void smartSetEnabled(bool value, const QVariantMap& options = {});

  signals:
    void smartSelftestStatusChanged();
    void smartPowerOnSecondsChanged();
    void smartUpdatedChanged();
    void smartNumBadSectorsChanged();
    void smartTemperatureChanged();
    void aamVendorRecommendedValueChanged();
    void securityEnhancedEraseUnitMinutesChanged();
    void securityEraseUnitMinutesChanged();
    void smartNumAttributesFailedInThePastChanged();
    void smartNumAttributesFailingChanged();
    void smartSelftestPercentRemainingChanged();
    void aamEnabledChanged();
    void aamSupportedChanged();
    void apmEnabledChanged();
    void apmSupportedChanged();
    void pmEnabledChanged();
    void pmSupportedChanged();
    void readLookaheadEnabledChanged();
    void readLookaheadSupportedChanged();
    void securityFrozenChanged();
    void smartEnabledChanged();
    void smartFailingChanged();
    void smartSupportedChanged();
    void writeCacheEnabledChanged();
    void writeCacheSupportedChanged();

  private:
    UDisks2DriveAtaInterface* m_iface = nullptr;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)

    ES_DBUS_BINDABLE_PROPERTY(DriveAta, QString, b_smartSelftestStatus, smartSelftestStatusChanged, "SmartSelftestStatus", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, qulonglong, b_smartPowerOnSeconds, smartPowerOnSecondsChanged, "SmartPowerOnSeconds", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, qulonglong, b_smartUpdated, smartUpdatedChanged, "SmartUpdated", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS(DriveAta, qlonglong, b_smartNumBadSectors, -1, smartNumBadSectorsChanged, "SmartNumBadSectors", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, double, b_smartTemperature, smartTemperatureChanged, "SmartTemperature", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, int, b_aamVendorRecommendedValue, aamVendorRecommendedValueChanged, "AamVendorRecommendedValue", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, int, b_securityEnhancedEraseUnitMinutes, securityEnhancedEraseUnitMinutesChanged, "SecurityEnhancedEraseUnitMinutes", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, int, b_securityEraseUnitMinutes, securityEraseUnitMinutesChanged, "SecurityEraseUnitMinutes", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS(DriveAta, int, b_smartNumAttributesFailedInThePast, -1, smartNumAttributesFailedInThePastChanged, "SmartNumAttributesFailedInThePast", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS(DriveAta, int, b_smartNumAttributesFailing, -1, smartNumAttributesFailingChanged, "SmartNumAttributesFailing", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS(DriveAta, int, b_smartSelftestPercentRemaining, -1, smartSelftestPercentRemainingChanged, "SmartSelftestPercentRemaining", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_aamEnabled, aamEnabledChanged, "AamEnabled", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_aamSupported, aamSupportedChanged, "AamSupported", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_apmEnabled, apmEnabledChanged, "ApmEnabled", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_apmSupported, apmSupportedChanged, "ApmSupported", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_pmEnabled, pmEnabledChanged, "PmEnabled", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_pmSupported, pmSupportedChanged, "PmSupported", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_readLookaheadEnabled, readLookaheadEnabledChanged, "ReadLookaheadEnabled", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_readLookaheadSupported, readLookaheadSupportedChanged, "ReadLookaheadSupported", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_securityFrozen, securityFrozenChanged, "SecurityFrozen", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_smartEnabled, smartEnabledChanged, "SmartEnabled", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_smartFailing, smartFailingChanged, "SmartFailing", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_smartSupported, smartSupportedChanged, "SmartSupported", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_writeCacheEnabled, writeCacheEnabledChanged, "WriteCacheEnabled", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(DriveAta, bool, b_writeCacheSupported, writeCacheSupportedChanged, "WriteCacheSupported", propertiesGroup)
};

} // namespace extrashell::udisks2