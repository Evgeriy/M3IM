#ifndef AUTHORIZATIONDATA_H
#define AUTHORIZATIONDATA_H

// QT
#include <QObject>

// CPP
#include <cstdio>

// FORWARD DECLARATIONS
QT_FORWARD_DECLARE_CLASS(QTcpSocket)
QT_FORWARD_DECLARE_CLASS(MockTCPServer)

// ANOTHERS
#include "nlohmann/json.hpp"

class TCPClient : public QObject {
    Q_OBJECT

public:
    void setMockTCPServer(MockTCPServer *_server);

public:
    explicit TCPClient(QObject *_parent = nullptr);
    virtual ~TCPClient();

signals:
    void closed();
    void received(nlohmann::json);

public:
    virtual void connectToHost(const QString &_hostAddressString, const quint16 &_port);
    virtual void send(const nlohmann::json &_sendedPackage);

private slots:
    virtual void onConnected();
    virtual void onReceived();

public:
    static std::string jsonToMsgpack(const nlohmann::json &_json);

private:
    QTcpSocket  *m_pSocket{nullptr};
    MockTCPServer *m_pMockTCPServer{nullptr};
};


#endif
