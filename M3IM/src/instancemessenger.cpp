#include "instancemessenger.h"

// QT
#include <QFile>
#include <QTextStream>

// CPP/STL
#include <iostream>

// ANOTHERS
#include "tcp_client.h"
#include "contacts_model.hpp"
#include "dialogs_model.hpp"

InstanceMessenger::InstanceMessenger(TCPClient *_tcpClient, QObject *_parent) :
    QObject(_parent),
    m_pTCPClient(_tcpClient) {

    readJWTFromFile();
    connect(m_pTCPClient, &TCPClient::received, this, &InstanceMessenger::onReceived);
    connect(m_pTCPClient, &TCPClient::statusChanged, this, &InstanceMessenger::onSocketStateChanged);

    m_clientData.m_isOnline = true;
    m_pContactsModel = new ContactsModel();
    m_pDialogModel = new DialogModel();

    connect(m_pContactsModel, &ContactsModel::userItemChanged, this, &InstanceMessenger::onUserItemChanged);
}

InstanceMessenger::~InstanceMessenger() {
    delete m_pTCPClient;
    delete m_pContactsModel;
    delete m_pDialogModel;
}

void InstanceMessenger::sendHello() {
    // configure json package
    nlohmann::json jsonPhoneNumber;
    jsonPhoneNumber[COMMAND] = "hello";

    // send json
    m_pTCPClient->send(jsonPhoneNumber);
}

void InstanceMessenger::reconnect(QString _ip, int _port) {
    if (m_pTCPClient != nullptr) {
        m_pTCPClient->reconnect(_ip, _port);
    }
}

void InstanceMessenger::disconnect() {
    if (m_pTCPClient != nullptr) {
        m_pTCPClient->disconnect();
    }
}

void InstanceMessenger::sendRequestCode() {
    // configure payload part
    nlohmann::json payload;
    payload[PHONE] = m_clientData.m_phone.toStdString();
    payload[TYPE] = REGISTER;

    // configure header
    nlohmann::json header;
    header[COMMAND] = CMD_REQUEST_CODE;

    // configure result package
    nlohmann::json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    std::cout << resultPackage << std::endl;

    // send json
    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::sendRequestJWT() {
    // configure payload part
    nlohmann::json payload;
    payload[TYPE] = REGISTER;
    payload[CODE] = m_code.toStdString();
    payload[RESET_PASSWORD] = false;
    payload[RESTORE] = false;
    payload[TOKEN] = m_tempToken.toStdString();

    // configure header
    nlohmann::json header;
    header[COMMAND] = CMD_REQUEST_JWT;

    // configure result package
    nlohmann::json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    // send json
    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::sendRequestPresence() {
    // configure payload part
    nlohmann::json payload;
    payload[JWT] = m_jwt.toStdString();

    // configure header
    nlohmann::json header;
    header[COMMAND] = CMD_REQUEST_PRESENCE;

    // configure result package
    nlohmann::json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    // send json
    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::sendRequestContacts() {
    // configure header
    nlohmann::json header;
    header[COMMAND] = CMD_REQUEST_CONTACTS;

    // configure result package
    nlohmann::json resultPackage;
    resultPackage = header;

    // send json
    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::sendRequestDialog(const int &_userId) {
    // configure payload part
    nlohmann::json payload;
    payload[JWT] = m_jwt.toStdString();
    payload[USER_ID] = _userId;
    // configure header
    nlohmann::json header;
    header[COMMAND] = CMD_REQUEST_DIALOG;

    // configure result package
    nlohmann::json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    m_pDialogModel->setDialog(m_dialogs[_userId]);

    // send json
    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::sendMessage(const QString &_message, const int &_userId) {
    // create dialog item and add that to local storage
    DialogItem dialogItem(_message, m_clientData.m_id);
    m_dialogs[_userId].append(dialogItem);

    // Change model
    m_pDialogModel->addDialogItem(dialogItem);

    // configure payload part
    nlohmann::json payload;
    payload[MESSAGE] = _message.toStdString();
    payload[USER_ID] = _userId;

    // configure header
    nlohmann::json header;
    header[COMMAND] = CMD_SEND_MESSAGE;

    // configure result package
    nlohmann::json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    // send json
    m_pTCPClient->send(resultPackage);
}

int InstanceMessenger::addNewContact(const QString &_firstName, const QString &_lastName, const QString &_phone) {
    return 2;
}

int InstanceMessenger::getUserId(const QString &_firstName, const QString &_phone) {
    int retValue = -1;
    QList<int> keys = m_contacts.keys();

    for (int i = 0; i < keys.size(); ++i) {
        qDebug() << _phone << m_contacts[keys[i]].m_phone;
        if (/*m_contacts[keys[i]].m_firstName == _firstName &&*/
                m_contacts[keys[i]].m_phone == _phone) {
            qDebug() << m_contacts[keys[i]].m_id;
            retValue = m_contacts[keys[i]].m_id;
            break;
        }
    }


    qDebug() << retValue;
    return retValue;
}

void InstanceMessenger::setActiveDialog(const int &_userId) {
    m_activeDialog = _userId;
    m_pDialogModel->setDialog(m_dialogs[m_activeDialog]);
}

void InstanceMessenger::onSocketStateChanged(QAbstractSocket::SocketState _socketState) {
    switch(_socketState) {
    case QAbstractSocket::UnconnectedState:
        m_socketState = "UnconnectedState";
        break;
    case QAbstractSocket::HostLookupState:
        m_socketState = "HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        m_socketState = "ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        m_socketState = "ConnectedState";
        sendRequestPresence();
        break;
    case QAbstractSocket::BoundState:
        m_socketState = "BoundState";
        break;
    case QAbstractSocket::ClosingState:
        m_socketState = "ClosingState";
        break;
    case QAbstractSocket::ListeningState:
        m_socketState = "ListeningState";
        break;
    }

    emit socketStatusChanged();
}

std::string InstanceMessenger::getCommand(const nlohmann::json &_json) const {
    return _json[COMMAND];
}

std::string InstanceMessenger::getDescription(const nlohmann::json &_json) const {
    return _json[DESCRIPTION];
}

int InstanceMessenger::getCode(const nlohmann::json &_json) const {
    if (_json.find(CODE) != _json.end()) {
        return _json[CODE];
    } else {
        return -1;
    }

}

void InstanceMessenger::onUserItemChanged(int _id, UserItem _userItem) {
    m_contacts[_id] = _userItem;
}

void InstanceMessenger::onReceived(nlohmann::json _receivedPackage) {
    if (getCode(_receivedPackage) == -1) {
        std::cout << "[InstanceMessenger::onReceived] " << "Server message:" << _receivedPackage << std::endl;

        if (getCommand(_receivedPackage) == NEW_MESSAGE) {
            processReceivedMessage(_receivedPackage);
        } else if (getCommand(_receivedPackage) == USER_ONLINE
                   || getCommand(_receivedPackage) == USER_OFFLINE) {
            processReceivedOnlineStatus(_receivedPackage);
        }

    } else {
        // RESPONSES
        switch(getCode(_receivedPackage)) {
        case OK:
            std::cout << "[InstanceMessenger::onReceived] " << "Response code " << OK << std::endl;
            std::cout << "[InstanceMessenger::onReceived] " << "Response message: " << _receivedPackage << std::endl;

            if (getCommand(_receivedPackage) == CMD_HELLO) {
                processResponseToRequestHello(_receivedPackage);
            } else if (getCommand(_receivedPackage) == CMD_REQUEST_CODE) {
                processResponseToRequestCode(_receivedPackage);
            } else if (getCommand(_receivedPackage) == CMD_REQUEST_JWT) {
                processResponseToRequestJWT(_receivedPackage);
            } else if (getCommand(_receivedPackage) == CMD_REQUEST_PRESENCE) {
                processResponseToPresence(_receivedPackage);
            } else if (getCommand(_receivedPackage) == CMD_REQUEST_CONTACTS) {
                processResponseToContacts(_receivedPackage);
            } else if (getCommand(_receivedPackage) == CMD_REQUEST_DIALOG) {
                processResponseToDialog(_receivedPackage);
            } else if (getCommand(_receivedPackage) == CMD_SEND_MESSAGE) {
                processResponseToSendMessage(_receivedPackage);
            }

            break;
        case PARTIAL_CONTENT:
            std::cout << "[InstanceMessenger::onReceived] " << "Response code: " << PARTIAL_CONTENT << std::endl;
            break;
        case MOVED_PERMANENTLY:
            std::cout << "[InstanceMessenger::onReceived] " << "Response code: " << MOVED_PERMANENTLY << std::endl;
            break;
        case FOUND:
            std::cout << "[InstanceMessenger::onReceived] " << "Response code: " << FOUND << std::endl;
            break;
        case NOT_MODIFIED:
            std::cout << "[InstanceMessenger::onReceived] " << "Response code: " << NOT_MODIFIED << std::endl;
            break;
        case FORBIDDEN:
            std::cout << "[InstanceMessenger::onReceived] " << "Response code: " << FORBIDDEN << std::endl;
            break;
        case NOT_FOUND:
            std::cout << "[InstanceMessenger::onReceived] " << "Response code: " << NOT_FOUND << std::endl;
            break;
        case INTERNAL_SERVER_ERROR:
            std::cout << "[InstanceMessenger::onReceived] " << "Response code: " << INTERNAL_SERVER_ERROR << std::endl;
            break;
        default:
            std::cout << "[InstanceMessenger::onReceived] " << "Undefined response code!" << std::endl;
            break;
        }
    }
}

void InstanceMessenger::processResponseToRequestHello(nlohmann::json _json) {
    std::cout << "[InstanceMessenger::processResponseToRequestHello] " << "Json: " << _json << std::endl;
    m_worldStdString = _json[PAYLOAD][MESSAGE];
}

void InstanceMessenger::processResponseToRequestCode(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processResponseToRequestCode] " << "Json: " << _json << std::endl;
    m_tempToken = QString::fromStdString(_json[PAYLOAD][TOKEN]);
}

void InstanceMessenger::processResponseToRequestJWT(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processResponseToRequestJWT] " << "Json: " << _json << std::endl;
    m_jwt = QString::fromStdString(_json[PAYLOAD][JWT]);
    writeJWTToFile();
}

void InstanceMessenger::processResponseToPresence(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processResponseToPresence] " << "Json: " << _json << std::endl;
    m_authStatus = getDescription(_json) == "ok";
}

void InstanceMessenger::processResponseToContacts(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processResponseToContacts] " << "Json: " << _json << std::endl;

    nlohmann::json users = _json[PAYLOAD][USERS];
    const size_t amount = users.size();

    for (size_t i = 0; i < amount; ++i) {
        // parse phone and user_id
        QString phone = QString::fromStdString(users[i][PHONE]);

        int id = 0;
        if (users[i][USER_ID].type() == nlohmann::detail::value_t::string) {
            std::string idStr = users[i][USER_ID];
            id = std::atoi(idStr.c_str());
        } else {
            id = users[i][USER_ID];
        }

        if (phone == m_clientData.m_phone) {
            m_clientData.m_id = id;
        }

        UserItem userItem(phone, id);
        userItem.m_isOnline = users[i][ONLINE];

        // add new user item to local storage
        if (!m_contacts.contains(id)) {
            m_contacts[id] = userItem;
        }
    }

    // change model
    m_pContactsModel->setContacts(m_contacts);
}

void InstanceMessenger::processResponseToDialog(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processResponseToDialog] " << "Json: " << _json << std::endl;
}

void InstanceMessenger::processResponseToSendMessage(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processResponseToSendMessage] " << "Json: " << _json << std::endl;
}

void InstanceMessenger::processReceivedMessage(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processReceivedMessage] " << "Json: " << _json << std::endl;

    // parse payload part
    nlohmann::json payload = _json[PAYLOAD];

    // parse message
    QString msg = QString::fromStdString(payload[MESSAGE]);

    // parse user id
    int userId = 0;
    if (payload[SENDER_USER_ID].type() == nlohmann::detail::value_t::string) {
        std::string idStr = payload[SENDER_USER_ID];
        userId = std::atoi(idStr.c_str());
    } else {
        userId = payload[SENDER_USER_ID];
    }

    if (userId) {
        DialogItem dialogItem(msg, userId);

        // add new dialog item to local storage
        m_dialogs[userId].append(dialogItem);

        // change dialog model
        m_pDialogModel->addDialogItem(dialogItem);

        // change user model
        m_contacts[userId].m_lastMessage = msg;
        m_contacts[userId].m_unreadCount++;

        m_pContactsModel->setContacts(m_contacts);
        m_pContactsModel->sort(0);
    }
}

void InstanceMessenger::processReceivedOnlineStatus(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processReceivedOnlineStatus] " << "Json: " << _json << std::endl;

    // parse payload part
    nlohmann::json payload = _json[PAYLOAD];

    // parse user id
    int userId = 0;
    if (payload[USER_ID].type() == nlohmann::detail::value_t::string) {
        std::string idStr = payload[USER_ID];
        userId = std::atoi(idStr.c_str());
    } else {
        userId = payload[USER_ID];
    }

    m_contacts[userId].m_isOnline = (_json[COMMAND] == USER_ONLINE);
    m_pContactsModel->setContacts(m_contacts);
}

void InstanceMessenger::readJWTFromFile() {
    QFile file(m_jwtPath);
    if (file.open(QIODevice::ReadOnly)) {
        m_clientData.m_phone = file.readLine();
        m_clientData.m_phone.remove("\n");
        m_jwt = file.readLine();
        m_jwt.remove("\n");
        std::cout << m_clientData.m_phone.toStdString() << std::endl;
        std::cout << m_jwt.toStdString() << std::endl;
    }
}

void InstanceMessenger::writeJWTToFile() {
    QFile file(m_jwtPath);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << m_clientData.m_phone << endl;
        stream << m_jwt << endl;
    }
}

nlohmann::json InstanceMessenger::getJsonFromString(const QString &_strName, const QString &_str) {
    return getJsonFromString(_strName.toStdString(), _str.toStdString());
}

nlohmann::json InstanceMessenger::getJsonFromString(const std::string &_strName, const std::string &_str) {
    nlohmann::json retValue;
    retValue[_strName] = _str;
    return retValue;
}

nlohmann::json InstanceMessenger::getPhoneJson() const {
    return getJsonFromString(QString::fromStdString(PHONE), getPhone());
}
