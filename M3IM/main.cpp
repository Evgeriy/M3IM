// QT
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QFont>

// ANOTHERS
#include "mock_tcp_server.hpp"

// TEST BUILD SETTINGS
#define TEST_MODE             0
#define TEST_REQUEST_CODE     1
#define TEST_REQUEST_JWT      1
#define TEST_REQUEST_PRESENCE 1
#define TEST_REQUEST_CONTACTS 1
#define TEST_REQUEST_DIALOG   0
#define TEST_REQUEST_MESSAGE  1

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
    #if TEST_REQUEST_MESSAGE
        #include "unit_send_request_message.hpp"
    #endif
#endif

// CONSOLE MODE SETTINGS
#define CONSOLE_MODE 0

// QML MODE SETTINGS
#define QML_MODE 1

#if QML_MODE
    #include <QGuiApplication>
    #include <QQmlContext>
    #include <QQmlComponent>
    #include <QQmlApplicationEngine>

    #include "contacts_model.hpp"
    #include "dialogs_model.hpp"
#endif

int main(int argc, char *argv[]) {

#if TEST_MODE
    QCoreApplication app(argc, argv);

    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();

#elif CONSOLE_MODE
    QCoreApplication app(argc, argv);

#elif QML_MODE
    // create gui app
    QGuiApplication app(argc, argv);

    // create qml engine obj
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    // create tcp client and connection
    TCPClient *client = new TCPClient();
    client->connectToHost("192.168.0.113", 6000);

    InstanceMessenger *messenger = new InstanceMessenger(client);

    // set messenger and models to qml context
    engine.rootContext()->setContextProperty("client", messenger);
    engine.rootContext()->setContextProperty("contacts", messenger->getContactsModel());
    engine.rootContext()->setContextProperty("dialog", messenger->getDialogModel());

#endif

    return app.exec();
}
