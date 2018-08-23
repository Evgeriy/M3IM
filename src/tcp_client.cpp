#include "tcp_client.h"

// QT
#include <QTime>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>

// CPP/STL
#include <cstdio>
#include <iostream>

// ANOTHERS
//#include "../tests/mock_tcp_server.hpp"

void TCPClient::setMockTCPServer(AbstractTCPServer *_server) {
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

void TCPClient::send(const nlohmann::json &_sendedPackage) {
    // pack to binary
    std::string packed = "0000" + jsonToMsgpack(_sendedPackage);

    if (m_pMockTCPServer != nullptr) {
        // send to mock server and receive
        m_pMockTCPServer->onReceived(packed);
        onReceived();
    } else {
        // print debug
        std::cout << "[Send] Json package: " << _sendedPackage << std::endl;
        std::cout << "[Send] Msgpack package: " << packed << std::endl;

        // send to server
        m_pSocket->write(packed.c_str(), packed.size());
    }
}

void TCPClient::onConnected() {
    // print debug
    std::cout << "Client IP: " << m_pSocket->localAddress().toString().toStdString() << std::endl;
    std::cout << "Server IP: " << m_pSocket->peerAddress().toString().toStdString() << std::endl;
}

void TCPClient::onReceived() {
    std::string receivedPackageStdString = "";
    if (m_pMockTCPServer != nullptr) {
        // use send() of mock server
        receivedPackageStdString = m_pMockTCPServer->send();
    } else {
        // read all from socket
        receivedPackageStdString = m_pSocket->readAll().toStdString();
    }

    // print debug
    std::cout << "[Receive] Raw package: " << receivedPackageStdString << std::endl;

    // remove "0000"
    receivedPackageStdString = receivedPackageStdString.substr(4, receivedPackageStdString.size() - 4);

    // print debug
    std::cout << "[Receive] Msgpack package: " << receivedPackageStdString << std::endl;

    // unpack to json
    nlohmann::json unpacked = nlohmann::json::from_msgpack(receivedPackageStdString);

    // print debug
    std::cout << "[Receive] Json package: " << unpacked << std::endl;

    emit received(unpacked);
}

std::string TCPClient::jsonToMsgpack(const nlohmann::json &_json) {
    std::string retValue = "";
    nlohmann::json::to_msgpack(_json, retValue);
    return retValue;
}
