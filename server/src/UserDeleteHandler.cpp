#include "UserDeleteHandler.h"
#include "User.h"
#include "Logger.h"

UserDeleteHandler::UserDeleteHandler(Database *db, TokenAuthenticator *a) : EventHandlerRemovesAuthentication(db, a)
{
    //ctor
}

UserDeleteHandler::~UserDeleteHandler()
{
    //dtor
}

bool UserDeleteHandler::isMyRequest(HttpRequest &hmsg){
    // DELETE /users/'username' quiere decir borrar el usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo UserDelete",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::DELETE){
        log->Log("Confirma que es un Handler tipo UserDelete",INFO);
        return true;
    }
    return false;
}

void UserDeleteHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string username = hmsg.getUsername();
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("El campo recibido por username es : "+username,DEBUG);
    if(username == ""){
        hmsg.setResponse(Status::InvalidArgument());
        return;
    }

    User user(db);
    user.setUsername(username);
    s = user.DBerase();
    // devolver mensaje de error
    /*bool existed = auth->removeToken(username);
    if(!existed){
        //informar o loggear que no existia el usuario
    }*/
    hmsg.setResponse(s);
}
