#include "UserDeleteHandler.h"
#include "User.h"

UserDeleteHandler::UserDeleteHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

UserDeleteHandler::~UserDeleteHandler()
{
    //dtor
}

void UserDeleteHandler::_handle(HttpRequest &hmsg){
    User user(db);
    user.setUsername(hmsg.getCampo("username"));
    Status s = user.DBerase();
    // devolver mensaje de error
}
