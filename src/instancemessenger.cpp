﻿#include "instancemessenger.h"

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

//    m_clientData.m_id = "111";

//    UserItem first("First", "123");
//    m_contacts[first.m_id] = first;

//    UserItem second("Second", "222");
//    m_contacts[second.m_id] = second;

//    m_pContactsModel->addContact(first);
//    m_pContactsModel->addContact(second);

//    DialogItem fDialog("First dialog - first message. First dialog - first message. First dialog - first message. First dialog - first message.", "111");
//    DialogItem sDialog("First dialog - second message. First dialog - second message. First dialog - second message. First dialog - second message.", "123");

//    DialogItem fDialog2("Second dialog - first message. Second dialog - first message. Second dialog - first message. Second dialog - first message", "111");
//    DialogItem sDialog2("Second dialog - second message. Second dialog - second message. Second dialog - second message", "222");

//    m_dialogs["123"].append(fDialog);
//    m_dialogs["123"].append(sDialog);

//    m_dialogs["222"].append(fDialog2);
//    m_dialogs["222"].append(sDialog2);


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

std::string InstanceMessenger::getCommand(const nlohmann::json &_json) const {
    return _json[COMMAND];
}

std::string InstanceMessenger::getDescription(const nlohmann::json &_json) const {
    return _json[DESCRIPTION];
}

int InstanceMessenger::getCode(const nlohmann::json &_json) const {
    return _json[CODE];
}


void InstanceMessenger::onReceived(nlohmann::json _receivedPackage) {
    switch(getCode(_receivedPackage)) {
    case OK:
        std::cout << "[InstanceMessenger::onReceived] " << "Response code " << OK << std::endl;

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

    processResponseToContacts(_receivedPackage);
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

        UserItem userItem(phone, id);

        // add new user item to local storage
        if (!m_contacts.contains(id)) {
            m_contacts[id] = userItem;
        }

        // change model
        m_pContactsModel->addContact(userItem);
    }
}

void InstanceMessenger::processResponseToDialog(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processResponseToDialog] " << "Json: " << _json << std::endl;
}

void InstanceMessenger::processResponseToSendMessage(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processResponseToSendMessage] " << "Json: " << _json << std::endl;
}

void InstanceMessenger::processReceivedMessage(nlohmann::json &_json) {
    std::cout << "[InstanceMessenger::processReceivedMessage] " << "Json: " << _json << std::endl;

    nlohmann::json payload = _json[PAYLOAD];

    QString receivedMsg = QString::fromStdString(payload[MESSAGE]);

    int userId = 0;
    if (payload[USER_ID].type() == nlohmann::detail::value_t::string) {
        std::string idStr = payload[USER_ID];
        userId = std::atoi(idStr.c_str());
    } else if (payload[USER_ID].type() == nlohmann::detail::value_t::number_integer) {
        userId = payload[USER_ID];
    }

    DialogItem dialogItem(receivedMsg, userId);

    // add new dialog item to local storage
    m_dialogs[userId].append(dialogItem);

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

        sendRequestPresence();
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
