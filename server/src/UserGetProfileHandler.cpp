#include "UserGetProfileHandler.h"
#include "UserMetadata.h"

UserGetProfileHandler::UserGetProfileHandler(Database* db , TokenAuthenticator* a) : EventHandlerIgnoresAuthentication(db,a)
{
    //ctor
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
