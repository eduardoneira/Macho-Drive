#include "HandlerManager.h"

#include <iostream>
#include <string>
#include "json/json.h"
#include "SignUpHandler.h"
#include "LogInHandler.h"


HandlerManager::HandlerManager()
{
	db = new DatabaseRocksDB();
	db->config("/tmp/test"); // tal vez se deberia poder setear, por ahora lo dejo aca
	db->open(); // se abre al principio y queda asi o se abre y cierra para procesar cada pedido?

    handlers.push_back(new SignUpHandler(db));
	handlers.push_back(new LogInHandler(db));
	/*handlers.push_back(new SendFileHandler); //tal vez muy generico
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

void HandlerManager::handle(HttpRequest &hmsg){

    //en el request viene puesto un num que indica quien maneja el eveneto (lo definimos nosotros, total hacemos el cliente tmb)
    std::string h_str = hmsg.getHandlerType();
    HandlerType h = (HandlerType)atoi(h_str.c_str()); //ver error en parseo
    if(h == 0 || h > HANDLER_TYPE_SIZE){
        //std::cout << h << std::endl;
        // handler invalido
        return;
    }
    handlers[h-1]->handle(hmsg); // puse que 0 sea error porque si mandan un msg sin campo "handlerType" el atoi lo convierte a 0. Tal vez se puede detectar en el json en vez de aca y queda mas lindo
    // devolver lo que devuelva, errores, etc
}

