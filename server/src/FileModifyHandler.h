#ifndef FILEMODIFYHANDLER_H
#define FILEMODIFYHANDLER_H

#include "EventHandler.h"

class FileModifyHandler : public EventHandler
{
    public:
        FileModifyHandler(Database *db);
        virtual ~FileModifyHandler();
        void handle(HttpRequest &hmsg);
    protected:
    private:
};

#endif // FILEMODIFYHANDLER_H
