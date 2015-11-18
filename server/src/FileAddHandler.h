#ifndef FILEADDHANDLER_H
#define FILEADDHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que agrega archivos.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class FileAddHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        FileAddHandler(Database *db, TokenAuthenticator *a);
        virtual ~FileAddHandler();

    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest los distintos campos que necesita para crear el nuevo archivo
        y los agrega a la base de datos.
        */
        void _handle(HttpRequest &hmsg);
};

#endif // FILEADDHANDLER_H
