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

Q_MOC_INCLUDE("drive_ata.h")

Q_DECLARE_LOGGING_CATEGORY(logUDisks2Drive)

class UDisks2DriveInterface;

namespace extrashell::udisks2 {

class DriveAta;

class Drive : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UDisks2Drive)
    QML_UNCREATABLE("Drive must be acquired from the UDisks2.")

    Q_PROPERTY(extrashell::udisks2::DriveAta* ata READ default NOTIFY ataChanged BINDABLE bindableAta)
    Q_PROPERTY(QVariantMap configuration READ default NOTIFY configurationChanged BINDABLE bindableConfiguration)
    Q_PROPERTY(QStringList mediaCompatibility READ default NOTIFY mediaCompatibilityChanged BINDABLE bindableMediaCompatibility)
    Q_PROPERTY(QString connectionBus READ default NOTIFY connectionBusChanged BINDABLE bindableConnectionBus)
    Q_PROPERTY(QString id READ default NOTIFY idChanged BINDABLE bindableId)
    Q_PROPERTY(QString media READ default NOTIFY mediaChanged BINDABLE bindableMedia)
    Q_PROPERTY(QString model READ default NOTIFY modelChanged BINDABLE bindableModel)
    Q_PROPERTY(QString revision READ default NOTIFY revisionChanged BINDABLE bindableRevision)
    Q_PROPERTY(QString seat READ default NOTIFY seatChanged BINDABLE bindableSeat)
    Q_PROPERTY(QString serial READ default NOTIFY serialChanged BINDABLE bindableSerial)
    Q_PROPERTY(QString siblingId READ default NOTIFY siblingIdChanged BINDABLE bindableSiblingId)
    Q_PROPERTY(QString sortKey READ default NOTIFY sortKeyChanged BINDABLE bindableSortKey)
    Q_PROPERTY(QString vendor READ default NOTIFY vendorChanged BINDABLE bindableVendor)
    Q_PROPERTY(QString wwn READ default NOTIFY wwnChanged BINDABLE bindableWWN)
    Q_PROPERTY(qulonglong size READ default NOTIFY sizeChanged BINDABLE bindableSize)
    Q_PROPERTY(qulonglong timeDetected READ default NOTIFY timeDetectedChanged BINDABLE bindableTimeDetected)
    Q_PROPERTY(qulonglong timeMediaDetected READ default NOTIFY timeMediaDetectedChanged BINDABLE bindableTimeMediaDetected)
    Q_PROPERTY(uint opticalNumAudioTracks READ default NOTIFY opticalNumAudioTracksChanged BINDABLE bindableOpticalNumAudioTracks)
    Q_PROPERTY(uint opticalNumDataTracks READ default NOTIFY opticalNumDataTracksChanged BINDABLE bindableOpticalNumDataTracks)
    Q_PROPERTY(uint opticalNumSessions READ default NOTIFY opticalNumSessionsChanged BINDABLE bindableOpticalNumSessions)
    Q_PROPERTY(uint opticalNumTracks READ default NOTIFY opticalNumTracksChanged BINDABLE bindableOpticalNumTracks)
    Q_PROPERTY(int rotationRate READ default NOTIFY rotationRateChanged BINDABLE bindableRotationRate)
    Q_PROPERTY(bool canPowerOff READ default NOTIFY canPowerOffChanged BINDABLE bindableCanPowerOff)
    Q_PROPERTY(bool ejectable READ default NOTIFY ejectableChanged BINDABLE bindableEjectable)
    Q_PROPERTY(bool mediaAvailable READ default NOTIFY mediaAvailableChanged BINDABLE bindableMediaAvailable)
    Q_PROPERTY(bool mediaChangeDetected READ default NOTIFY mediaChangeDetectedChanged BINDABLE bindableMediaChangeDetected)
    Q_PROPERTY(bool mediaRemovable READ default NOTIFY mediaRemovableChanged BINDABLE bindableMediaRemovable)
    Q_PROPERTY(bool optical READ default NOTIFY opticalChanged BINDABLE bindableOptical)
    Q_PROPERTY(bool opticalBlank READ default NOTIFY opticalBlankChanged BINDABLE bindableOpticalBlank)
    Q_PROPERTY(bool removable READ default NOTIFY removableChanged BINDABLE bindableRemovable)

  public:
    explicit Drive(const QString& path, const QVariantHash& initials, QObject* parent = nullptr);

    [[nodiscard]] QBindable<DriveAta*> bindableAta() const { return &b_ata; }
    [[nodiscard]] QBindable<QVariantMap> bindableConfiguration() const { return &b_configuration; }
    [[nodiscard]] QBindable<QStringList> bindableMediaCompatibility() const { return &b_mediaCompatibility; }
    [[nodiscard]] QBindable<QString> bindableConnectionBus() const { return &b_connectionBus; }
    [[nodiscard]] QBindable<QString> bindableId() const { return &b_id; }
    [[nodiscard]] QBindable<QString> bindableMedia() const { return &b_media; }
    [[nodiscard]] QBindable<QString> bindableModel() const { return &b_model; }
    [[nodiscard]] QBindable<QString> bindableRevision() const { return &b_revision; }
    [[nodiscard]] QBindable<QString> bindableSeat() const { return &b_seat; }
    [[nodiscard]] QBindable<QString> bindableSerial() const { return &b_serial; }
    [[nodiscard]] QBindable<QString> bindableSiblingId() const { return &b_siblingId; }
    [[nodiscard]] QBindable<QString> bindableSortKey() const { return &b_sortKey; }
    [[nodiscard]] QBindable<QString> bindableVendor() const { return &b_vendor; }
    [[nodiscard]] QBindable<QString> bindableWWN() const { return &b_wwn; }
    [[nodiscard]] QBindable<qulonglong> bindableSize() const { return &b_size; }
    [[nodiscard]] QBindable<qulonglong> bindableTimeDetected() const { return &b_timeDetected; }
    [[nodiscard]] QBindable<qulonglong> bindableTimeMediaDetected() const { return &b_timeMediaDetected; }
    [[nodiscard]] QBindable<uint> bindableOpticalNumAudioTracks() const { return &b_opticalNumAudioTracks; }
    [[nodiscard]] QBindable<uint> bindableOpticalNumDataTracks() const { return &b_opticalNumDataTracks; }
    [[nodiscard]] QBindable<uint> bindableOpticalNumSessions() const { return &b_opticalNumSessions; }
    [[nodiscard]] QBindable<uint> bindableOpticalNumTracks() const { return &b_opticalNumTracks; }
    [[nodiscard]] QBindable<int> bindableRotationRate() const { return &b_rotationRate; }
    [[nodiscard]] QBindable<bool> bindableCanPowerOff() const { return &b_canPowerOff; }
    [[nodiscard]] QBindable<bool> bindableEjectable() const { return &b_ejectable; }
    [[nodiscard]] QBindable<bool> bindableMediaAvailable() const { return &b_mediaAvailable; }
    [[nodiscard]] QBindable<bool> bindableMediaChangeDetected() const { return &b_mediaChangeDetected; }
    [[nodiscard]] QBindable<bool> bindableMediaRemovable() const { return &b_mediaRemovable; }
    [[nodiscard]] QBindable<bool> bindableOptical() const { return &b_optical; }
    [[nodiscard]] QBindable<bool> bindableOpticalBlank() const { return &b_opticalBlank; }
    [[nodiscard]] QBindable<bool> bindableRemovable() const { return &b_removable; }

    Q_INVOKABLE void eject(const QVariantMap& options = {});
    Q_INVOKABLE void powerOff(const QVariantMap& options = {});
    Q_INVOKABLE void setConfiguration(const QVariantMap& value, const QVariantMap& options = {});

    void setAta(DriveAta* ata) { b_ata = ata; }

  signals:
    void ataChanged();
    void configurationChanged();
    void mediaCompatibilityChanged();
    void connectionBusChanged();
    void idChanged();
    void mediaChanged();
    void modelChanged();
    void revisionChanged();
    void seatChanged();
    void serialChanged();
    void siblingIdChanged();
    void sortKeyChanged();
    void vendorChanged();
    void wwnChanged();
    void sizeChanged();
    void timeDetectedChanged();
    void timeMediaDetectedChanged();
    void opticalNumAudioTracksChanged();
    void opticalNumDataTracksChanged();
    void opticalNumSessionsChanged();
    void opticalNumTracksChanged();
    void rotationRateChanged();
    void canPowerOffChanged();
    void ejectableChanged();
    void mediaAvailableChanged();
    void mediaChangeDetectedChanged();
    void mediaRemovableChanged();
    void opticalChanged();
    void opticalBlankChanged();
    void removableChanged();

  private:
    UDisks2DriveInterface* m_iface = nullptr;

    ES_DBUS_PROPERTIES_GROUP(propertiesGroup)
    
    Q_OBJECT_BINDABLE_PROPERTY(Drive, DriveAta*, b_ata, &Drive::ataChanged)
    
    ES_DBUS_BINDABLE_PROPERTY(Drive, QVariantMap, b_configuration, configurationChanged, "Configuration", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QStringList, b_mediaCompatibility, mediaCompatibilityChanged, "MediaCompatibility", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_connectionBus, connectionBusChanged, "ConnectionBus", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_id, idChanged, "Id", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_media, mediaChanged, "Media", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_model, modelChanged, "Model", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_revision, revisionChanged, "Revision", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_seat, seatChanged, "Seat", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_serial, serialChanged, "Serial", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_siblingId, siblingIdChanged, "SiblingId", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_sortKey, sortKeyChanged, "SortKey", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_vendor, vendorChanged, "Vendor", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, QString, b_wwn, wwnChanged, "WWN", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, qulonglong, b_size, sizeChanged, "Size", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, qulonglong, b_timeDetected, timeDetectedChanged, "TimeDetected", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, qulonglong, b_timeMediaDetected, timeMediaDetectedChanged, "TimeMediaDetected", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, uint, b_opticalNumAudioTracks, opticalNumAudioTracksChanged, "OpticalNumAudioTracks", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, uint, b_opticalNumDataTracks, opticalNumDataTracksChanged, "OpticalNumDataTracks", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, uint, b_opticalNumSessions, opticalNumSessionsChanged, "OpticalNumSessions", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, uint, b_opticalNumTracks, opticalNumTracksChanged, "OpticalNumTracks", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, int, b_rotationRate, rotationRateChanged, "RotationRate", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, bool, b_canPowerOff, canPowerOffChanged, "CanPowerOff", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, bool, b_ejectable, ejectableChanged, "Ejectable", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS(Drive, bool, b_mediaAvailable, true, mediaAvailableChanged, "MediaAvailable", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY_WITH_ARGS(Drive, bool, b_mediaChangeDetected, true, mediaChangeDetectedChanged, "MediaChangeDetected", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, bool, b_mediaRemovable, mediaRemovableChanged, "MediaRemovable", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, bool, b_optical, opticalChanged, "Optical", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, bool, b_opticalBlank, opticalBlankChanged, "OpticalBlank", propertiesGroup)
    ES_DBUS_BINDABLE_PROPERTY(Drive, bool, b_removable, removableChanged, "Removable", propertiesGroup)
};

} // namespace extrashell::udisks2