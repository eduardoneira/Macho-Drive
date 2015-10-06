#ifndef USERGETHANDLER_H
#define USERGETHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class UserGetHandler : public EventHandlerChecksAuthentication
{
    public:
        UserGetHandler(Database *db, TokenAuthenticator *a);
        virtual ~UserGetHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // USERGETHANDLER_H
