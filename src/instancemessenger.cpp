#include "instancemessenger.h"

// CPP/STL
#include <iostream>

// ANOTHERS
#include "tcp_client.h"

InstanceMessenger::InstanceMessenger(TCPClient *_tcpClient, QObject *_parent) :
    QObject(_parent),
    m_pTCPClient(_tcpClient) {

    connect(m_pTCPClient, &TCPClient::received, this, &InstanceMessenger::onReceived);
}

InstanceMessenger::~InstanceMessenger() {
    delete m_pTCPClient;
}

void InstanceMessenger::sendHello() {
    // configure json package
    nlohmann::json jsonPhoneNumber;
    jsonPhoneNumber[COMMAND] = "hello";

    // send json
    m_pTCPClient->send(jsonPhoneNumber);
}

void InstanceMessenger::receiveWorld(nlohmann::json _json) {
    m_worldStdString = _json[RESULT];
    std::cout << m_worldStdString << std::endl;
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

void InstanceMessenger::sendRequestDialog(const QString &_userId) {
    // configure payload part
    nlohmann::json payload;
    payload[JWT] = m_jwt.toStdString();
    payload[USER_ID] = _userId.toStdString();

    // configure header
    nlohmann::json header;
    header[COMMAND] = CMD_REQUEST_DIALOG;

    // configure result package
    nlohmann::json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    // send json
    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::sendMessage(const QString &_message, const QString &_userId) {
    // create dialog item and add that to local storage
    m_dialogs[_userId].append(DialogItem(_message, _userId));

    // TODO change dialogs model

    // configure payload part
    nlohmann::json payload;
    payload[MESSAGE] = _message.toStdString();
    payload[USER_ID] = _userId.toStdString();

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

void InstanceMessenger::onReceived(nlohmann::json _receivedPackage) {
//    std::cout << _receivedPackage << std::endl;
//    receiveWorld(_receivedPackage);
//    receiveTempToken(_receivedPackage);
//    receiveJWT(_receivedPackage);
    receivePresence(_receivedPackage);
}

void InstanceMessenger::receiveTempToken(nlohmann::json &_json) {
    m_tempToken = QString::fromStdString(_json[RESULT]);
    std::cout << m_tempToken.toStdString() << std::endl;
}

void InstanceMessenger::receiveJWT(nlohmann::json &_json) {
    m_jwt = QString::fromStdString(_json[RESULT]);
    std::cout << m_jwt.toStdString() << std::endl;
}

void InstanceMessenger::receivePresence(nlohmann::json &_json) {
    m_authStatus = QString::fromStdString(_json[RESULT]);
    std::cout << m_authStatus.toStdString() << std::endl;
}

void InstanceMessenger::receiveContacts(nlohmann::json &_json) {
    const size_t amount = _json[RESULT].size();
    for (size_t i = 0; i < amount; ++i) {
        // parse phone and user_id
        QString phone = QString::fromStdString(_json[RESULT][i][PHONE]);
        QString id = QString::fromStdString(_json[RESULT][i][USER_ID]);

        // add new user item to local storage
        if (!m_contacts.contains(id)) {
            m_contacts[id] = UserItem(phone, id);
        }

        // TODO change user model
    }
}

void InstanceMessenger::receiveDialog(nlohmann::json &_json) {

}

void InstanceMessenger::receiveMessage(nlohmann::json &_json) {
    // parse message and user_id
    QString receivedMsg = QString::fromStdString(_json[MESSAGE]);
    QString receivedUserId = QString::fromStdString(_json[USER_ID]);

    // add new dialog item to local storage
    m_dialogs[receivedUserId].append(DialogItem(receivedMsg, receivedUserId));

    // TODO change dialogs model
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
