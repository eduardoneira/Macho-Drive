#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "mongoose/mongoose.h"
#include "HttpRequest.h"
#include "Database.h"

class EventHandler
{
    public:
        EventHandler(Database *db);
        virtual ~EventHandler();

        virtual void handle(HttpRequest &hmsg) = 0;
    protected:
        Database *db;

        virtual void _handle(HttpRequest &hmsg) = 0;

    private:

};

#endif // EVENTHANDLER_H
