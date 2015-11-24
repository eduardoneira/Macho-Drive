#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include "mongoose/mongoose.h"
#include <vector>
#include "EventHandler.h"
#include "Database.h"
#include "TokenAuthenticator.h"

//!Clase que maneja los handlers.
/*!Posee una lista con los handlers
*/
class HandlerManager
{
    public:
        //!Funcion que inicializa la clase.
        /*!Con los parametros que recibe crea un objeto DatabaseRocksDB\
        Luego crea un TokenAuthenticator y con ambos llena la lista de handlers.
        */
        HandlerManager(std::string, bool, bool);
        virtual ~HandlerManager();

        typedef enum HandlerType {
            HANDLER_SIGNUP,
            HANDLER_LOGIN,
            HANDLER_LOGOUT,
            HANDLER_ADD_FILE,
            HANDLER_GET_FILES,
            HANDLER_GET_FILE,
            HANDLER_GET_FILE_METADATA,
            HANDLER_MODIFY_FILE,
            HANDLER_DELETE_FILE,
            HANDLER_SEARCH_FILE,
            HANDLER_GET_USER,
            HANDLER_DELETE_USER,
            HANDLER_MODIFY_USER,
            HANDLER_GET_RECYCLE_BIN,
            HANDLER_EMPTY_RECYCLE_BIN,
            HANDLER_RECOVER_RECYCLE_BIN,
            HANDLER_GET_PROFILE_USER,
            HANDLER_DEFAULT,
            HANDLER_TYPE_SIZE
        } HandlerType;

        //!Funcion que interpreta las httprequests.
        /*!Dependiendo de los parametros que recibe llama al
        handler adecuado que lo tiene inicializado el vector handlers.
        */
        void handle(HttpRequest &hmsg);

    protected:

	//se llama EventHandler o Handler?
        //!Variable vector con todos los handlers.
        std::vector<EventHandler*> handlers;
        //!Variable puntero a Database.
        Database *db; // tal vez no va aca
        //!Variable puntero a TokenAuthenticator.
        TokenAuthenticator *auth; // tal vez no va aca
};

#endif // HANDLERMANAGER_H
