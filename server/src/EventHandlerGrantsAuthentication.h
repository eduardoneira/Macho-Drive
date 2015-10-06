#ifndef EVENTHANDLERGRANTSAUTHENTICATION_H
#define EVENTHANDLERGRANTSAUTHENTICATION_H

#include "EventHandler.h"
#include "TokenAuthenticator.h"

class EventHandlerGrantsAuthentication : public EventHandler
{
    public:
        EventHandlerGrantsAuthentication(Database *db, TokenAuthenticator *a);
        virtual ~EventHandlerGrantsAuthentication();

        void handle(HttpRequest &hmsg);
    protected:
        TokenAuthenticator *auth;
    private:
};

#endif // EVENTHANDLERGRANTSAUTHENTICATION_H
