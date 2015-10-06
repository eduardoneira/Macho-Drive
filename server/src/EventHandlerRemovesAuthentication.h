#ifndef EVENTHANDLERREMOVESAUTHENTICATION_H
#define EVENTHANDLERREMOVESAUTHENTICATION_H

#include "EventHandler.h"
#include "TokenAuthenticator.h"


class EventHandlerRemovesAuthentication : public EventHandler
{
    public:
        EventHandlerRemovesAuthentication(Database *db, TokenAuthenticator *a);
        virtual ~EventHandlerRemovesAuthentication();

        void handle(HttpRequest &hmsg);
    protected:
        TokenAuthenticator *auth;
    private:
};

#endif // EVENTHANDLERREMOVESAUTHENTICATION_H
