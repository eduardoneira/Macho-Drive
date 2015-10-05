#include "SignUpHandler.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "User.h"
#include "UserMetadata.h"

SignUpHandler::SignUpHandler(Database *db) : EventHandler(db) {

}

SignUpHandler::~SignUpHandler(){

}

void SignUpHandler::handle(HttpRequest &hmsg){

    User user(db);
    user.setUsername(hmsg.getCampo("username"));
    user.setPassword(hmsg.getCampo("password"));

    UserMetadata metadata(db);
    metadata.setJoinDate("hoy"); // obviamente, cambiar esto

    std::string usr_token = hmsg.getCampo("username");
    //usr_token.append("_token"); // yo que se, por ahora. total si no permitimos que se use '_' y como los usuarios son unicos, este token tambien va a ser unico (no es el caso con archivos)
    metadata.setUsername(usr_token);

    Status s = this->db->put(user);
    // ver error en status
    s = this->db->put(metadata);
}
