#ifndef USERDELETEHANDLER_H
#define USERDELETEHANDLER_H

#include "EventHandler.h"

class UserDeleteHandler : public EventHandler
{
    public:
        UserDeleteHandler(Database *db);
        virtual ~UserDeleteHandler();
        void handle(HttpRequest &hmsg);
    protected:
    private:
};

#endif // USERDELETEHANDLER_H
