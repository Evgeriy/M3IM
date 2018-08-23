#include "mock_tcp_server.hpp"

TEST(TCPClientTest, SendPresence) {
    // create client, messenger, mock_server
    TCPClient *client = new TCPClient();
    InstanceMessenger *messenger = new InstanceMessenger(client);
    MockTCPServer *mock_server = new MockTCPServer();

    // set mock server to client
    client->setMockTCPServer(mock_server);

    QString jwt = "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9...";
    QString auth_status = "ok";

    messenger->setJWT(jwt);

    nlohmann::json input = R"({
                           "command": "presence",
                           "payload": {
                               "jwt": "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9..."
                           }
                       })"_json;

    nlohmann::json output = R"({"result": "ok"})"_json;

    // configure returned values
    std::string inputMsgpack   = "0000" + TCPClient::jsonToMsgpack(input);
    std::string outputMsgpack  = "0000" + TCPClient::jsonToMsgpack(output);

    // mock behaviour settings
    EXPECT_CALL(*mock_server, onReceived(inputMsgpack));
    EXPECT_CALL(*mock_server, send()).WillRepeatedly(Return(outputMsgpack));

    // call function
    messenger->sendRequestPresence();

    std::cout << std::endl << std::endl;
    std::cout << "[ACTUAL   TEMP TOKEN] " << messenger->getAuthStatus().toStdString() << std::endl;
    std::cout << "[EXPECTED TEMP TOKEN] " << auth_status.toStdString() << std::endl;

    // check results
    EXPECT_EQ(messenger->getAuthStatus(), auth_status);

    delete messenger;
}
