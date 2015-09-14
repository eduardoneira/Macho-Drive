#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "mongoose.h"

class EventHandler
{
    public:
        EventHandler();
        virtual ~EventHandler();

        virtual void handle(struct http_message* hmsg) = 0;
    protected:
    private:

};

#endif // EVENTHANDLER_H
