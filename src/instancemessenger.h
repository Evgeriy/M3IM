#ifndef INSTANCEMESSENGER_H
#define INSTANCEMESSENGER_H

#include <QObject>
#include <QDateTime>
#include <QString>

#include <vector>
#include <string>

#include "tcp_client.h"

static const std::string CODE         = "code";
static const std::string AUTH_TOKEN   = "auth_token";
static const std::string AUTH_STATUS  = "auth_tatus";
static const std::string PHONE_NUMBER = "phone_number";
static const std::string SENDER       = "sender";
static const std::string RECEIVER     = "receiver";
static const std::string MESSAGE      = "message";
static const std::string DATE_TIME    = "date_time";
static const std::string COMMAND      = "command";
static const std::string RESULT       = "result";
static const std::string CONTACTS     = "contacts";
static const std::string DIALOG       = "dialog";

static const int CONTACT_LIST_REQUEST = 1;
static const int DIALOG_REQUEST       = 1;

struct UserItem {
    UserItem() {}
    QString m_phoneNumber;
    QString m_nikName;
    QString m_firstName;
    QString m_secondName;
};

struct DialogItem {
    DialogItem() {}
    DialogItem(QString _dialogName, bool _isReceived, QString _message, QDateTime _time) :
        m_dialogName(_dialogName),
        m_isReceived(_isReceived),
        m_message(_message),
        m_dateTime(_time) {}

    QString m_dialogName;
    bool m_isReceived;
    QString m_message;
    QDateTime m_dateTime;
};

class InstanceMessenger : public QObject {
    Q_OBJECT
public:
    explicit InstanceMessenger(TCPClient *_tcpClient, QObject *_parent = nullptr);
    ~InstanceMessenger();

public:
    void sendHello();
    void receiveWorld(json _json);

public:
    void sendPhoneNumber();
    void sendCode();
    void sendAuthorizationToken();
    void sendMessage(const QString &_to, const QString &_message);

public:
    void sendRequestByCommand(const int &_command);
    void sendRequestForContactList();
    void sendRequestForDialog();

public:
    QString getAuthorizationToken() const { return m_authorizationToken; }
    bool getAuthorizationStatus() const { return m_authorizationStatus; }
    QString getPhoneNumber() const { return m_clientData.m_phoneNumber; }
    QString getNikName() const { return m_clientData.m_nikName; }
    QString getFirstName() const { return m_clientData.m_firstName; }
    QString getSecondName() const { return m_clientData.m_secondName; }

public:
    void setAuthorizationToken(const QString &_authToken) { m_authorizationToken = _authToken; }
    void setPhoneNumber(const QString &_phoneNumber) { m_clientData.m_phoneNumber = _phoneNumber; }
    void setNickName(const QString &_nikName) { m_clientData.m_nikName = _nikName; }
    void setFirstName(const QString &_firstName) { m_clientData.m_firstName = _firstName; }
    void setSecondName(const QString &_secondName) { m_clientData.m_secondName = _secondName; }
    void addNewContact(const UserItem &_user);
    void addNewDialog(const DialogItem &_dialog);
    void addNewMessage(const QString &_message, const QString &_from, const QString &_to);

public slots:
    void onReceived(json _receivedPackage);

private:
    void receiveCode(json &_json);
    void receiveAuthorizationToken(json &_json);
    void receiveAuthorizationStatus(json &_json);
    void receiveContactList(json &_json);
    void receiveDialog(json &_json);
    void receiveNewMessage(json &_json);

public:
    static json getJsonFromString(const QString &_strName, const QString &_str);
    static json getJsonFromString(const std::string &_strName, const std::string &_str);

public:
    json getPhoneNumberJson() const;
    json getNikNameJson() const;
    json getFirstNameJson() const;
    json getSecondNameJson() const;

private:
    TCPClient *m_pTCPClient{nullptr};
    QString m_authorizationToken;
    bool m_authorizationStatus{false};

public:
    quint16 m_smsCode;
    UserItem m_clientData;
    QMap<QString, QList<DialogItem> > m_dialogs;
    QMap<QString, UserItem> m_contacts;
    std::string m_worldStdString{""};
};

#endif // INSTANCEMESSENGER_H
