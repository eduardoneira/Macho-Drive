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
#include "RecyclebinEmptyHandler.h"
#include "RecyclebinGetHandler.h"
#include "RecyclebinRecoverHandler.h"
#include "UserGetProfileHandler.h"
#include "DefaultHandler.h"

HandlerManager::HandlerManager(std::string db_path, bool create_if_missing)
{
	db = new DatabaseRocksDB();
	db->config(db_path, create_if_missing);
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
	handlers.push_back(new RecyclebinGetHandler(db, auth));
	handlers.push_back(new RecyclebinEmptyHandler(db,auth));
	handlers.push_back(new RecyclebinRecoverHandler(db,auth));
	handlers.push_back(new UserGetProfileHandler(db,auth));
	handlers.push_back(new DefaultHandler(db, auth)); // este tiene q ir ultimo pq acepta cualquier uri
}

HandlerManager::~HandlerManager()
{
    for(std::vector<EventHandler*>::iterator it = handlers.begin(); it != handlers.end(); ++it){
		EventHandler* temp = *it;
		handlers.erase(it);
		delete temp;
	}
}

/// USERS
    /// COLLECTION
        // POST /users/ quiere decir sign up
        // GET /users/'username'/profile/
    /// ELEMENT
        // GET /users/'username' quiere decir pedir info del usuario
        // DELETE /users/'username' quiere decir borrar el usuario
        // PUT /users/'username' quiere decir modificar perfil de tal usuario

/// SESSIONS
    /// COLLECTION
        // POST /sessions/ quiere decir log in
    /// ELEMENT
        // DELETE /sessions/'username' quiere decir log out

/// FILES
    /// COLLECTION
        // POST /files/'username'/ quiere decir subir archivo de tal usuario
        // GET /files/'username'/ devuelve un arbol de archivos
        // GET /files/'username'/search/val1/val2 busca en los archivos del usuario segun el campo val1 (tag, nombre o extension) y valor val2(.exe, test, miarch.txt, etc)
        // GET /files/'username'/recycle_bin/ devuelve los archivos de la recycle bin del usuario
        // DELETE /files/'username'/recycle_bin/ limpia la recycle bin del usuario
        //PUT /files/'username'/recycle_bin/'filename' restaura el archivo de la recycle bin del usuario
    /// ELEMENT
        // GET /files/'username'/'filename' quiere decir pedir archivo de tal usuario
        // DELETE /files/'username'/'filename' quiere decir borrar archivo de tal usuario
        // PUT /files/'username'/'filename' quiere decir modificar archivo de tal usuario

/// OTHER
    // cualquier otro request es invalido, y se devuelve error

void HandlerManager::handle(HttpRequest &hmsg){
    for(int i = 0; i < HANDLER_TYPE_SIZE; ++i){
        if(handlers[i]->handle(hmsg)) break;
    }
}

