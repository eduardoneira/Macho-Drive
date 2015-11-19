#include "UserGetHandler.h"
#include "UserMetadata.h"
#include "Logger.h"

UserGetHandler::UserGetHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

UserGetHandler::~UserGetHandler()
{
    //dtor
}

bool UserGetHandler::isMyRequest(HttpRequest &hmsg){
    // GET /users/'username' quiere decir pedir info del usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo UserGet",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::GET){
        log->Log("Confirma que es un Handler tipo UserGet",INFO);
        return true;
    }
    return false;
}

void UserGetHandler::_handle(HttpRequest &hmsg){
    Status s = Status::OK();

    std::string username = hmsg.getUsername();
    if(username == "") return;
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("El campo recibido por username es : "+username,DEBUG);

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);

    s = user_metadata.DBget();
    if(!s.ok()){
        hmsg.setResponse(s);
        return;
    }

    hmsg.setResponse(Status::OK(), user_metadata.getValueToString());
}
