#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "json/json.h"
#include "mongoose/mongoose.h"
#include "rocksdb/status.h"
#include <vector>

using namespace rocksdb;

class HttpRequest
{
    public:

        typedef enum UriType{
            COLLECTION_URI,
            ELEMENT_URI
        } UriType;

        typedef enum MethodType{
            POST,
            PUT,
            GET,
            DELETE,
            INVALID_METHOD
        } MethodType;

        typedef enum UriField{
            USERS,
            FILES,
            FILENAME,
            USERNAME,
            SESSIONS,
            INVALID_URI_FIELD
        } UriField;

        HttpRequest();
        void init(struct mg_connection* n_conn, struct http_message* n_hmsg);

        virtual ~HttpRequest();

        //void* getSender();
        //void*getReceiver();
        std::string getUri();
        void getUriParsed(std::vector<std::string>& parsed);
        UriField getUriParsedByIndex(int index);
        UriType getUriType();
        MethodType getMethod();
        std::string getHandlerType();
        std::string getCampo(std::string);
        std::string getCampoDeArray(std::string campo, int index);

        void setResponse(std::string r) { response = r; }
        std::string getResponse() { return response; }

        void addValueToBody(std::string name, std::string val);
        Status getStatusCode();
        void setStatusCode(Status statusCode);

    protected:
    private:

        struct mg_connection* nc;
        struct http_message* hmsg;
        Json::Value json_body;
        std::string response;
        Status statusCode;
};

#endif // HTTPREQUEST_H
