#ifndef SERVER_H
#define SERVER_H

#include "mongoose.h"

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

        static void handlerCaller(struct mg_connection *nc, int ev, void* ev_data);
        virtual void handler(struct mg_connection *nc, int ev, void* ev_data);
};

#endif // SERVER_H
