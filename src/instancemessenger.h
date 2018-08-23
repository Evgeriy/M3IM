#ifndef INSTANCEMESSENGER_H
#define INSTANCEMESSENGER_H

// QT
#include <QObject>
#include <QMap>
#include <QDateTime>

// FORWARD DECLARATIONS
QT_FORWARD_DECLARE_CLASS(TCPClient)

// ANOTHERS
#include "instancemessenger_global.h"
#include "nlohmann/json.hpp"

struct UserItem {
public:
    UserItem() {}
    UserItem(const QString &_phone, const QString &_id) :
        m_phone(_phone),
        m_id(_id) {}

public:
    QString m_phone;
    QString m_id;

    // Unused yet
    QString m_nikName;
    QString m_firstName;
    QString m_secondName;
};

struct DialogItem {
public:
    DialogItem() {}
    DialogItem(const QString &_message, const QString &_userId) :
        m_message(_message),
        m_user_id(_userId) {}
public:
    QString m_message;
    QString m_user_id;

    // Unused yet
    QString m_dialogName;
    bool m_isReceived;
    QDateTime m_dateTime;
};

class InstanceMessenger : public QObject {
    Q_OBJECT
public:
    explicit InstanceMessenger(TCPClient *_tcpClient, QObject *_parent = nullptr);
    ~InstanceMessenger();

public:
    void sendHello();
    void sendRequestCode();
    void sendRequestJWT();
    void sendRequestPresence();
    void sendRequestContacts();
    void sendRequestDialog(const QString &_userId);
    void sendMessage(const QString &_message, const QString &_userId);

public:
    QString getCode() const { return m_code; }
    QString getTempToken() const { return m_tempToken; }
    QString getJWT() const { return m_jwt; }
    QString getAuthStatus() const { return m_authStatus; }
    QString getPhone() const { return m_clientData.m_phone; }

public:
    void setCode(const QString &_code) { m_code = _code; }
    void setTempToken(const QString &_tempToken) { m_tempToken = _tempToken; }
    void setJWT(const QString &_jwt) { m_jwt = _jwt; }
    void setAuthStatus(const QString &_phone) { m_clientData.m_phone = _phone; }
    void setPhone(const QString &_phone) { m_clientData.m_phone = _phone; }

public slots:
    void onReceived(nlohmann::json _receivedPackage);

private:
    void receiveWorld(nlohmann::json _json);
    void receiveTempToken(nlohmann::json &_json);
    void receiveJWT(nlohmann::json &_json);
    void receivePresence(nlohmann::json &_json);
    void receiveContacts(nlohmann::json &_json);
    void receiveDialog(nlohmann::json &_json);
    void receiveMessage(nlohmann::json &_json);

private:
    TCPClient *m_pTCPClient{nullptr};   // socket
    QString m_code;                  // sms_code (should received from server with code request)
    QString m_tempToken;                // temp token (should received from server with code request)
    QString m_jwt;                      // jwt token (should received from server with jwt request)
    QString m_authStatus;               // authorization status (should received from server with presence request)

private:
    UserItem m_clientData;                       // personal data   - phone and id
    QMap<QString, UserItem> m_contacts;          // map of contacts - key - user_id, value - userItem obj
    QMap<QString, QList<DialogItem> > m_dialogs; // map of dialogs  - key - user_id, value - list of dialogItem obj

    std::string m_worldStdString{""};

public:
    static nlohmann::json getJsonFromString(const QString &_strName, const QString &_str);
    static nlohmann::json getJsonFromString(const std::string &_strName, const std::string &_str);

public:
    nlohmann::json getPhoneJson() const;
};

#endif // INSTANCEMESSENGER_H
