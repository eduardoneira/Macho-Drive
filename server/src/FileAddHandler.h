#ifndef FILEADDHANDLER_H
#define FILEADDHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class FileAddHandler : public EventHandlerChecksAuthentication
{
    public:
        FileAddHandler(Database *db, TokenAuthenticator *a);
        virtual ~FileAddHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // FILEADDHANDLER_H
