#include "instancemessenger.h"

#include <iostream>

using namespace nlohmann;

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
    json jsonPhoneNumber;
    jsonPhoneNumber[COMMAND] = "hello";

    // send json
    m_pTCPClient->send(jsonPhoneNumber);
}

void InstanceMessenger::receiveWorld(json _json) {
    m_worldStdString = _json[RESULT];
    std::cout << m_worldStdString << std::endl;
}

void InstanceMessenger::sendRequestCode() {
    // configure payload part
    json payload;
    payload[PHONE_NUMBER] = m_clientData.m_phoneNumber.toStdString();
    payload[TYPE] = REGISTER;

    // configure header
    json header;
    header[COMMAND] = CMD_REQUEST_CODE;

    // configure result package
    json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    // send json
    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::sendRequestJWT() {
    // configure payload part
    json payload;
    payload[TYPE] = REGISTER;
    payload[CODE] = m_smsCode.toStdString();
    payload[RESET_PASSWORD] = false;
    payload[RESTORE] = false;
    payload[TOKEN] = m_tempToken.toStdString();

    // configure header
    json header;
    header[COMMAND] = CMD_REQUEST_JWT;

    // configure result package
    json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    // send json
    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::sendPresence() {
    // configure payload part
    json payload;
    payload[JWT] = m_jwt.toStdString();

    // configure header
    json header;
    header[COMMAND] = CMD_PRESENCE;

    // configure result package
    json resultPackage;
    resultPackage = header;
    resultPackage[PAYLOAD] = payload;

    // send json
    m_pTCPClient->send(resultPackage);
}





void InstanceMessenger::sendMessage(const QString &_to, const QString &_message) {
    // create new dialog item and save that to local storage
    addNewMessage(_message, getNikName(), _to);

    // send last dialog item to server
    DialogItem lastDialog = m_dialogs[_to].last();

    // configure json package
    json jsonMessage;
    jsonMessage[AUTH_TOKEN] = m_jwt.toStdString();
    jsonMessage[SENDER]     = getNikName().toStdString();
    jsonMessage[RECEIVER]   = lastDialog.m_dialogName.toStdString();
    jsonMessage[MESSAGE]    = lastDialog.m_message.toStdString();
    jsonMessage[DATE_TIME]  = lastDialog.m_dateTime.toString().toStdString();

    // send json
    m_pTCPClient->send(jsonMessage);
}

void InstanceMessenger::sendRequestByCommand(const int &_command) {
    // configure json package
    json jsonCommand;
    jsonCommand[AUTH_TOKEN] = m_jwt.toStdString();  // set auth token
    jsonCommand[COMMAND]    = _command;                            // set command

    // send json
    m_pTCPClient->send(jsonCommand);
}

void InstanceMessenger::sendRequestForContactList() {
    sendRequestByCommand(CONTACT_LIST_REQUEST);
}

void InstanceMessenger::sendRequestForDialog() {
    sendRequestByCommand(DIALOG_REQUEST);
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

void InstanceMessenger::onReceived(json _receivedPackage) {
//    std::cout << _receivedPackage << std::endl;
//    receiveWorld(_receivedPackage);
//    receiveTempToken(_receivedPackage);
//    receiveJWT(_receivedPackage);
    receivePresence(_receivedPackage);
}

void InstanceMessenger::receiveTempToken(json &_json) {
    m_tempToken = QString::fromStdString(_json[RESULT]);
    std::cout << m_tempToken.toStdString() << std::endl;
}

void InstanceMessenger::receiveJWT(json &_json) {
    m_jwt = QString::fromStdString(_json[RESULT]);
    std::cout << m_jwt.toStdString() << std::endl;
}

void InstanceMessenger::receivePresence(json &_json) {
    m_authorizationStatus = QString::fromStdString(_json[RESULT]);
    std::cout << m_authorizationStatus.toStdString() << std::endl;
}


void InstanceMessenger::receiveCode(json &_json) {
    // m_smsCode = _json[CODE];
}

void InstanceMessenger::receiveAuthorizationToken(json &_json) {
    m_jwt = QString::fromStdString(_json[AUTH_TOKEN]);
}

void InstanceMessenger::receiveAuthorizationStatus(json &_json) {
//    m_authorizationStatus = _json[AUTH_STATUS];
}

void InstanceMessenger::receiveContactList(json &_json) {

}

void InstanceMessenger::receiveDialog(json &_json) {

}

void InstanceMessenger::receiveNewMessage(json &_json) {
    QString sender   = QString::fromStdString(_json[SENDER]);
    QString receiver = QString::fromStdString(_json[RECEIVER]);
    QString message  = QString::fromStdString(_json[MESSAGE]);
    QString datetime = QString::fromStdString(_json[DATE_TIME]);

    addNewMessage(message, sender, receiver);
}

json InstanceMessenger::getJsonFromString(const QString &_strName, const QString &_str) {
    return getJsonFromString(_strName.toStdString(), _str.toStdString());
}

json InstanceMessenger::getJsonFromString(const std::string &_strName, const std::string &_str) {
    json retValue;
    retValue[_strName] = _str;
    return retValue;
}

json InstanceMessenger::getPhoneNumberJson() const {
    return getJsonFromString(QString::fromStdString(PHONE_NUMBER), getPhoneNumber());
}

json InstanceMessenger::getNikNameJson() const {
    return getJsonFromString(QString("nikName"), getNikName());
}

json InstanceMessenger::getFirstNameJson() const {
    return getJsonFromString(QString("firstName"), getFirstName());
}

json InstanceMessenger::getSecondNameJson() const {
    return getJsonFromString(QString("secondName"), getSecondName());
}


