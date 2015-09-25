#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include "mongoose/mongoose.h"
#include <vector>
#include "EventHandler.h"
#include "Database.h"

class HandlerManager
{
    public:
        HandlerManager();
        virtual ~HandlerManager();

        typedef enum HandlerType {
            HANDLER_DEFAULT     = 0,
            HANDLER_SIGNUP      = 1,
            HANDLER_LOGIN       = 2,
            HANDLER_SEND_FILE   = 3,
            HANDLER_GET_FILE    = 4,
            HANDLER_TYPE_SIZE
        } HandlerType;

        void handle(HttpRequest &hmsg);

    protected:

	//se llama EventHandler o Handler?
        std::vector<EventHandler*> handlers;
        Database *db; // tal vez no va aca
};

#endif // HANDLERMANAGER_H
