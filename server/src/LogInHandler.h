#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include "EventHandler.h"

class LogInHandler : public EventHandler{

	public:
		LogInHandler();
		~LogInHandler();
		void handle(struct http_message* hmsg);
	protected:
	private:

};
#endif