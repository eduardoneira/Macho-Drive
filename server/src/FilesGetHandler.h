#ifndef FILESGETHANDLER_H
#define FILESGETHANDLER_H

#include "EventHandler.h"

class FilesGetHandler : public EventHandler
{
    public:
        FilesGetHandler(Database *db);
        virtual ~FilesGetHandler();
        void handle(HttpRequest &hmsg);
    protected:
    private:
};

#endif // FILESGETHANDLER_H
