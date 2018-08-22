#ifndef MOCKTCPSERVER_H
#define MOCKTCPSERVER_H

#include <QObject>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoAll;
using ::testing::Return;

class TCPServer {
public:
    virtual ~TCPServer() {}
    virtual std::string send() = 0;
    virtual std::string onReceived(std::string msg) = 0;
};

class MockTCPServer : public TCPServer {
public:
    MOCK_METHOD0(send, std::string());
    MOCK_METHOD1(onReceived, std::string(std::string));
};

#endif // MOCKTCPSERVER_H
