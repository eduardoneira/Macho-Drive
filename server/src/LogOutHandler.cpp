#include "LogOutHandler.h"
#include "rocksdb/status.h"
#include "HttpRequest.h"

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

    std::string username = hmsg.getUsername();
    bool existed = auth->removeToken(username);
    if(!existed){
        hmsg.setResponse(Status::Aborted("la sesion indicada no era valida"));
    }
    hmsg.setResponse(Status::OK());
}
