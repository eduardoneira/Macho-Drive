#include "UserGetHandler.h"
#include "UserMetadata.h"

UserGetHandler::UserGetHandler(Database *db) : EventHandler(db)
{
    //ctor
}

UserGetHandler::~UserGetHandler()
{
    //dtor
}

void UserGetHandler::handle(HttpRequest &hmsg){
    Status s;

    UserMetadata user_metadata(db);
    user_metadata.setUsername(hmsg.getCampo("username"));
    s = user_metadata.DBget();
    // ver status

    hmsg.setResponse(user_metadata.getValueToString());
}
