#include "SignUpHandler.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "User.h"
#include "UserMetadata.h"
#include "Logger.h"

SignUpHandler::SignUpHandler(Database *db, TokenAuthenticator *a) : EventHandlerIgnoresAuthentication(db, a)
{

}

SignUpHandler::~SignUpHandler(){

}

void SignUpHandler::_handle(HttpRequest &hmsg){
    Status s = Status::OK();

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Se corrobora el campo del username dentro de la request",INFO);
    std::string usuario = hmsg.getCampo("username");
    if(usuario == "") {
        log->Log("El campo esta vacio",INFO);
        return;
    }
    std::string password = hmsg.getCampo("password");
    if(password == "") {
        log->Log("El campo es valido",INFO);
        return;}

    User user(db);
    user.setUsername(usuario);
    user.setPassword(password);
    s = user.DBcreate();
    // ver status
    hmsg.setResponse(s);
}
