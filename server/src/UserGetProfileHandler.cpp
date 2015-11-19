#include "UserGetProfileHandler.h"
#include "UserMetadata.h"
#include "Logger.h"

UserGetProfileHandler::UserGetProfileHandler(Database* db , TokenAuthenticator* a) : EventHandlerIgnoresAuthentication(db,a)
{
    //ctor
}

bool UserGetProfileHandler::isMyRequest(HttpRequest &hmsg){
    // GET /users/'username'/profile/
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo UserGetProfile",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS &&
        hmsg.getUriCantCampos() == 3 &&
        hmsg.getUriParsedByIndex(2) == HttpRequest::PROFILE &&
        hmsg.getMethod() == HttpRequest::GET){
        log->Log("Confirma que es un Handler tipo UserGetProfile",INFO);
        return true;
    }
    return false;
}

void UserGetProfileHandler::_handle(HttpRequest& hmsg){
    std::string username = hmsg.getUsername();

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    Status s = user_metadata.DBget();

    Server_Logger* log = Server_Logger::getInstance();
    if(s.ok())
        hmsg.setResponse(s,user_metadata.getProfile());
    else
        log->Log("No se encontro el usuario",WARNING);
        hmsg.setResponse(s,"No se encontro el usuario");

}

UserGetProfileHandler::~UserGetProfileHandler()
{
    //dtor
}
