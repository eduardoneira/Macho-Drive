#ifndef USERDELETEHANDLER_H
#define USERDELETEHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class UserDeleteHandler : public EventHandlerChecksAuthentication
{
    public:
        UserDeleteHandler(Database *db, TokenAuthenticator *a);
        virtual ~UserDeleteHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // USERDELETEHANDLER_H
