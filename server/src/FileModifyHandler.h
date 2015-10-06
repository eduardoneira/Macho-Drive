#ifndef FILEMODIFYHANDLER_H
#define FILEMODIFYHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class FileModifyHandler : public EventHandlerChecksAuthentication
{
    public:
        FileModifyHandler(Database *db, TokenAuthenticator *a);
        virtual ~FileModifyHandler();

    protected:
    private:
        void _handle(HttpRequest &hmsg);
};

#endif // FILEMODIFYHANDLER_H
