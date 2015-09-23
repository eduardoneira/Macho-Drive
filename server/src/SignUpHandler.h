#ifndef SIGNUPHANDLER_H
#define SIGNUPHANDLER_H

#include "EventHandler.h"

class SignUpHandler : public EventHandler{

	public:
		SignUpHandler();
		~SignUpHandler();
		void handle(struct http_message* hmsg);
	protected:
	private:

};
#endif