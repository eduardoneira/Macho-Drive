#ifndef FILEGETHANDLER_H
#define FILEGETHANDLER_H

#include "EventHandler.h"

class FileGetHandler : public EventHandler
{
    public:
        FileGetHandler(Database *db);
        virtual ~FileGetHandler();
        void handle(HttpRequest &hmsg);
    protected:
    private:
};

#endif // FILEGETHANDLER_H
