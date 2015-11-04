#include "HandlerManager.h"

#include "DatabaseRocksDB.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "SignUpHandler.h"
#include "LogInHandler.h"
#include "LogOutHandler.h"
#include "FileAddHandler.h"
#include "FileGetHandler.h"
#include "UserGetHandler.h"
#include "FileModifyHandler.h"
#include "FilesGetHandler.h"
#include "UserDeleteHandler.h"
#include "FileDeleteHandler.h"
#include "UserModifyHandler.h"
#include "FileSearchHandler.h"
#include "FileSearchHandler.h"

HandlerManager::HandlerManager()
{
	db = new DatabaseRocksDB();
	db->config("/tmp/test"); // tal vez se deberia poder setear, por ahora lo dejo aca
	db->open(); // se abre al principio y queda asi o se abre y cierra para procesar cada pedido?

	auth = new TokenAuthenticator();

    handlers.push_back(new SignUpHandler(db, auth));
	handlers.push_back(new LogInHandler(db, auth));
	handlers.push_back(new LogOutHandler(db, auth));
	handlers.push_back(new FileAddHandler(db, auth));
	handlers.push_back(new FilesGetHandler(db, auth));
	handlers.push_back(new FileGetHandler(db, auth));
    handlers.push_back(new FileModifyHandler(db, auth));
    handlers.push_back(new FileDeleteHandler(db, auth));
	handlers.push_back(new FileSearchHandler(db, auth));
	handlers.push_back(new UserGetHandler(db, auth));
	handlers.push_back(new UserDeleteHandler(db, auth));
	handlers.push_back(new UserModifyHandler(db, auth));
	handlers.push_back(new FileSearchHandler(db, auth));

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

    // esto tal vez se puede delegar para ordenarlo mas, ponele un UserHandler que maneja todo lo que tiene que ver con crear usuarios, hacer login, get de info de usuario, etc

/// USERS
    // se puede agregar PUT /users/'username' si queremos que se pueda actualizar la info de un usuario

    /// COLLECTION

    // POST /users/ quiere decir sign up
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS && hmsg.getUriType() ==  HttpRequest::COLLECTION_URI && hmsg.getMethod() == HttpRequest::POST){
        handlers[HANDLER_SIGNUP]->handle(hmsg);

    /// ELEMENT

    // GET /users/'username' quiere decir pedir info del usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::GET){
        handlers[HANDLER_GET_USER]->handle(hmsg);
    // DELETE /users/'username' quiere decir borrar el usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::DELETE){
        handlers[HANDLER_DELETE_USER]->handle(hmsg);
    // PUT /users/'username' quiere decir modificar perfil de tal usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::PUT){
        handlers[HANDLER_MODIFY_USER]->handle(hmsg);

/// SESSIONS

    // POST /sessions/ quiere decir log in
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::SESSIONS && hmsg.getUriType() ==  HttpRequest::COLLECTION_URI && hmsg.getMethod() == HttpRequest::POST){
        handlers[HANDLER_LOGIN]->handle(hmsg);
    // DELETE /sessions/'token' quiere decir log out
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::SESSIONS && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::DELETE){
        handlers[HANDLER_LOGOUT]->handle(hmsg);

/// FILES

    /// COLLECTION

    // POST /files/'username' quiere decir subir archivo de tal usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriType() ==  HttpRequest::COLLECTION_URI && hmsg.getMethod() == HttpRequest::POST){
        handlers[HANDLER_ADD_FILE]->handle(hmsg);
    // GET /files/'username'/ devuelve un arbol de archivos
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriParsedByIndex(2) == HttpRequest::INVALID_URI_FIELD && hmsg.getUriType() ==  HttpRequest::COLLECTION_URI && hmsg.getMethod() == HttpRequest::GET){
        handlers[HANDLER_GET_FILES]->handle(hmsg);
    // GET /files/'username'/search?metadata_to_search=val1&word_to_search=val2
    } else if (hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriParsedByIndex(2) == HttpRequest::SEARCH /*&& hmsg.getUriType() ==  HttpRequest::COLLECTION_URI*/ && hmsg.getMethod() == HttpRequest::GET ){
        handlers[HANDLER_SEARCH_FILE]->handle(hmsg);
    /// ELEMENT

    // GET /files/'username'/'filename' quiere decir pedir archivo de tal usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::GET){
        handlers[HANDLER_GET_FILE]->handle(hmsg);
    // DELETE /files/'username'/'filename' quiere decir borrar archivo de tal usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::DELETE){
        handlers[HANDLER_DELETE_FILE]->handle(hmsg);
    // PUT /files/'username'/'filename' quiere decir modificar archivo de tal usuario
    } else if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES && hmsg.getUriType() ==  HttpRequest::ELEMENT_URI && hmsg.getMethod() == HttpRequest::PUT){
        handlers[HANDLER_MODIFY_FILE]->handle(hmsg);

/// OTHER

    } else {
        // refactorizar
        // aca podria ir un handler default o handler de request invalida (agregar antes con else if los otros tipos de requests y handlers)
        hmsg.setResponse("El request recibido no es valido");
    }
}

