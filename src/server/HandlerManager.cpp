#include "HandlerManager.h"
#include <iostream>
#include <string>
#include "json.h"

HandlerManager::HandlerManager()
{
    /*handlers.push_back(new SignUpHandler);
	handlers.push_back(new LogInHandler);
	handlers.push_back(new SendFileHandler); //tal vez muy generico
	handlers.push_back(new GetFileHandler);*/
}

HandlerManager::~HandlerManager()
{
    for(std::vector<EventHandler*>::iterator it = handlers.begin(); it != handlers.end(); ++it){
		EventHandler* temp = *it;
		handlers.erase(it);
		delete temp;
	}
}

void HandlerManager::handle(struct http_message* hmsg){
    using namespace Json;

	std::string body = "";
	body.append(hmsg->body.p, hmsg->body.len); // la idea es que en el campo body del request (y del reply) estan los datos necesarios en formato json

    Reader reader;
    Value value;

    bool ok = reader.parse(/*builder,*/body.c_str(), value/*, false*/); // ver errs

    //en el request viene puesto un num que indica quien maneja el eveneto (lo definimos nosotros, total hacemos el cliente tmb)
    HandlerType h = (HandlerType)atoi(value["handlerType"].toStyledString().c_str());
    handlers[h]->handle(hmsg); //tal vez se puede masticar un poco mas la info que se le pasa
    // devolver lo que devuelva, errores, etc
}
