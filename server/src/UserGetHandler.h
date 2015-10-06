#ifndef USERGETHANDLER_H
#define USERGETHANDLER_H

#include "EventHandler.h"

class UserGetHandler : public EventHandler
{
    public:
        UserGetHandler(Database *db);
        virtual ~UserGetHandler();
        void handle(HttpRequest &hmsg);
    protected:
    private:
};

#endif // USERGETHANDLER_H
