#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include "mongoose.h"

class HandlerManager
{
    public:
        HandlerManager();
        virtual ~HandlerManager();

        /*typedef enum ServerEvent { // la idea es que en el vector se usan los eventos como indice para los distintos handlers, que cada uno sabe que hacer con el mensaje html en su propio contexto
            EVENT_SIGNUP        = 0,
            EVENT_LOGIN         = 1,
            EVENT_SEND_FILE     = 2,
            EVENT_GET_FILE      = 3,
            SERVER_EVENTS_SIZE  = 4
        } ServerEvent;*/

        typedef enum HandlerType {
            HANDLER_SIGNUP      = 0,
            HANDLER_LOGIN       = 1,
            HANDLER_SEND_FILE   = 2,
            HANDLER_GET_FILE    = 3,
            HANDLER_TYPE_SIZE
        } HandlerType;

        void handle(struct http_msg* hmsg);

    protected:

	//se llama EventHandler o Handler?
        std::vector<EventHandler*> handlers;
};

#endif // HANDLERMANAGER_H
