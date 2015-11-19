#include "EventHandlerIgnoresAuthentication.h"
#include "HttpRequest.h"

EventHandlerIgnoresAuthentication::EventHandlerIgnoresAuthentication(Database *db, TokenAuthenticator *a) : EventHandler(db), auth(a)
{
    //ctor
}

EventHandlerIgnoresAuthentication::~EventHandlerIgnoresAuthentication()
{
    //dtor
}

void EventHandlerIgnoresAuthentication::_check_permission(HttpRequest &hmsg){
    this->_handle(hmsg);
}
