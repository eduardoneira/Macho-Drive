#ifndef SIGNUPHANDLER_H
#define SIGNUPHANDLER_H

#include "EventHandlerGrantsAuthentication.h"

class SignUpHandler : public EventHandlerGrantsAuthentication{

	public:
		SignUpHandler(Database *db, TokenAuthenticator *a);
		~SignUpHandler();

	protected:
	private:
        void _handle(HttpRequest &hmsg);
};
#endif
