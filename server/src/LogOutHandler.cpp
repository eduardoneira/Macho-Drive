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

bool LogOutHandler::isMyRequest(HttpRequest &hmsg){
    // DELETE /sessions/'username' quiere decir log out
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::SESSIONS &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::DELETE){
        return true;
    }
    return false;
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
