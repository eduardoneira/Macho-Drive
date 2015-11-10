#ifndef RECYCLEBINGETHANDLER_H
#define RECYCLEBINGETHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class RecyclebinGetHandler: public EventHandlerChecksAuthentication
{
    public:
        RecyclebinGetHandler(Database* db, TokenAuthenticator *a);
        virtual ~RecyclebinGetHandler();
    protected:
         void _handle(HttpRequest &hmsg);
    private:
};

#endif // RECYCLEBINGETHANDLER_H
