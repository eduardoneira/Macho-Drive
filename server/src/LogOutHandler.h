#ifndef LOGOUTHANDLER_H
#define LOGOUTHANDLER_H

#include "EventHandlerRemovesAuthentication.h"

class LogOutHandler : public EventHandlerRemovesAuthentication
{
    public:
        LogOutHandler(Database *db, TokenAuthenticator *a);
        virtual ~LogOutHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // LOGOUTHANDLER_H
