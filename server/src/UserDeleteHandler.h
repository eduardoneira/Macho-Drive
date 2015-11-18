#ifndef USERDELETEHANDLER_H
#define USERDELETEHANDLER_H

#include "EventHandlerRemovesAuthentication.h"

//!Clase handler que elimina usuario.
/*!Hereda de EventHandlerRemovesAuthentication.
*/
class UserDeleteHandler : public EventHandlerRemovesAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        UserDeleteHandler(Database *db, TokenAuthenticator *a);
        virtual ~UserDeleteHandler();

    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest el username y elimina el usuario.
        */
        void _handle(HttpRequest &hmsg);
};

#endif // USERDELETEHANDLER_H
