#ifndef USERDELETEHANDLER_H
#define USERDELETEHANDLER_H

#include "EventHandlerRemovesAuthentication.h"

class UserDeleteHandler : public EventHandlerRemovesAuthentication
{
    public:
        UserDeleteHandler(Database *db, TokenAuthenticator *a);
        virtual ~UserDeleteHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // USERDELETEHANDLER_H
