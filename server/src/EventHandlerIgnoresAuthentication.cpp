#include "EventHandlerIgnoresAuthentication.h"

EventHandlerIgnoresAuthentication::EventHandlerIgnoresAuthentication(Database *db, TokenAuthenticator *a) : EventHandler(db), auth(a)
{
    //ctor
}

EventHandlerIgnoresAuthentication::~EventHandlerIgnoresAuthentication()
{
    //dtor
}

void EventHandlerIgnoresAuthentication::handle(HttpRequest &hmsg){
    this->_handle(hmsg);
}
