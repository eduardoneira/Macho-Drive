#include "SignUpHandler.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "User.h"

SignUpHandler::SignUpHandler(Database *db) : EventHandler(db) {

}

SignUpHandler::~SignUpHandler(){

}

void SignUpHandler::handle(HttpRequest &hmsg){

    User user;
    user.setUsername(hmsg.getCampo("username"));
    user.setPassword(hmsg.getCampo("password"));

    Status s = this->db->put(user); // ver error en status
}
