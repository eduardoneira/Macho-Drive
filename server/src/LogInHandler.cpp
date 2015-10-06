#include "LogInHandler.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "User.h"

LogInHandler::LogInHandler(Database *db, TokenAuthenticator *a) : EventHandlerGrantsAuthentication(db, a)
{

}

LogInHandler::~LogInHandler(){

}

void LogInHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string username = hmsg.getCampo("username");

    User user(db);
    user.setUsername(username);

    s = user.DBget();
    // ver status

    bool pass_match = (hmsg.getCampo("password").compare(user.getValueToString())) == 0;

    if(pass_match){ // cambiar por define
        std::string token = auth->createToken(username);
        hmsg.addValueToBody("conn_token", token);
    } else {
        // informar contraseña invalida
    }
}
