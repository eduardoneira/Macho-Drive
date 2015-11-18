#ifndef USERMODIFYHANDLER_H
#define USERMODIFYHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que modifica un usuario.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class UserModifyHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        UserModifyHandler(Database *db, TokenAuthenticator *a);
        virtual ~UserModifyHandler();

    protected:
    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest el username y busca el usuario\
        Luego extrae los datos a modificar y los cambia.
        */
        void _handle(HttpRequest &hmsg);
};

#endif // USERMODIFYHANDLER_H
