#ifndef FILESGETHANDLER_H
#define FILESGETHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que busca archivos.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class FilesGetHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        FilesGetHandler(Database *db, TokenAuthenticator *a);
        virtual ~FilesGetHandler();

    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest el username para buscar el archivo
        y lo obtiene de la base de datos.
        */
        void _handle(HttpRequest &hmsg);
};

#endif // FILESGETHANDLER_H
