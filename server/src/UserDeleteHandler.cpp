#include "UserDeleteHandler.h"
#include "User.h"

UserDeleteHandler::UserDeleteHandler(Database *db) : EventHandler(db)
{
    //ctor
}

UserDeleteHandler::~UserDeleteHandler()
{
    //dtor
}

void UserDeleteHandler::handle(HttpRequest &hmsg){
    User user(db);
    user.setUsername(hmsg.getCampo("username"));
    Status s = user.DBerase();
    // devolver mensaje de error
}
