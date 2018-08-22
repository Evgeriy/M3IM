#include <QCoreApplication>

#include "mock_tcp_server.hpp"
#include "tcp_client.h"
#include "instancemessenger.h"

#define TEST_MODE 1

//std::string getPhoneNumberJsonMsgpack(const std::string &_phoneNumber) {
//    std::string retValue = "";
//    json jsonNumber;
//    jsonNumber["phoneNumber"] = _phoneNumber;
//    json::to_msgpack(jsonNumber, retValue);
//    return retValue;
//}

json getCodeJson(const quint16 &_code) {
    json retValue;
    retValue[CODE] = _code;
    return retValue;
}

json getStatusJson(const bool &_status) {
    json retValue;
    retValue[AUTH_STATUS] = _status;
    return retValue;
}

//TEST(TCPClientTest, SendPhoneNumber) {
//    // create tcp client
//    TCPClient *client = new TCPClient();

//    // create messenger
//    InstanceMessenger *pMessenger = new InstanceMessenger(client);

//    // create mock server
//    MockTCPServer *mock_server = new MockTCPServer();

//    // set mock server to client
//    client->setMockTCPServer(mock_server);

//    QString number = "65025";
//    quint16 code = 65025;

//    pMessenger->setPhoneNumber(number);

//    // configure returned values
//    std::string phoneNumberJsonMsgpack = TCPClient::jsonToMsgpack(pMessenger->getPhoneNumberJson());
//    std::string codeNumberJsonMsgpack  = TCPClient::jsonToMsgpack(getCodeJson(code));

//    // mock behaviour settings
//    EXPECT_CALL(*mock_server, onReceived(phoneNumberJsonMsgpack));
//    EXPECT_CALL(*mock_server, send()).Times(1).WillRepeatedly(Return(codeNumberJsonMsgpack));

//    // call function
//    pMessenger->sendPhoneNumber();

//    // check results
//    EXPECT_EQ(pMessenger->m_smsCode, code);
//}

//TEST(TCPClientTest, SendCode) {
//    // create tcp client
//    TCPClient *client = new TCPClient();

//    // create messenger
//    InstanceMessenger *pMessenger = new InstanceMessenger(client);

//    // create mock server
//    MockTCPServer *mock_server = new MockTCPServer();

//    // set mock server to client
//    client->setMockTCPServer(mock_server);

//    QString authorizationToken = "authorization_token_here";
//    quint16 code = 65025;
//    pMessenger->m_smsCode = code;

//    // configure returned values
//    std::string authTokenJsonMsgpack   = TCPClient::jsonToMsgpack(InstanceMessenger::getJsonFromString(QString("auth_token"), authorizationToken));
//    std::string codeNumberJsonMsgpack  = TCPClient::jsonToMsgpack(getCodeJson(code));

//    // mock behaviour settings
//    EXPECT_CALL(*mock_server, onReceived(codeNumberJsonMsgpack));
//    EXPECT_CALL(*mock_server, send()).Times(1).WillOnce(Return(authTokenJsonMsgpack));

//    // call function
//    pMessenger->sendCode();

//    // check results
//    EXPECT_EQ(pMessenger->getAuthorizationToken(), authorizationToken);
//}

//TEST(TCPClientTest, SendAuthorizationToken) {
//    // create tcp client
//    TCPClient *client = new TCPClient();

//    // create messenger
//    InstanceMessenger *pMessenger = new InstanceMessenger(client);

//    // create mock server
//    MockTCPServer *mock_server = new MockTCPServer();

//    // set mock server to client
//    client->setMockTCPServer(mock_server);

//    QString authorizationToken = "authorization_token_here";
//    bool authorizationStatus = true;

//    pMessenger->setAuthorizationToken(authorizationToken);

//    // configure returned values
//    std::string authTokenJsonMsgpack   = TCPClient::jsonToMsgpack(InstanceMessenger::getJsonFromString(QString("auth_token"), authorizationToken));
//    std::string authStatusJsonMsgpack  = TCPClient::jsonToMsgpack(getStatusJson(authorizationStatus));

//    // mock behaviour settings
//    EXPECT_CALL(*mock_server, onReceived(authTokenJsonMsgpack));
//    EXPECT_CALL(*mock_server, send()).Times(1).WillOnce(Return(authStatusJsonMsgpack));

//    // call function
//    pMessenger->sendAuthorizationToken();

//    // check results
//    EXPECT_EQ(pMessenger->getAuthorizationStatus(), authorizationToken);
//}

TEST(TCPClientTest, HelloWorld) {
    // create tcp client
    TCPClient *client = new TCPClient();

    // create messenger
    InstanceMessenger *pMessenger = new InstanceMessenger(client);

    // create mock server
    MockTCPServer *mock_server = new MockTCPServer();

    // set mock server to client
    client->setMockTCPServer(mock_server);

    std::string result = "world";

    // configure returned values
    std::string commandHello = TCPClient::jsonToMsgpack(InstanceMessenger::getJsonFromString(QString::fromStdString(COMMAND), "hello"));
    std::string resultWorld   = TCPClient::jsonToMsgpack(InstanceMessenger::getJsonFromString(QString::fromStdString(RESULT), "world"));

    // mock behaviour settings
    EXPECT_CALL(*mock_server, onReceived(commandHello));
    EXPECT_CALL(*mock_server, send()).Times(1).WillOnce(Return(resultWorld));

    // call function
    pMessenger->sendHello();

    // check results
    EXPECT_EQ(pMessenger->m_worldStdString, result);
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

#if TEST_MODE
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
#else

    // create tcp client
    TCPClient *client = new TCPClient();

    // create messenger
    InstanceMessenger *pMessenger = new InstanceMessenger(client);
#endif

    return a.exec();
}
