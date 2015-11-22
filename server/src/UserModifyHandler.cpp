#include "UserModifyHandler.h"
#include "UserMetadata.h"
#include "Logger.h"

UserModifyHandler::UserModifyHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

UserModifyHandler::~UserModifyHandler()
{
    //dtor
}

bool UserModifyHandler::isMyRequest(HttpRequest &hmsg){
    // PUT /users/'username' quiere decir modificar perfil de tal usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo UserModify",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::PUT){
        log->Log("Confirma que es un Handler tipo UserModify",INFO);
        return true;
    }
    return false;
}

void UserModifyHandler::_handle(HttpRequest &hmsg){
    Status s;

    Server_Logger* log = Server_Logger::getInstance();
    std::string username = hmsg.getUsername();
    log->Log("El campo recibido por username es : "+username,DEBUG);
    if(username == ""){
        hmsg.setResponse(Status::InvalidArgument());
        return;
    }
    std::string email = hmsg.getCampo("email");
    log->Log("El campo recibido por email es : "+email,DEBUG);
    std::string picture = hmsg.getCampo("picture");
    log->Log("El campo recibido por picture es : "+picture,DEBUG);
    std::string place = hmsg.getCampo("place");
    log->Log("El campo recibido por place es : "+place,DEBUG);
    std::string name = hmsg.getCampo("name");
    log->Log("El campo recibido por name es : "+name,DEBUG);

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    // tal vez cambiar a ModifyUser como con los archivos
    s = user_metadata.DBchange_email(email);
    if (s.ok()) s = user_metadata.DBchange_profile_pic(picture);
    if (s.ok()) s = user_metadata.DBchange_last_place(place);
    if (s.ok()) s = user_metadata.DBchange_name(name);
    // ver status
    hmsg.setResponse(s);
}
