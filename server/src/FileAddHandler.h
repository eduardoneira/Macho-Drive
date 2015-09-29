#ifndef FILEADDHANDLER_H
#define FILEADDHANDLER_H

#include "EventHandler.h"

class FileAddHandler : public EventHandler
{
    public:
        FileAddHandler(Database *db);
        virtual ~FileAddHandler();
        void handle(HttpRequest &hmsg);
    protected:
    private:
};

#endif // FILEADDHANDLER_H
