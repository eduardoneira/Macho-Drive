#ifndef EVENTHANDLERGRANTSAUTHENTICATION_H
#define EVENTHANDLERGRANTSAUTHENTICATION_H

#include "EventHandler.h"
#include "TokenAuthenticator.h"

//!Clase madre para algunos EventHandlers.
/*!Clase de la que heredan eventhandlers que otorgan autentificacion.
*/
class EventHandlerGrantsAuthentication : public EventHandler
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        EventHandlerGrantsAuthentication(Database *db, TokenAuthenticator *a);
        virtual ~EventHandlerGrantsAuthentication();

    protected:
        //!Variable puntero a un tokenauthenticator.
        TokenAuthenticator *auth;
    private:
        //!Funcion que maneja httprequests.
        /*!Utiliza una funcion _handle que es definida en sus clases hijas.
        */
        void _check_permission(HttpRequest &hmsg);
};

#endif // EVENTHANDLERGRANTSAUTHENTICATION_H
