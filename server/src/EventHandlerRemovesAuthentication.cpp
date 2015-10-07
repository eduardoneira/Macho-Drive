#include "EventHandlerRemovesAuthentication.h"

EventHandlerRemovesAuthentication::EventHandlerRemovesAuthentication(Database *db, TokenAuthenticator *a) : EventHandler(db), auth(a)
{
    //ctor
}

EventHandlerRemovesAuthentication::~EventHandlerRemovesAuthentication()
{
    //dtor
}

void EventHandlerRemovesAuthentication::handle(HttpRequest &hmsg){
    std::string username = hmsg.getCampo("username");
    std::string token = hmsg.getCampo("conn_token");
    // ver si no existen los campos (va, va a fallar la autenticacion si no)
    if(this->auth->isValidToken(username, token)){
        this->_handle(hmsg);
    } else {
        // setear respuesta de error de autenticacion
    }
}