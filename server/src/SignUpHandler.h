#ifndef SIGNUPHANDLER_H
#define SIGNUPHANDLER_H

#include "EventHandlerIgnoresAuthentication.h"

class SignUpHandler : public EventHandlerIgnoresAuthentication{

	public:
		SignUpHandler(Database *db, TokenAuthenticator *a);
		~SignUpHandler();

	protected:
	private:
        void _handle(HttpRequest &hmsg);
};
#endif
