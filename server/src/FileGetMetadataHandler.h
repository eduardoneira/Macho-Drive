#ifndef FILEGETMETADATAHANDLER_H
#define FILEGETMETADATAHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que busca llos metadatos de archivos.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class FileGetMetadataHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        FileGetMetadataHandler(Database *db, TokenAuthenticator *a);
        virtual ~FileGetMetadataHandler();
    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest los distintos campos que necesita para buscar el archivo
        y lo obtiene de la base de datos.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};

#endif // FILEGETMETADATAHANDLER_H
