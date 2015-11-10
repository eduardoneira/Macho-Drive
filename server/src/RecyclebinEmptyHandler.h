#ifndef RECYCLEBINEMPTYHANDLER_H
#define RECYCLEBINEMPTYHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class RecyclebinEmptyHandler : public EventHandlerChecksAuthentication
{
    public:
        RecyclebinEmptyHandler(Database* db, TokenAuthenticator *a);
        virtual ~RecyclebinEmptyHandler();
    protected:
        void _handle(HttpRequest &hmsg);
    private:
};

#endif // RECYCLEBINEMPTYHANDLER_H
