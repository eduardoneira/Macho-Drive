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
            HANDLER_SIGNUP,
            HANDLER_LOGIN,
            HANDLER_ADD_FILE,
            HANDLER_GET_FILES,
            HANDLER_GET_FILE,
            HANDLER_MODIFY_FILE,
            HANDLER_DELETE_FILE,
            HANDLER_GET_USER,
            HANDLER_DELETE_USER,
            HANDLER_DEFAULT,
            HANDLER_TYPE_SIZE
        } HandlerType;

        void handle(HttpRequest &hmsg);

    protected:

	//se llama EventHandler o Handler?
        std::vector<EventHandler*> handlers;
        Database *db; // tal vez no va aca
};

#endif // HANDLERMANAGER_H
