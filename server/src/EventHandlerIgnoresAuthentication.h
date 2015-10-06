#ifndef EVENTHANDLERIGNORESAUTHENTICATION_H
#define EVENTHANDLERIGNORESAUTHENTICATION_H

#include "EventHandler.h"
#include "TokenAuthenticator.h"

class EventHandlerIgnoresAuthentication : public EventHandler
{
    public:
        EventHandlerIgnoresAuthentication(Database *db, TokenAuthenticator *a);
        virtual ~EventHandlerIgnoresAuthentication();

        void handle(HttpRequest &hmsg);
    protected:
        TokenAuthenticator *auth;
    private:
};

#endif // EVENTHANDLERIGNORESAUTHENTICATION_H
