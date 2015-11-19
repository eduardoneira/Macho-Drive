#ifndef RECYCLEBINGETHANDLER_H
#define RECYCLEBINGETHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que obtiene la papelera de reciclaje.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class RecyclebinGetHandler: public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        RecyclebinGetHandler(Database* db, TokenAuthenticator *a);
        virtual ~RecyclebinGetHandler();
    protected:

    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest el username y con este busca la papelera del usuario.
        */
         void _handle(HttpRequest &hmsg);
         bool isMyRequest(HttpRequest &hmsg);
};

#endif // RECYCLEBINGETHANDLER_H
