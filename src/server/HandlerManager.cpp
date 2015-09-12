#include "HandlerManager.h"
#include <iostream>

HandlerManager::HandlerManager()
{
    handlers.push_back(new SignUpHandler);
	handlers.push_back(new LogInHandler);
	handlers.push_back(new SendFileHandler);
	handlers.push_back(new GetFileHandler);
}

HandlerManager::~HandlerManager()
{
    for(std::vector<EventHandler*> it = handlers.begin(); it != handlers.end(); ++it){
		EventHandler* temp = *it;
		handlers.pop(it);
		delete temp;
	}
}

void HandlerManager::handle(struct http_msg* hmsg){
	//ver a quien llamar, errores, etc
}
