#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include "mongoose.h"

class HandlerManager
{
    public:
        HandlerManager();
        virtual ~HandlerManager();
	
	typedef enum { // la idea es que en el vector se usan los eventos como indice para los distintos handlers, que cada uno sabe que hacer con el mensaje html en su propio contexto
		EVENT_SIGNUP, 
		EVENT_LOGIN, 
		EVENT_SEND_FILE,
		EVENT_GET_FILE,
		SERVER_EVENTS_SIZE
	} SERVER_EVENT;

	void handle(struct http_msg* hmsg);

    protected:

	//se llama EventHandler o Handler?
        std::vector<EventHandler*> handlers;
};

#endif // HANDLERMANAGER_H
