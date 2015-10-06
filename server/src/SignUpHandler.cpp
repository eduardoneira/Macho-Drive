#include "SignUpHandler.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "User.h"
#include "UserMetadata.h"

SignUpHandler::SignUpHandler(Database *db, TokenAuthenticator *a) : EventHandlerGrantsAuthentication(db, a)
{

}

SignUpHandler::~SignUpHandler(){

}

void SignUpHandler::_handle(HttpRequest &hmsg){
    Status s;
    std::string usuario = hmsg.getCampo("username");
    std::string password = hmsg.getCampo("password");

    User user(db);
    user.setUsername(usuario);
    user.setPassword(password);
    s = user.DBcreate(); // db create devuelve error si ya existe la clave en la DB
    // ver status

    //mensaje de respuesta?
}
