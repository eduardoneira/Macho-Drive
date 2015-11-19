#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "mongoose/mongoose.h"
#include "Database.h"
#include "HttpRequest.h"

//!Clase madre para todos los event handlers.
/*!Contiene una funcion incializadora y luego dos funciones mas que son redefinidas por cada clase hija.
*/
class EventHandler
{
    public:
        //!Funcion que inicializa EventHandler.
        /*!Recibe como parametro un puntero a Database y lo almacena en su variable db.
        */
        EventHandler(Database *db);
        virtual ~EventHandler();

        //!Funcion que maneja httprequests.
        /*!Es redefinida en cada clase hija.
        */
        bool handle(HttpRequest &hmsg);

    protected:

        //!Variable puntero db.
        /*!Contiene un puntero a una base de datos.
        */
        Database *db;

        //!Funcion que maneja httprequests.
        /*!Es redefinida en cada clase hija.
        */
        virtual void _handle(HttpRequest &hmsg) = 0;
        virtual void _check_permission(HttpRequest &hmsg) = 0;
        virtual bool isMyRequest(HttpRequest &hmsg) = 0;

    private:

};

#endif // EVENTHANDLER_H
