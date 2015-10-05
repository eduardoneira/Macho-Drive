#ifndef FILEDELETEHANDLER_H
#define FILEDELETEHANDLER_H

#include "EventHandler.h"

class FileDeleteHandler : public EventHandler
{
    public:
        FileDeleteHandler(Database *db);
        virtual ~FileDeleteHandler();
        void handle(HttpRequest &hmsg);
    protected:
    private:
};

#endif // FILEDELETEHANDLER_H
