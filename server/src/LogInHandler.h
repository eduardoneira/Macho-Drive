#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include "EventHandlerIgnoresAuthentication.h"

class LogInHandler : public EventHandlerIgnoresAuthentication{

	public:
		LogInHandler(Database *db, TokenAuthenticator *a);
		~LogInHandler();

	protected:
	private:
        void _handle(HttpRequest &hmsg);
};
#endif
