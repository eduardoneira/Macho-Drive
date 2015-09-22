#ifndef SERVER_H
#define SERVER_H

#include "mongoose/mongoose.h"
#include "HandlerManager.h"

class Server
{
    public:
        Server();
        virtual ~Server();

        void poll();
        void stop();

        bool createListeningConnection(const char * addr);

    protected:
        struct mg_mgr* server_mgr;
        bool active;

        static void staticHandler(struct mg_connection *nc, int ev, void* ev_data);
        virtual void handler(struct mg_connection *nc, int ev, void* ev_data);
    private:
        HandlerManager* handlerManager;
};

#endif // SERVER_H
