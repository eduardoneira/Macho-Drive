#include "LogInHandler.h"
#include <iostream>
#include <string>
#include "json/json.h"

LogInHandler::LogInHandler(){

}

LogInHandler::~LogInHandler(){

}

void LogInHandler::handle(struct http_message* hmsg){
	using namespace Json;
	Reader reader;
	Value value;	
	std::string body = "";
	body.append(hmsg->body.p, hmsg->body.len);
    if(reader.parse(/*builder,*/body.c_str(), value/*, false*/)){
    	std::cout << value["user"] << std::endl;
    	std::cout << value["password"] << std::endl;
    	//dataBase.register(value["user"], value["password"]);
	}



}