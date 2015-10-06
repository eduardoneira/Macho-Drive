#ifndef EVENTHANDLERCHECKSAUTHENTICATION_H
#define EVENTHANDLERCHECKSAUTHENTICATION_H

#include "EventHandler.h"
#include "TokenAuthenticator.h"

class EventHandlerChecksAuthentication : public EventHandler
{
    public:
        EventHandlerChecksAuthentication(Database *db, TokenAuthenticator *a);
        virtual ~EventHandlerChecksAuthentication();

        void handle(HttpRequest &hmsg);
    protected:
        TokenAuthenticator *auth;
    private:

};

#endif // EVENTHANDLERCHECKSAUTHENTICATION_H
