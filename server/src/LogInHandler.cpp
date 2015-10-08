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
    Status s = Status::OK();

    std::string username = hmsg.getCampo("username");
    if(username == "") return;

    User user(db);
    user.setUsername(username);

    s = user.DBget();
    if(!s.ok()){
        hmsg.setResponse(s.ToString());
    }

    std::string pass = hmsg.getCampo("password");
    if(pass == ""){
        return;
    }

    bool pass_match = (pass.compare(user.getValueToString())) == 0;

    if(pass_match){ // cambiar por define
        std::string token = auth->createToken(username);
        hmsg.addValueToBody("conn_token", token);
    } else {
        hmsg.setResponse(Status::Aborted("password invalida").ToString());
    }
}
