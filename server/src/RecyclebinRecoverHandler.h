#ifndef RECYCLEBINRECOVERHANDLER_H
#define RECYCLEBINRECOVERHANDLER_H

#include "EventHandlerChecksAuthentication.h"

//!Clase handler que obtiene un archivo de la papelera de reciclaje.
/*!Hereda de EventHandlerChecksAuthentication.
*/
class RecyclebinRecoverHandler : public EventHandlerChecksAuthentication
{
    public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
        RecyclebinRecoverHandler(Database* db, TokenAuthenticator *a);
        virtual ~RecyclebinRecoverHandler();
    protected:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest el username y el nombre del archivo a recuperar\
        Luego con esto busca la papelera del usuario y dentro de esta el archivo.
        */
        void _handle(HttpRequest &hmsg);
    private:
};

#endif // RECYCLEBINRECOVERHANDLER_H
