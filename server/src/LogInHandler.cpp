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

bool LogInHandler::isMyRequest(HttpRequest &hmsg){
    // POST /sessions/ quiere decir log in
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo LogIn",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::SESSIONS &&
        hmsg.getUriCantCampos() == 1 &&
        hmsg.getUriType() ==  HttpRequest::COLLECTION_URI &&
        hmsg.getMethod() == HttpRequest::POST){
        log->Log("Confirma que es un Handler tipo Login",INFO);
        return true;
    }
    return false;
}

void LogInHandler::_handle(HttpRequest &hmsg){
    Status s = Status::OK();
    Server_Logger* log = Server_Logger::getInstance();
    std::string username = hmsg.getCampo("username");
    log->Log("El campo recibido por username es : "+username,DEBUG);
    if(username == ""){
        hmsg.setResponse(Status::InvalidArgument());
        return;
    }
    User user(db);
    user.setUsername(username);

    s = user.DBget();
    if(!s.ok()){
        hmsg.setResponse(s);
    }

    std::string pass = hmsg.getCampo("password");
    log->Log("El campo recibido por password es : "+pass,DEBUG);
    if(pass == ""){
        return;
    }
    log->Log("Se fija si la contrasenia es correcta",INFO);
    bool pass_match = (pass.compare(user.getValueToString())) == 0;

    if(pass_match){ // cambiar por define
        log->Log("La contrasenia es correcta",INFO);
        std::string token = auth->createToken(username);
        hmsg.setResponse(Status::OK());
        hmsg.addValueToBody("conn_token", token);
    } else {
        log->Log("La contrasenia es incorrecta",WARNING);
        hmsg.setResponse(Status::Corruption("password invalida"));
    }
}
