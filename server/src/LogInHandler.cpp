#include "LogInHandler.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "User.h"

LogInHandler::LogInHandler(Database *db) : EventHandler(db) {

}

LogInHandler::~LogInHandler(){

}

void LogInHandler::handle(HttpRequest &hmsg){
    User user(db);
    user.setUsername(hmsg.getCampo("username"));
    //user.setPassword(hmsg.getCampo("password"));

    std::cout << "pass antes: " << user.getValueToString() << std::endl;
    Status s = this->db->get(user); // ver error en status
    std::cout << "pass despues: " << user.getValueToString() << std::endl;

    bool pass_match = (hmsg.getCampo("password").compare(user.getValueToString())) == 0;
    std::cout << "match: " << pass_match << std::endl;
}
