#ifndef USERMODIFYHANDLER_H
#define USERMODIFYHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class UserModifyHandler : public EventHandlerChecksAuthentication
{
    public:
        UserModifyHandler(Database *db, TokenAuthenticator *a);
        virtual ~UserModifyHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // USERMODIFYHANDLER_H
