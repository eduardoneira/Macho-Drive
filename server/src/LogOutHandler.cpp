#include "LogOutHandler.h"
#include "rocksdb/status.h"
#include "HttpRequest.h"
#include "Logger.h"

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
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo LogOut",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::SESSIONS &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::DELETE){
        log->Log("Confirma que es un Handler tipo LogOut",INFO);
        return true;
    }
    return false;
}

void LogOutHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string username = hmsg.getUsername();
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("El campo recibido por username es : "+username,DEBUG);
    bool existed = auth->removeToken(username);
    if(!existed){
        hmsg.setResponse(Status::IOError(), "Invalid or expired session");
    }
    hmsg.setResponse(Status::OK(), "Logging out");
}
