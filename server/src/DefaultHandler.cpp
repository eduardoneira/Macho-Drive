#include "DefaultHandler.h"

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
    hmsg.setResponse(Status::Aborted("El request recibido no es valido"));
}
