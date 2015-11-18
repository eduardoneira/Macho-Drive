#include "UserDeleteHandler.h"
#include "User.h"

UserDeleteHandler::UserDeleteHandler(Database *db, TokenAuthenticator *a) : EventHandlerRemovesAuthentication(db, a)
{
    //ctor
}

UserDeleteHandler::~UserDeleteHandler()
{
    //dtor
}

void UserDeleteHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string username = hmsg.getUsername();
    // ver si falta el campo

    User user(db);
    user.setUsername(username);
    s = user.DBerase();
    // devolver mensaje de error
    /*bool existed = auth->removeToken(username);
    if(!existed){
        //informar o loggear que no existia el usuario
    }*/
    hmsg.setResponse(s);
}
