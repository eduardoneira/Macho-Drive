#ifndef CLIENT_H
#define CLIENT_H

#include "mongoose.h"
#include <string>

class Client
{
    public:
        Client();
        virtual ~Client();

        void setRequest(std::string &req) { request = req; }
        bool connect(std::string addr);

        void poll();
        void stop();

        bool createListeningConnection(const char * addr);
    protected:
    private:

        void handler(struct mg_connection *nc, int ev, void* ev_data);
        std::string request;

        struct mg_mgr* server_mgr;
        bool active;

        static void handlerCaller(struct mg_connection *nc, int ev, void* ev_data);
};

#endif // CLIENT_H
