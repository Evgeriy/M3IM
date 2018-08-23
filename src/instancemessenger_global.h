﻿#ifndef INSTANCEMESSENGER_GLOBAL_H
#define INSTANCEMESSENGER_GLOBAL_H

#include <string>

static const std::string CODE         = "code";
static const std::string AUTH_TOKEN   = "auth_token";
static const std::string AUTH_STATUS  = "auth_tatus";
static const std::string PHONE = "phone";
static const std::string TYPE         = "type";
static const std::string REGISTER     = "register";
static const std::string SENDER       = "sender";
static const std::string RECEIVER     = "receiver";
static const std::string MESSAGE      = "message";
static const std::string DATE_TIME    = "date_time";

static const std::string COMMAND = "command";
static const std::string CMD_REQUEST_CODE = "request_code";
static const std::string CMD_REQUEST_JWT  = "request_jwt";
static const std::string CMD_REQUEST_PRESENCE  = "presence";
static const std::string CMD_REQUEST_CONTACTS  = "contacts";
static const std::string CMD_REQUEST_DIALOG  = "dialog";
static const std::string CMD_SEND_MESSAGE  = "send_message";

static const std::string USER_ID  = "user_id";

static const std::string PAYLOAD = "payload";

static const std::string RESET_PASSWORD = "reset_password";
static const std::string RESTORE        = "restore";
static const std::string TOKEN          = "token";
static const std::string JWT            = "jwt";

static const std::string RESULT       = "result";

static const int CONTACT_LIST_REQUEST = 1;
static const int DIALOG_REQUEST       = 1;

#endif // INSTANCEMESSENGER_GLOBAL_H