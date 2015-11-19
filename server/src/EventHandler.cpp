#include "EventHandler.h"
#include "Logger.h"


EventHandler::EventHandler(Database *datab) : db(datab)
{
    //ctor
}

EventHandler::~EventHandler()
{
    //dtor
}

bool EventHandler::handle(HttpRequest &hmsg){
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Se evalua la httprequest y se verifica que tipo de handler debe tomarla",INFO);
    if(this->isMyRequest(hmsg)){
        this->_check_permission(hmsg);
        return true;
    } else {
        return false;
    }
}
