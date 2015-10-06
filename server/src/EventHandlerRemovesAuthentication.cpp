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
    this->_handle(hmsg);

    if(/*hmsg->getStatusCode()*/true){
        std::string username = hmsg.getCampo("username");
        bool existed = auth->removeToken(username);
        if(!existed){
            //informar o loggear que no existia el usuario
        }
    }
}
