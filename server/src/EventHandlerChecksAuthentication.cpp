#include "EventHandlerChecksAuthentication.h"
#include "rocksdb/status.h"

EventHandlerChecksAuthentication::EventHandlerChecksAuthentication(Database *db, TokenAuthenticator *a) : EventHandler(db), auth(a)
{
    //ctor
}

EventHandlerChecksAuthentication::~EventHandlerChecksAuthentication()
{
    //dtor
}

void EventHandlerChecksAuthentication::handle(HttpRequest &hmsg){
    std::string username = hmsg.getUsername();
    std::string token = hmsg.getConnToken();
    // ver si no existen los campos (va, va a fallar la autenticacion si no)
    if(this->auth->isValidToken(username, token)){
        this->_handle(hmsg);
    } else {
        hmsg.setResponse(Status::Aborted("la token de autenticacion provista no es correcta").ToString());
    }
}
