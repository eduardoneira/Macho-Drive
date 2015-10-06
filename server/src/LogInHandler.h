#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include "EventHandler.h"

class LogInHandler : public EventHandler{

	public:
		LogInHandler(Database *db);
		~LogInHandler();
		void handle(HttpRequest &hmsg);
	protected:
	private:

};
#endif
