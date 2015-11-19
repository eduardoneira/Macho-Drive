#ifndef DEFAULTHANDLER_H
#define DEFAULTHANDLER_H

#include "EventHandlerIgnoresAuthentication.h"

//!Clase handler que maneja requests invalidos.
/*!Hereda de EventHandlerIgnoresAuthentication.
*/
class DefaultHandler : public EventHandlerIgnoresAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        DefaultHandler(Database *db, TokenAuthenticator *a);
        virtual ~DefaultHandler();
    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest los distintos campos que necesita para crear el nuevo archivo
        y los agrega a la base de datos.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};

#endif // DEFAULTHANDLER_H
