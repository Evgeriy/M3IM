#include <QCoreApplication>

#include "mock_tcp_server.hpp"
#include "tcp_client.h"
#include "instancemessenger.h"

#define TEST_MODE 0

using namespace nlohmann;

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

//TEST(TCPClientTest, SendRequestCode) {
//    // create client, messenger, mock_server
//    TCPClient *client = new TCPClient();
//    InstanceMessenger *messenger = new InstanceMessenger(client);
//    MockTCPServer *mock_server = new MockTCPServer();

//    // set mock server to client
//    client->setMockTCPServer(mock_server);

//    QString phone  = "79081234567";
//    QString result = "3872115D34DF2106135E1F9E4107CAB5C170E14BC56B9D4F941C031153B8017F";

//    messenger->setPhoneNumber(phone);

//    json input = R"({
//            "command": "request_code",
//            "payload": {
//                "phone": "79081234567",
//                "type": "register"
//            }
//        })"_json;

//    json output = R"({"result": "3872115D34DF2106135E1F9E4107CAB5C170E14BC56B9D4F941C031153B8017F"})"_json;

//    // configure returned values
//    std::string inputMsgpack = TCPClient::jsonToMsgpack(input);
//    std::string outputMsgpack  = TCPClient::jsonToMsgpack(output);

//    // mock behaviour settings
//    EXPECT_CALL(*mock_server, onReceived(inputMsgpack));
//    EXPECT_CALL(*mock_server, send()).WillRepeatedly(Return(outputMsgpack));

//    // call function
//    messenger->sendRequestCode();

//    // check results
//    EXPECT_EQ(messenger->getTempToken(), result);

//    delete messenger;
//}

//TEST(TCPClientTest, SendRequestJWT) {
//    // create client, messenger, mock_server
//    TCPClient *client = new TCPClient();
//    InstanceMessenger *messenger = new InstanceMessenger(client);
//    MockTCPServer *mock_server = new MockTCPServer();

//    // set mock server to client
//    client->setMockTCPServer(mock_server);

//    QString code = "3802";
//    QString tempToken = "3872115D34DF2106135E1F9E4107CAB5C170E14BC56B9D4F941C031153B8017F";

//    QString result = "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9...";

//    messenger->setCode(code);
//    messenger->setTempToken(tempToken);

//    json input = R"({
//            "command": "request_jwt",
//            "payload": {
//                "type": "register"
//                "code": "3802",
//                "reset_password": false,
//                "restore": false,
//                "token": "3872115D34DF2106135E1F9E4107CAB5C170E14BC56B9D4F941C031153B8017F"
//            }
//        })"_json;

//    json output = R"({"result": "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9..."})"_json;

    // configure returned values
//    std::string inputMsgpack = TCPClient::jsonToMsgpack(input);
//    std::string outputMsgpack = TCPClient::jsonToMsgpack(output);

    // mock behaviour settings
//    EXPECT_CALL(*mock_server, onReceived(inputMsgpack));
//    EXPECT_CALL(*mock_server, send()).WillRepeatedly(Return(outputMsgpack));

    // call function
    //messenger->sendRequestJWT();

    // check results
    //EXPECT_EQ(messenger->getJWT(), result);

//    delete messenger;
//}

//TEST(TCPClientTest, SendPresence) {
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

//    pMessenger->setJWT(authorizationToken);

//    // configure returned values
//    std::string authTokenJsonMsgpack   = TCPClient::jsonToMsgpack(InstanceMessenger::getJsonFromString(QString::fromStdString(AUTH_TOKEN), authorizationToken));
//    std::string authStatusJsonMsgpack  = TCPClient::jsonToMsgpack(getStatusJson(authorizationStatus));

//    // mock behaviour settings
//    EXPECT_CALL(*mock_server, onReceived(authTokenJsonMsgpack));
//    EXPECT_CALL(*mock_server, send()).Times(1).WillOnce(Return(authStatusJsonMsgpack));

//    // call function
//    pMessenger->sendRequestJWT();

//    // check results
//    EXPECT_EQ(pMessenger->getAuthorizationStatus(), authorizationStatus);

//    delete pMessenger;
//}

//TEST(TCPClientTest, HelloWorld) {
//    // create tcp client
//    TCPClient *client = new TCPClient();

//    // create messenger
//    InstanceMessenger *pMessenger = new InstanceMessenger(client);

//    // create mock server
//    MockTCPServer *mock_server = new MockTCPServer();

//    // set mock server to client
//    client->setMockTCPServer(mock_server);

//    std::string result = "world";

//    // configure returned values
//    std::string commandHello = TCPClient::jsonToMsgpack(InstanceMessenger::getJsonFromString(QString::fromStdString(COMMAND), "hello"));
//    std::string resultWorld   = TCPClient::jsonToMsgpack(InstanceMessenger::getJsonFromString(QString::fromStdString(RESULT), "world"));

//    // mock behaviour settings
//    EXPECT_CALL(*mock_server, onReceived(commandHello));
//    EXPECT_CALL(*mock_server, send()).Times(1).WillOnce(Return(resultWorld));

//    // call function
//    pMessenger->sendHello();

//    // check results
//    EXPECT_EQ(pMessenger->m_worldStdString, result);

//    delete pMessenger;
//}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

#if TEST_MODE
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
#else

//    // setup client
//    TCPClient *client = new TCPClient();
//    client->connectToHost("192.168.8.102", 6000);

//    // setup messenger
//    InstanceMessenger *messenger = new InstanceMessenger(client);

//    // send hello
//    QString phone  = "79081234567";

//    messenger->setPhoneNumber(phone);


//    QString tempToken = "DF09085DADB710BA52E8908EA837C7DC3D3F92FEC216CEA07F32E18F660E844D";
//    QString code = "9564";
//    QString jwt = "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJNMyIsImV4cCI6MTU2NjA2MDAzNCwiaWF0IjoxNTM0OTU2MDM0LCJpc3MiOiJNMyIsImp0aSI6ImVkZGMzMTYwLTg2ZGEtNDIzMy05ZTM5LTg0ZjQ2MWZjY2JmMiIsIm5iZiI6MTUzNDk1NjAzMywic3ViIjoiVXNlcjoyMzAiLCJ0eXAiOiJhY2Nlc3MifQ.Icuy8kSrX1Mw74KOTdcajDiRiLp4wmxtM0BQpqFGsoNjJ__c_27u-a-4QwL71Uf3bYDPc47kctpg_6ikJFRIqw";

//    messenger->setTempToken(tempToken);
//    messenger->setCode(code);
//    messenger->setJWT(jwt);

//    messenger->sendPresence();


//    json payload;
//    payload[TYPE] = REGISTER;
//    payload[CODE] = "2323";
//    payload[RESET_PASSWORD] = false;
//    payload[RESTORE] = false;
//    payload[TOKEN] = "2323";

//    // configure header
//    json header;
//    header[COMMAND] = CMD_REQUEST_JWT;

//    // configure result package
//    json resultPackage;
//    resultPackage = header;
//    resultPackage[PAYLOAD] = payload;

//    std::string jj = resultPackage.dump();

//    std::cout << resultPackage << std::endl;
//    std::cout << R"" << std::endl;

#endif

    return a.exec();
}
