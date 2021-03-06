#include "EventHandlerChecksAuthentication.h"
#include "rocksdb/status.h"
#include "Logger.h"
//#include "HttpRequest.h"

EventHandlerChecksAuthentication::EventHandlerChecksAuthentication(Database *db, TokenAuthenticator *a) : EventHandler(db), auth(a)
{
    //ctor
}

EventHandlerChecksAuthentication::~EventHandlerChecksAuthentication()
{
    //dtor
}

void EventHandlerChecksAuthentication::_check_permission(HttpRequest &hmsg){
    std::string username = hmsg.getUsername();
    std::string token = hmsg.getConnToken();
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Se recibe la httprequest y se verifica token",INFO);
    log->Log("El username de la request es : "+username,DEBUG);
    log->Log("El token de la request es : "+username,DEBUG);
    // ver si no existen los campos (va, va a fallar la autenticacion si no)
    if(this->auth->isValidToken(username, token)){
        this->_handle(hmsg);
    } else {
        hmsg.setResponse(Status::IOError("la token de autenticacion provista no es correcta"));
    }
}
