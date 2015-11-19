#ifndef FILESEARCHHANDLER_H
#define FILESEARCHHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que busca archivos.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class FileSearchHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        FileSearchHandler(Database* db, TokenAuthenticator *a);
        virtual ~FileSearchHandler();
    protected:

    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest los distintos campos que necesita para buscar el archivo
        y luego realiza la busqueda en la base de datos.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};

#endif // FILESEARCHHANDLER_H
