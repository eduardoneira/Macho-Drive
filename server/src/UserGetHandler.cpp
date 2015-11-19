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

bool UserGetHandler::isMyRequest(HttpRequest &hmsg){
    // GET /users/'username' quiere decir pedir info del usuario
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::GET){
        return true;
    }
    return false;
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
