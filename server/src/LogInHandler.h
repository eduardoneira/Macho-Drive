#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include "EventHandlerGrantsAuthentication.h"

//!Clase handler que se encarga del login.
/*!Hereda de EventHandlerGrantsAuthentication.
*/
class LogInHandler : public EventHandlerGrantsAuthentication
{

	public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
		LogInHandler(Database *db, TokenAuthenticator *a);
		~LogInHandler();

	protected:
	private:
        //!Funcion que maneja httprequests.
        /*!Primero se asegura que haya estado abierta y si lo estaba la cierra.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};
#endif
