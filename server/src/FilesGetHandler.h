#ifndef FILESGETHANDLER_H
#define FILESGETHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class FilesGetHandler : public EventHandlerChecksAuthentication
{
    public:
        FilesGetHandler(Database *db, TokenAuthenticator *a);
        virtual ~FilesGetHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // FILESGETHANDLER_H
