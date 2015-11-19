#ifndef USERGETHANDLER_H
#define USERGETHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que obtiene un usuario.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class UserGetHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        UserGetHandler(Database *db, TokenAuthenticator *a);
        virtual ~UserGetHandler();

    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest el username y busca el usuario.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};

#endif // USERGETHANDLER_H
