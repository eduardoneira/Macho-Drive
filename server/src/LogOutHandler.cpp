#include "LogOutHandler.h"
#include "rocksdb/status.h"

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
        hmsg.setResponse(Status::Aborted("la sesion indicada no era valida").ToString());
    }
    hmsg.setResponse(Status::OK().ToString());
}
