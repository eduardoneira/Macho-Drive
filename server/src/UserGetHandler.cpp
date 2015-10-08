#include "UserGetHandler.h"
#include "UserMetadata.h"

UserGetHandler::UserGetHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

UserGetHandler::~UserGetHandler()
{
    //dtor
}

void UserGetHandler::_handle(HttpRequest &hmsg){
    Status s = Status::OK();

    std::string username = hmsg.getCampo("username");
    if(username == "") return;

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);

    s = user_metadata.DBget();
    if(!s.ok()){
        hmsg.setResponse(s.ToString());
        return;
    }

    hmsg.setResponse(user_metadata.getValueToString());
}
