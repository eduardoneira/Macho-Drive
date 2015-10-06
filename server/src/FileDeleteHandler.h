#ifndef FILEDELETEHANDLER_H
#define FILEDELETEHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class FileDeleteHandler : public EventHandlerChecksAuthentication
{
    public:
        FileDeleteHandler(Database *db, TokenAuthenticator *a);
        virtual ~FileDeleteHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // FILEDELETEHANDLER_H
