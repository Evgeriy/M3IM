#ifndef FAKE_TCP_CLIENT_H
#define FAKE_TCP_CLIENT_H

#include "mock_tcp_server.hpp"
#include "tcp_client.h"

class FakeTCPClient : public TCPClient {
    Q_OBJECT

public:
    explicit FakeTCPClient(QObject *_parent = nullptr);
    ~FakeTCPClient() override;

public:
    void setMockTCPServer(MockTCPServer *_server);

public:
    virtual void connectToHost(const QString &_hostAddressString, const quint16 &_port) override;
    virtual void send(const json &_sendedPackage) override;

private slots:
    virtual void onConnected() override;
    virtual void onReceived() override;

private:
    MockTCPServer *m_pMockTCPServer{nullptr};
};

#endif // FAKE_TCP_CLIENT_H

FakeTCPClient::FakeTCPClient(QObject *_parent) :
    TCPClient(_parent) {

}

FakeTCPClient::~FakeTCPClient() {
    delete m_pMockTCPServer;
}

void FakeTCPClient::setMockTCPServer(MockTCPServer *_server) {
    m_pMockTCPServer = _server;
}

void FakeTCPClient::connectToHost(const QString &_hostAddressString, const quint16 &_port) {

}

void FakeTCPClient::send(const json &_sendedPackage) {
    // pack to binary
    std::string packed = "";
    json::to_msgpack(_sendedPackage, packed);

    m_pMockTCPServer->onReceived(packed);
}

void FakeTCPClient::onConnected() {

}

void FakeTCPClient::onReceived() {
    // read received package
    std::string receivedPackageStdString = m_pMockTCPServer->send();

    // TODO: header/payload parsing

    // unpack to json;
    json unpacked = json::from_msgpack(receivedPackageStdString);
    emit received(unpacked);
}
