#include "UserGetProfileHandler.h"
#include "UserMetadata.h"

UserGetProfileHandler::UserGetProfileHandler(Database* db , TokenAuthenticator* a) : EventHandlerIgnoresAuthentication(db,a)
{
    //ctor
}

bool UserGetProfileHandler::isMyRequest(HttpRequest &hmsg){
    // GET /users/'username'/profile/
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS &&
        hmsg.getUriCantCampos() == 3 &&
        hmsg.getUriParsedByIndex(2) == HttpRequest::PROFILE &&
        hmsg.getMethod() == HttpRequest::GET){
        return true;
    }
    return false;
}

void UserGetProfileHandler::_handle(HttpRequest& hmsg){
    std::string username = hmsg.getUsername();

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    Status s = user_metadata.DBget();

    if(s.ok())
        hmsg.setResponse(s,user_metadata.getProfile());
    else
        hmsg.setResponse(s,"No se encontro el usuario");

}

UserGetProfileHandler::~UserGetProfileHandler()
{
    //dtor
}
