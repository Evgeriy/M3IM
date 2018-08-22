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
    payload[PHONE_NUMBER] = m_clientData.m_phoneNumber.toStdString();
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
    payload[CODE] = m_smsCode.toStdString();
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

void InstanceMessenger::sendPresence() {
    // configure payload part
    nlohmann::json payload;
    payload[JWT] = m_jwt.toStdString();

    // configure header
    nlohmann::json header;
    header[COMMAND] = CMD_PRESENCE;

    // configure result package
    nlohmann::json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    // send json
    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::addNewContact(const UserItem &_user) {
    if (!m_contacts.contains(_user.m_nikName)) {
        m_contacts[_user.m_nikName] = _user;
    }
}

void InstanceMessenger::addNewDialog(const DialogItem &_dialog) {
    if (!m_dialogs.contains(_dialog.m_dialogName)) {
        m_dialogs[_dialog.m_dialogName].append(_dialog);
    }
}

void InstanceMessenger::addNewMessage(const QString &_message, const QString &_from, const QString &_to) {
    QString to = _from == getNikName() ? _to : _from;
    if (m_dialogs.contains(to)) {
        m_dialogs[to].append(DialogItem(to, _to == getNikName(), _message, QDateTime::currentDateTime()));
    }
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
    m_authorizationStatus = QString::fromStdString(_json[RESULT]);
    std::cout << m_authorizationStatus.toStdString() << std::endl;
}

nlohmann::json InstanceMessenger::getJsonFromString(const QString &_strName, const QString &_str) {
    return getJsonFromString(_strName.toStdString(), _str.toStdString());
}

nlohmann::json InstanceMessenger::getJsonFromString(const std::string &_strName, const std::string &_str) {
    nlohmann::json retValue;
    retValue[_strName] = _str;
    return retValue;
}

nlohmann::json InstanceMessenger::getPhoneNumberJson() const {
    return getJsonFromString(QString::fromStdString(PHONE_NUMBER), getPhoneNumber());
}

nlohmann::json InstanceMessenger::getNikNameJson() const {
    return getJsonFromString(QString("nikName"), getNikName());
}

nlohmann::json InstanceMessenger::getFirstNameJson() const {
    return getJsonFromString(QString("firstName"), getFirstName());
}

nlohmann::json InstanceMessenger::getSecondNameJson() const {
    return getJsonFromString(QString("secondName"), getSecondName());
}


