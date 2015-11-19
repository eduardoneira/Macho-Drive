#include "EventHandler.h"


EventHandler::EventHandler(Database *datab) : db(datab)
{
    //ctor
}

EventHandler::~EventHandler()
{
    //dtor
}

bool EventHandler::handle(HttpRequest &hmsg){
    if(this->isMyRequest(hmsg)){
        this->_check_permission(hmsg);
        return true;
    } else {
        return false;
    }
}
