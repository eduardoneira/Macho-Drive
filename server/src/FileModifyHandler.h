#ifndef FILEMODIFYHANDLER_H
#define FILEMODIFYHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que modifica archivos.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class FileModifyHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        FileModifyHandler(Database *db, TokenAuthenticator *a);
        virtual ~FileModifyHandler();

    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest los distintos campos que necesita para buscar el archivo
        y luego los campos que debe modificar en la base de datos.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};

#endif // FILEMODIFYHANDLER_H
