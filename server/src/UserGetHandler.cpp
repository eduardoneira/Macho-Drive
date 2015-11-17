#include "UserGetHandler.h"
#include "UserMetadata.h"

UserGetHandler::UserGetHandler(Database *db, TokenAuthenticator *a) : EventHandlerIgnoresAuthentication(db, a)
{
    //ctor
}

UserGetHandler::~UserGetHandler()
{
    //dtor
}

void UserGetHandler::_handle(HttpRequest &hmsg){
    Status s = Status::OK();

    std::string username = hmsg.getUsername();
    if(username == "") return;

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);

    s = user_metadata.DBget();
    if(!s.ok()){
        hmsg.setResponse(s);
        return;
    }

    hmsg.setResponse(Status::OK(), user_metadata.getValueToString());
}
