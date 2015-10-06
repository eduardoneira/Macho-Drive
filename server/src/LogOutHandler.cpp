#include "LogOutHandler.h"

LogOutHandler::LogOutHandler(Database *db, TokenAuthenticator *a) : EventHandlerRemovesAuthentication(db, a)
{
    //ctor
}

LogOutHandler::~LogOutHandler()
{
    //dtor
}

void LogOutHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string username = hmsg.getCampo("username");
    bool existed = auth->removeToken(username);
    if(!existed){
        //informar o loggear que no existia el usuario
    }
}
