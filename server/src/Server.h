#ifndef SERVER_H
#define SERVER_H

#include "mongoose/mongoose.h"
#include "HandlerManager.h"

class Server
{
    public:
        Server(std::string, bool);
        virtual ~Server();

        void poll();
        void stop();

        bool createListeningConnection(const char * addr);

    protected:

    private:
        HandlerManager* handlerManager;
        struct mg_mgr* server_mgr;
        bool active;
        static Server* open_server;

        static void staticHandler(struct mg_connection *nc, int ev, void* ev_data);
        virtual void handler(struct mg_connection *nc, int ev, void* ev_data);
};

#endif // SERVER_H
