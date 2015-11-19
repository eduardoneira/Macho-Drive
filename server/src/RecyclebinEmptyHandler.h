#ifndef RECYCLEBINEMPTYHANDLER_H
#define RECYCLEBINEMPTYHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que vacia la papelera de reciclaje.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class RecyclebinEmptyHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        RecyclebinEmptyHandler(Database* db, TokenAuthenticator *a);
        virtual ~RecyclebinEmptyHandler();
    protected:

    private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest el username y con este busca la papelera del usuario\
        Una vez obtiene esto elimina todos los archivos dentro de esta.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};

#endif // RECYCLEBINEMPTYHANDLER_H
