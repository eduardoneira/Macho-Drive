#ifndef FILEDELETEHANDLER_H
#define FILEDELETEHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que elimina archivos.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class FileDeleteHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        FileDeleteHandler(Database *db, TokenAuthenticator *a);
        virtual ~FileDeleteHandler();

    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest los distintos campos que necesita para buscar el archivo
        y lo elimina de la base de datos.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};

#endif // FILEDELETEHANDLER_H
