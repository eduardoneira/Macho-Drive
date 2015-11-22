#include "DefaultHandler.h"
#include "Logger.h"

DefaultHandler::DefaultHandler(Database *db, TokenAuthenticator *a) : EventHandlerIgnoresAuthentication(db, a)
{
    //ctor
}

DefaultHandler::~DefaultHandler()
{
    //dtor
}

bool DefaultHandler::isMyRequest(HttpRequest &hmsg){
    return true;
}

void DefaultHandler::_handle(HttpRequest &hmsg){
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Se recibio una httprequest no valida",ERROR);
    hmsg.setResponse(Status::InvalidArgument("El request recibido no es valido"));
}
