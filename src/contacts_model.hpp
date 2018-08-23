﻿#ifndef CONTACTS_MODEL_H
#define CONTACTS_MODEL_H

// QT
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>

// CPP/STL
#include <cstdio>
#include <iostream>

// ANOTHERS
#include "instancemessenger.h"

class ContactsModel : public QAbstractListModel {
    Q_OBJECT
    Q_ENUMS(ContactsRoles)

public:
    enum ContactsRoles {
        Phone = Qt::UserRole + 1,
        UserId
    };

//    using QAbstractListModel::QAbstractListModel;

    QHash<int,QByteArray> roleNames() const override {
        return { { Phone, "phone" },
                 { UserId, "userId" }
        };
    }

    void addContact(const UserItem &_userItem) {
        if (!m_contacts.contains(_userItem)) {
            beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
            m_contacts.append(_userItem);
            endInsertRows();

            QModelIndex index = createIndex(0, 0);
            emit dataChanged(index, index);
        }
    }

    int rowCount(const QModelIndex & parent = QModelIndex()) const override {
        if (parent.isValid())
            return 0;
        return m_contacts.size();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role) override {
        if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
            return false;

        UserItem &item = m_contacts[index.row()];
        switch(role) {
        case Phone:
            item.m_phone = value.toString();
            break;
        case UserId:
            item.m_id = value.toString();
            break;
        default:
            return false;
        }

        emit dataChanged(index, index, {role});
        return true;
    }

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override {
        if (!hasIndex(index.row(), index.column(), index.parent()))
            return {};

        const UserItem &item = m_contacts.at(index.row());
        switch(role) {
        case Phone:
            return item.m_phone;
        case UserId:
            return item.m_id;
        default:
            return {};
        }
    }

private:
    QList<UserItem> m_contacts;
};

#endif // CONTACTS_MODEL_H