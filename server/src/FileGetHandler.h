#ifndef FILEGETHANDLER_H
#define FILEGETHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que busca archivos.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class FileGetHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        FileGetHandler(Database *db, TokenAuthenticator *a);
        virtual ~FileGetHandler();

    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest los distintos campos que necesita para buscar el archivo
        y lo obtiene de la base de datos.
        */
        void _handle(HttpRequest &hmsg);
};

#endif // FILEGETHANDLER_H
