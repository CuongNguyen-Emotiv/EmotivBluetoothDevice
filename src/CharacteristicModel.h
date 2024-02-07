#ifndef CHARACTERISTICMODEL_H
#define CHARACTERISTICMODEL_H

#include <QLowEnergyCharacteristic>
#include <QAbstractListModel>

class CharacteristicItem {
public:
    CharacteristicItem(const QLowEnergyCharacteristic& characteristic);

    QString name() const;
    QString properties() const;
    QString uuid() const;
    QString textValue() const;
    QString hexValue() const;
    QLowEnergyCharacteristic* characteristic() const;

private:
    QLowEnergyCharacteristic mCharacteristic;
};

class CharacteristicModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CharacteristicRoles {
        NAME_ROLE = Qt::UserRole + 1,
        PROPERTIES_ROLE,
        UUID_ROLE,
        TEXT_VALUE_ROLE,
        HEX_VALUE_ROLE,
        Q_LOWENERGY_CHARACTERISTIC_PTR_ROLE,
    };
    explicit CharacteristicModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void clear();
    void addCharacteristic(const CharacteristicItem &characteristic);

private:
    QList<CharacteristicItem> mCharacteristics;
};

#endif // CHARACTERISTICMODEL_H
