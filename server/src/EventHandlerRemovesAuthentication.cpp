#include "EventHandlerRemovesAuthentication.h"
#include "rocksdb/status.h"
#include "HttpRequest.h"
#include "Logger.h"

EventHandlerRemovesAuthentication::EventHandlerRemovesAuthentication(Database *db, TokenAuthenticator *a) : EventHandler(db), auth(a)
{
    //ctor
}

EventHandlerRemovesAuthentication::~EventHandlerRemovesAuthentication()
{
    //dtor
}

void EventHandlerRemovesAuthentication::_check_permission(HttpRequest &hmsg){
    Server_Logger* log = Server_Logger::getInstance();
    std::string username = hmsg.getUsername();
    std::string token = hmsg.getConnToken();
    log->Log("Se recibe la httprequest y se verifica token",INFO);
    // ver si no existen los campos (va, va a fallar la autenticacion si no)
    if(this->auth->isValidToken(username, token)){
        this->_handle(hmsg);
    } else {
        log->Log("No era una sesion valida", WARNING);
        hmsg.setResponse(Status::Aborted("la sesion indicada no era valida"));
    }
}
