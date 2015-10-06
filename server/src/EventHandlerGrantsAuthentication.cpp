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
    this->_handle(hmsg); // medio feo, pero aca adentro me encargo de la autenticacion

}
