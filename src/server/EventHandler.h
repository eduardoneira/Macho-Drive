#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H


class EventHandler
{
    public:
        EventHandler();
        virtual ~EventHandler();
    protected:
    private:
        virtual void handle(struct http_msg* hmsg);
};

#endif // EVENTHANDLER_H
