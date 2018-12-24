#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QMetaEnum>
#include <QAbstractListModel>

template<typename Data>
class ListModel : public QAbstractListModel
{
public:
    enum Roles {};  // Has to be implemented by sub-class.
    Q_ENUM(Roles)

    ListModel() {}
    virtual ~ListModel() {}

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        const auto enumIndex = metaObject()->indexOfEnumerator("Roles");
        const auto metaEnum = metaObject()->enumerator(enumIndex);
        const auto keyCount = metaEnum.keyCount();

        for (auto i = 0; i < keyCount; i++)
            roles.insert(metaEnum.value(i), metaEnum.key(i));

        return roles;
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        Q_UNUSED(index)
        return Qt::ItemIsEditable;
    }

    virtual QVariant data(const QModelIndex &index, int role) const override = 0;
    // Sample implementation.
    //    {
    //        if (index.row() < static_cast<int>(m_items.size()))
    //            return m_items.at(index.row()).data(role);

    //        return QVariant();
    //    }

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override = 0;
    // Sample implementation.
    //    {
    //        if (index.row() < static_cast<int>(m_items.size()))
    //            return m_items[index.row()].setData(role, value);

    //        return false;
    //    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent)
        return m_items.size();
    }

    bool insertRow(const Data& data) {
        beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
        m_items.push_back(data);
        endInsertRows();
        return true;
    }

    bool insertRows(const QVector<Data>& data) {
        if (data.size() == 0)
            return false;

        beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + data.size() - 1);
        m_items += data;
        endInsertRows();
        return true;
    }

    Q_INVOKABLE bool removeRows(int row, int count = 0) {
        beginRemoveRows(QModelIndex(), row, row + count);
        m_items.erase(m_items.begin() + row, m_items.begin() + row + count + 1);
        endRemoveRows();
        return true;
    }

    bool clear() {
        if (rowCount() == 0)
            return true;

        return removeRows(0, rowCount() - 1);
    }

    QVector<Data> items() const {
        return m_items;
    }

    Q_INVOKABLE int size() const {
        return m_items.size();
    }

protected:
    QVector<Data> m_items;
};

#endif // LISTMODEL_H
