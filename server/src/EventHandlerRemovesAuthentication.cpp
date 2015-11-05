#include "EventHandlerRemovesAuthentication.h"
#include "rocksdb/status.h"

EventHandlerRemovesAuthentication::EventHandlerRemovesAuthentication(Database *db, TokenAuthenticator *a) : EventHandler(db), auth(a)
{
    //ctor
}

EventHandlerRemovesAuthentication::~EventHandlerRemovesAuthentication()
{
    //dtor
}

void EventHandlerRemovesAuthentication::handle(HttpRequest &hmsg){
    std::string username = hmsg.getUsername();
    std::string token = hmsg.getConnToken();
    // ver si no existen los campos (va, va a fallar la autenticacion si no)
    if(this->auth->isValidToken(username, token)){
        this->_handle(hmsg);
    } else {
        hmsg.setResponse(Status::Aborted("la sesion indicada no era valida"));
    }
}
