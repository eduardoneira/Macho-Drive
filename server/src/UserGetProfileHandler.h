#ifndef USERGETPROFILEHANDLER_H
#define USERGETPROFILEHANDLER_H

#include "EventHandlerIgnoresAuthentication.h"

//!Clase que devuelve el perfil de otros usuarios
/*!Ignora el token ya que deberian ser publicos los perfiles
*/

class UserGetProfileHandler : public EventHandlerIgnoresAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        UserGetProfileHandler(Database *db,TokenAuthenticator *a);
        virtual ~UserGetProfileHandler();
    protected:

    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest el nombre del usuario a buscar y
        y luego busca su perfil en la base de datos.
        */
        void _handle(HttpRequest& hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};

#endif // USERGETPROFILEHANDLER_H
