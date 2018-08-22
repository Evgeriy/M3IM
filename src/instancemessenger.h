#ifndef INSTANCEMESSENGER_H
#define INSTANCEMESSENGER_H

// QT
#include <QObject>
#include <QMap>
#include <QDateTime>

// FORWARD DECLARATIONS
QT_FORWARD_DECLARE_CLASS(TCPClient)

// ANOTHERS
#include "nlohmann/json.hpp"

static const std::string CODE         = "code";
static const std::string AUTH_TOKEN   = "auth_token";
static const std::string AUTH_STATUS  = "auth_tatus";
static const std::string PHONE_NUMBER = "phone";
static const std::string TYPE         = "type";
static const std::string REGISTER     = "register";
static const std::string SENDER       = "sender";
static const std::string RECEIVER     = "receiver";
static const std::string MESSAGE      = "message";
static const std::string DATE_TIME    = "date_time";

static const std::string COMMAND = "command";
static const std::string CMD_REQUEST_CODE = "request_code";
static const std::string CMD_REQUEST_JWT  = "request_jwt";
static const std::string CMD_PRESENCE  = "presence";

static const std::string PAYLOAD = "payload";

static const std::string RESET_PASSWORD = "reset_password";
static const std::string RESTORE        = "restore";
static const std::string TOKEN          = "token";
static const std::string JWT            = "jwt";

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

    void sendRequestCode();
    void sendRequestJWT();
    void sendPresence();

public:
    QString getJWT() const { return m_jwt; }
    QString getTempToken() const { return m_tempToken; }
    QString getAuthorizationStatus() const { return m_authorizationStatus; }
    QString getPhoneNumber() const { return m_clientData.m_phoneNumber; }
    QString getNikName() const { return m_clientData.m_nikName; }
    QString getFirstName() const { return m_clientData.m_firstName; }
    QString getSecondName() const { return m_clientData.m_secondName; }

public:
    void setCode(const QString &_code) { m_smsCode = _code; }
    void setJWT(const QString &_authToken) { m_jwt = _authToken; }
    void setTempToken(const QString &_authToken) { m_tempToken = _authToken; }
    void setPhoneNumber(const QString &_phoneNumber) { m_clientData.m_phoneNumber = _phoneNumber; }
    void setNickName(const QString &_nikName) { m_clientData.m_nikName = _nikName; }
    void setFirstName(const QString &_firstName) { m_clientData.m_firstName = _firstName; }
    void setSecondName(const QString &_secondName) { m_clientData.m_secondName = _secondName; }
    void addNewContact(const UserItem &_user);
    void addNewDialog(const DialogItem &_dialog);
    void addNewMessage(const QString &_message, const QString &_from, const QString &_to);

public slots:
    void onReceived(nlohmann::json _receivedPackage);

private:
    void receiveWorld(nlohmann::json _json);

    void receiveTempToken(nlohmann::json &_json);
    void receiveJWT(nlohmann::json &_json);
    void receivePresence(nlohmann::json &_json);

public:
    static nlohmann::json getJsonFromString(const QString &_strName, const QString &_str);
    static nlohmann::json getJsonFromString(const std::string &_strName, const std::string &_str);

public:
    nlohmann::json getPhoneNumberJson() const;
    nlohmann::json getNikNameJson() const;
    nlohmann::json getFirstNameJson() const;
    nlohmann::json getSecondNameJson() const;

private:
    TCPClient *m_pTCPClient{nullptr};
    QString m_tempToken;
    QString m_jwt;
    QString m_authorizationStatus;

public:
    QString m_smsCode;
    UserItem m_clientData;
    QMap<QString, QList<DialogItem> > m_dialogs;
    QMap<QString, UserItem> m_contacts;
    std::string m_worldStdString{""};
};

#endif // INSTANCEMESSENGER_H
