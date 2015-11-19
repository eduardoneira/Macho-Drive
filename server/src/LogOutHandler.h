#ifndef LOGOUTHANDLER_H
#define LOGOUTHANDLER_H

#include "EventHandlerRemovesAuthentication.h"

//!Clase handler se encarga del logout.
/*!Hereda de EventHandlerRemovesAuthentication.
*/
class LogOutHandler : public EventHandlerRemovesAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        LogOutHandler(Database *db, TokenAuthenticator *a);
        virtual ~LogOutHandler();

    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest los distintos campos que necesita para buscar el archivo
        y lo obtiene de la base de datos.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};

#endif // LOGOUTHANDLER_H
