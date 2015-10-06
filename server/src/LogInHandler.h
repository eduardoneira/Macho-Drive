#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include "EventHandlerGrantsAuthentication.h"

class LogInHandler : public EventHandlerGrantsAuthentication{

	public:
		LogInHandler(Database *db, TokenAuthenticator *a);
		~LogInHandler();

	protected:
	private:
        void _handle(HttpRequest &hmsg);
};
#endif
