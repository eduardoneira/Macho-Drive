#include "UserModifyHandler.h"
#include "UserMetadata.h"

UserModifyHandler::UserModifyHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

UserModifyHandler::~UserModifyHandler()
{
    //dtor
}

void UserModifyHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string username = hmsg.getCampo("username");
    std::string email = hmsg.getCampo("email");

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    s = user_metadata.DBchange_email(email);
    // ver status
}
