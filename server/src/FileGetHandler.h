#ifndef FILEGETHANDLER_H
#define FILEGETHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class FileGetHandler : public EventHandlerChecksAuthentication
{
    public:
        FileGetHandler(Database *db, TokenAuthenticator *a);
        virtual ~FileGetHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // FILEGETHANDLER_H
