#ifndef AUTHORIZATIONDATA_H
#define AUTHORIZATIONDATA_H

#include <QObject>
#include <QTime>
#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QAbstractListModel>

#include "mock_tcp_server.hpp"

#include "nlohmann/json.hpp"
using namespace nlohmann;

class TCPClient : public QObject {
    Q_OBJECT

public:
    void setMockTCPServer(MockTCPServer *_server);

public:
    explicit TCPClient(QObject *_parent = nullptr);
    virtual ~TCPClient();

signals:
    void closed();
    void received(json);

public:
    virtual void connectToHost(const QString &_hostAddressString, const quint16 &_port);
    virtual void send(const json &_sendedPackage);

private slots:
    virtual void onConnected();
    virtual void onReceived();

public:
    static std::string jsonToMsgpack(const json &_json);

private:
    QTcpSocket  *m_pSocket{nullptr};
    MockTCPServer *m_pMockTCPServer{nullptr};
};


#endif
