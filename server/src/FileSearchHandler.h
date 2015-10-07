#ifndef FILESEARCHHANDLER_H
#define FILESEARCHHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class FileSearchHandler : public EventHandlerChecksAuthentication
{
    public:
        FileSearchHandler(Database* db, TokenAuthenticator *a);
        virtual ~FileSearchHandler();
    protected:
        void _handle(HttpRequest &hmsg);
    private:
};

#endif // FILESEARCHHANDLER_H
