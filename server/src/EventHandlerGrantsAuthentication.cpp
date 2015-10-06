#include "EventHandlerGrantsAuthentication.h"

EventHandlerGrantsAuthentication::EventHandlerGrantsAuthentication(Database *db, TokenAuthenticator *a) : EventHandler(db), auth(a)
{
    //ctor
}

EventHandlerGrantsAuthentication::~EventHandlerGrantsAuthentication()
{
    //dtor
}

void EventHandlerGrantsAuthentication::handle(HttpRequest &hmsg){
    this->_handle(hmsg);

    if(hmsg.getStatusCode() == 200){ // cambiar por define
        std::string username = hmsg.getCampo("username");
        std::string token = auth->createToken(username);
        hmsg.addValueToBody("conn_token", token);
    }
}
