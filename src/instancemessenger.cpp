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

    m_pContactsModel = new ContactsModel();
    m_pDialogModel = new DialogModel();

    m_clientData.m_id = "111";

    UserItem first("First", "123");
    m_contacts[first.m_id] = first;

    UserItem second("Second", "222");
    m_contacts[second.m_id] = second;

    m_pContactsModel->addContact(first);
    m_pContactsModel->addContact(second);

    DialogItem fDialog("First dialog - first message. First dialog - first message. First dialog - first message. First dialog - first message.", "111");
    DialogItem sDialog("First dialog - second message. First dialog - second message. First dialog - second message. First dialog - second message.", "123");

    DialogItem fDialog2("Second dialog - first message. Second dialog - first message. Second dialog - first message. Second dialog - first message", "111");
    DialogItem sDialog2("Second dialog - second message. Second dialog - second message. Second dialog - second message", "222");

    m_dialogs["123"].append(fDialog);
    m_dialogs["123"].append(sDialog);

    m_dialogs["222"].append(fDialog2);
    m_dialogs["222"].append(sDialog2);


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

    std::cout << resultPackage << std::endl;

    // send json
   // m_pTCPClient->send(resultPackage);
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

    m_pDialogModel->setDialog(m_dialogs[_userId]);

    // send json
//    m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::sendMessage(const QString &_message, const QString &_userId) {
    // create dialog item and add that to local storage
    DialogItem dialogItem(_message, m_clientData.m_id);
    m_dialogs[_userId].append(dialogItem);

    // Change model
    m_pDialogModel->addDialogItem(dialogItem);

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
    //m_pTCPClient->send(resultPackage);
}

void InstanceMessenger::onReceived(nlohmann::json _receivedPackage) {
//    std::cout << _receivedPackage << std::endl;
//    receiveWorld(_receivedPackage);
//    receiveTempToken(_receivedPackage);
//    receiveJWT(_receivedPackage);
//    receivePresence(_receivedPackage);
    receiveContacts(_receivedPackage);
}

void InstanceMessenger::receiveTempToken(nlohmann::json &_json) {
    m_tempToken = QString::fromStdString(_json[RESULT]);
}

void InstanceMessenger::receiveJWT(nlohmann::json &_json) {
    m_jwt = QString::fromStdString(_json[RESULT]);
    writeJWTToFile();
}

void InstanceMessenger::receivePresence(nlohmann::json &_json) {
    m_authStatus = QString::fromStdString(_json[RESULT]);
}

void InstanceMessenger::receiveContacts(nlohmann::json &_json) {
    const size_t amount = _json[RESULT].size();
    for (size_t i = 0; i < amount; ++i) {
        // parse phone and user_id
        QString phone = QString::fromStdString(_json[RESULT][i][PHONE]);
        QString id = QString::fromStdString(_json[RESULT][i][USER_ID]);
        UserItem userItem(phone, id);

        // add new user item to local storage
        if (!m_contacts.contains(id)) {
            m_contacts[id] = userItem;
        }

        // TODO change user model
        m_pContactsModel->addContact(userItem);
    }
}

void InstanceMessenger::receiveDialog(nlohmann::json &_json) {

}

void InstanceMessenger::receiveMessage(nlohmann::json &_json) {
    // parse message and user_id
    QString receivedMsg = QString::fromStdString(_json[MESSAGE]);
    QString receivedUserId = QString::fromStdString(_json[USER_ID]);
    DialogItem dialogItem(receivedMsg, receivedUserId);

    // add new dialog item to local storage
    m_dialogs[receivedUserId].append(dialogItem);

    // Change model
    m_pDialogModel->addDialogItem(dialogItem);
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
