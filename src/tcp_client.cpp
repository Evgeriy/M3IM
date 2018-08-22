﻿#include "tcp_client.h"
#include "mock_tcp_server.hpp"

#include <QTime>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>

using namespace nlohmann;

void TCPClient::setMockTCPServer(MockTCPServer *_server) {
    m_pMockTCPServer = _server;
}

TCPClient::TCPClient(QObject *_parent) :
    QObject(_parent),
    m_pSocket(nullptr) {

}

TCPClient::~TCPClient() {
    delete m_pSocket;
    delete m_pMockTCPServer;
}

void TCPClient::connectToHost(const QString &_hostAddressString, const quint16 &_port) {
    // make connection
    m_pSocket = new QTcpSocket(this);
    m_pSocket->connectToHost(_hostAddressString, _port);

    // connect signals and slots
    connect(m_pSocket, &QTcpSocket::connected, this, &TCPClient::onConnected);
    connect(m_pSocket, &QTcpSocket::disconnected, this, &TCPClient::closed);
    connect(m_pSocket, &QTcpSocket::readyRead, this, &TCPClient::onReceived);
}

void TCPClient::send(const json &_sendedPackage) {
    // pack to binary
    std::string packed = "0000" + jsonToMsgpack(_sendedPackage);

    if (m_pMockTCPServer != nullptr) {
        m_pMockTCPServer->onReceived(packed);
        onReceived();
    } else {
        // send to server
        std::cout << packed << std::endl;
        m_pSocket->write(packed.c_str(), packed.size());
    }
}

void TCPClient::onConnected() {
    // print debug
    qDebug() << "Client IP:" << m_pSocket->localAddress().toString() <<
                "Server IP:" << m_pSocket->peerAddress().toString();
}

void TCPClient::onReceived() {
    std::string receivedPackageStdString = "";
    if (m_pMockTCPServer != nullptr) {
        // use send() of mock server
        receivedPackageStdString = m_pMockTCPServer->send();
    } else {
        // read received package
        std::cout << receivedPackageStdString << std::endl;
        receivedPackageStdString = m_pSocket->readAll().toStdString();
        receivedPackageStdString = receivedPackageStdString.substr(4, receivedPackageStdString.size() - 4);
        std::cout << receivedPackageStdString << std::endl;
    }

    // unpack to json
    json unpacked = json::from_msgpack(receivedPackageStdString);
    std::cout << unpacked << std::endl;
    emit received(unpacked);
}

std::string TCPClient::jsonToMsgpack(const json &_json) {
    std::string retValue = "";
    json::to_msgpack(_json, retValue);
    return retValue;
}
