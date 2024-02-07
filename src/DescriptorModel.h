#ifndef DESCRIPTORMODEL_H
#define DESCRIPTORMODEL_H

#include <QLowEnergyDescriptor>
#include <QAbstractListModel>

class DescriptorModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DescriptorRoles {
        IS_VALID_ROLE = Qt::UserRole + 1,
        NAME_ROLE,
        TYPE_ROLE,
        UUID_ROLE,
        VALUE_ROLE,
    };
    explicit DescriptorModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void clear();
    void addDescriptor(const QLowEnergyDescriptor &descriptor);

private:
    QString descriptorType(const QBluetoothUuid::DescriptorType &descriptorType) const;

    QList<QLowEnergyDescriptor> mDescriptors;
};

#endif // DESCRIPTORMODEL_H
