#ifndef SIGNUPHANDLER_H
#define SIGNUPHANDLER_H

#include "EventHandler.h"

class SignUpHandler : public EventHandler{

	public:
		SignUpHandler(Database *db);
		~SignUpHandler();
		void handle(HttpRequest &hmsg);
	protected:
	private:

};
#endif
