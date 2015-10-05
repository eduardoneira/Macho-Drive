#include "HandlerManager.h"

#include <iostream>
#include <string>
#include "json/json.h"
#include "SignUpHandler.h"
#include "LogInHandler.h"
#include "FileAddHandler.h"
#include "FileGetHandler.h"
#include "UserGetHandler.h"
#include "FileModifyHandler.h"


HandlerManager::HandlerManager()
{
	db = new DatabaseRocksDB();
	db->config("/tmp/test"); // tal vez se deberia poder setear, por ahora lo dejo aca
	db->open(); // se abre al principio y queda asi o se abre y cierra para procesar cada pedido?

    handlers.push_back(new SignUpHandler(db));
	handlers.push_back(new LogInHandler(db));
	handlers.push_back(new FileAddHandler(db));
	handlers.push_back(new FileGetHandler(db));
    handlers.push_back(new FileModifyHandler(db));
	handlers.push_back(new UserGetHandler(db));
}

HandlerManager::~HandlerManager()
{
    for(std::vector<EventHandler*>::iterator it = handlers.begin(); it != handlers.end(); ++it){
		EventHandler* temp = *it;
		handlers.erase(it);
		delete temp;
	}
}

//viejo
/*void HandlerManager::handle(HttpRequest &hmsg){

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
}*/

void HandlerManager::handle(HttpRequest &hmsg){

    // esto tal vez se puede delegar para ordenarlo mas, ponele un UserHandler que maneja todo lo que tiene que ver con crear usuarios, hacer login, get de info de usuario, etc

/// USERS
    // se puede agregar PUT /users/'username' si queremos que se pueda actualizar la info de un usuario

    // POST /users/ quiere decir sign up
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS && hmsg.getUriType() ==  HttpRequest::COLLECTION_URI && hmsg.getMethod() == HttpRequest::POST){
        handlers[HANDLER_SIGNUP]->handle(hmsg);
    // GET /users/'username' quiere decir pedir info del usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::GET){
        handlers[HANDLER_GET_USER]->handle(hmsg);
    // DELETE /users/'username' quiere decir borrar el usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::DELETE){
        //handlers[];

/// SESSIONS

    // POST /sessions/ quiere decir log in
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::SESSIONS && hmsg.getUriType() ==  HttpRequest::COLLECTION_URI && hmsg.getMethod() == HttpRequest::POST){
        handlers[HANDLER_LOGIN]->handle(hmsg);
    // DELETE /sessions/'token' quiere decir log out
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::SESSIONS && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::DELETE){
        //handlers[];

/// FILES

    /// COLLECTION

    // POST /files/'username' quiere decir subir archivo de tal usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriType() ==  HttpRequest::COLLECTION_URI && hmsg.getMethod() == HttpRequest::POST){
        handlers[HANDLER_ADD_FILE]->handle(hmsg);

    /// ELEMENT

    // GET /files/'username'/'filename' quiere decir pedir archivo de tal usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::GET){
        handlers[HANDLER_GET_FILE]->handle(hmsg);
    // DELETE /files/'username'/'filename' quiere decir borrar archivo de tal usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::DELETE){
        //handlers[HANDLER_DELETE_FILE]->handle(hmsg);
    // PUT /files/'username'/'filename' quiere decir modificar archivo de tal usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::PUT){
        handlers[HANDLER_MODIFY_FILE]->handle(hmsg);

/// OTHER

    } else {
        // aca podria ir un handler default o handler de request invalida (agregar antes con else if los otros tipos de requests y handlers)
    }
}

