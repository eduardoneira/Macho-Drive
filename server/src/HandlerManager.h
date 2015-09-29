#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include "mongoose/mongoose.h"
#include <vector>
#include "EventHandler.h"
#include "DatabaseRocksDB.h"

class HandlerManager
{
    public:
        HandlerManager();
        virtual ~HandlerManager();

        typedef enum HandlerType {
            HANDLER_SIGNUP      = 0,
            HANDLER_LOGIN       = 1,
            HANDLER_ADD_FILE    = 2,
            HANDLER_GET_FILE    = 3,
            HANDLER_GET_USER    = 4,
            HANDLER_DEFAULT     = 5,
            HANDLER_TYPE_SIZE
        } HandlerType;

        void handle(HttpRequest &hmsg);

    protected:

	//se llama EventHandler o Handler?
        std::vector<EventHandler*> handlers;
        Database *db; // tal vez no va aca
};

#endif // HANDLERMANAGER_H
