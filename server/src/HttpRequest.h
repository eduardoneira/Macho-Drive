#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "json/json.h"
#include "mongoose/mongoose.h"

class HttpRequest
{
    public:
        HttpRequest();
        void init(struct mg_connection* n_conn, struct http_message* n_hmsg);

        virtual ~HttpRequest();

        //void* getSender();
        //void*getReceiver();
        std::string getUri();
        std::string getMethod();
        std::string getHandlerType();
        std::string getCampo(std::string);

    protected:
    private:

        struct mg_connection* nc;
        struct http_message* hmsg;
        Json::Value json_body;
};

#endif // HTTPREQUEST_H
