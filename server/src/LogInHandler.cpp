#include "LogInHandler.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "User.h"
#include "Logger.h"

LogInHandler::LogInHandler(Database *db, TokenAuthenticator *a) : EventHandlerGrantsAuthentication(db, a)
{

}

LogInHandler::~LogInHandler(){

}

void LogInHandler::_handle(HttpRequest &hmsg){
    Status s = Status::OK();
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Se corrobora el campo del username dentro de la request",INFO);
    std::string username = hmsg.getCampo("username");
    if(username == "") {
        log->Log("El campo esta vacio",INFO);
        return;
    }
    log->Log("El campo es valido",INFO);
    User user(db);
    user.setUsername(username);

    s = user.DBget();
    if(!s.ok()){
        hmsg.setResponse(s);
    }

    log->Log("Se corrobora el campo del password dentro de la request",INFO);
    std::string pass = hmsg.getCampo("password");
    if(pass == ""){
        log->Log("El campo esta vacio",INFO);
        return;
    }
    log->Log("El campo es valido",INFO);
    log->Log("Se fija si la contrasenia es correcta",INFO);
    bool pass_match = (pass.compare(user.getValueToString())) == 0;

    if(pass_match){ // cambiar por define
        log->Log("La contrasenia es correcta",INFO);
        std::string token = auth->createToken(username);
        hmsg.setResponse(Status::OK());
        hmsg.addValueToBody("conn_token", token);
    } else {
        log->Log("La contrasenia es incorrecta",INFO);
        hmsg.setResponse(Status::Aborted("password invalida"));
    }
}
