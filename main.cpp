// QT
#include <QCoreApplication>

// ANOTHERS
#include "mock_tcp_server.hpp"

// TEST BUILD SETTINGS
#define TEST_MODE             1
#define TEST_REQUEST_CODE     0
#define TEST_REQUEST_JWT      0
#define TEST_REQUEST_PRESENCE 0
#define TEST_REQUEST_CONTACTS 1
#define TEST_REQUEST_DIALOG   0

#if TEST_MODE
    #if TEST_REQUEST_CODE
        #include "unit_send_request_code.hpp"
    #endif
    #if TEST_REQUEST_JWT
        #include "unit_send_request_jwt.hpp"
    #endif
    #if TEST_REQUEST_PRESENCE
        #include "unit_send_request_presence.hpp"
    #endif
    #if TEST_REQUEST_CONTACTS
        #include "unit_send_request_contacts.hpp"
    #endif
    #if TEST_REQUEST_DIALOG
        #include "unit_send_request_dialog.hpp"
    #endif
#endif

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

#if TEST_MODE
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
#else


#endif

    return a.exec();
}
