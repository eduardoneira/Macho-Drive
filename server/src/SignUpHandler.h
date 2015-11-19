#ifndef SIGNUPHANDLER_H
#define SIGNUPHANDLER_H

#include "EventHandlerIgnoresAuthentication.h"

//!Clase handler que se encarga del signup.
/*!Hereda de EventHandlerIgnoresAuthentication.
*/
class SignUpHandler : public EventHandlerIgnoresAuthentication{

	public:
        //!Funcion que inicializa la clase.
        /*!Recibe punteros a Database y a TokenAuthenticator y los guarda en db y auth.
        */
		SignUpHandler(Database *db, TokenAuthenticator *a);
		~SignUpHandler();

	protected:
	private:
        //!Funcion que maneja httprequests.
        /*!Extrae de la httprequest el username y la password y con ellas crea el usuario.
        */
        void _handle(HttpRequest &hmsg);
        bool isMyRequest(HttpRequest &hmsg);
};
#endif
