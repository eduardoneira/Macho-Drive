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
            SEARCH,
            INVALID_URI_FIELD
        } UriField;

        typedef enum StatusCode{
            OK = 200,
            ERROR = 404
        } StatusCode;

        HttpRequest();
        virtual ~HttpRequest();

        virtual void init(struct mg_connection* n_conn, struct http_message* n_hmsg) = 0;

        virtual std::string getUri() = 0;
        virtual void getUriParsed(std::vector<std::string>& parsed) = 0;
        virtual UriField getUriParsedByIndex(int index) = 0;
        virtual std::string getUriStringParsedByIndex(int index) = 0;
        virtual UriType getUriType() = 0;
        virtual MethodType getMethod() = 0;
        virtual std::string getHandlerType() = 0;
        virtual std::string getCampo(std::string) = 0;
        virtual std::string getCampoDeArray(std::string campo, int index) = 0;
        virtual std::string getUsername() = 0;
        virtual std::string getFilename() = 0;

        virtual void setResponse(Status s, std::string r = "") = 0;
        virtual std::string getResponse() = 0;

        virtual void addValueToBody(std::string name, std::string val) = 0;
        virtual unsigned int getStatusCode() = 0;
        virtual void setStatusCode(StatusCode statusCode) = 0;

        virtual std::string getConnToken() = 0;
        virtual std::string getHeaderValue(std::string name) = 0;
        virtual std::string getQueryCampo(std::string) = 0;

    protected:
    private:
};

#endif // HTTPREQUEST_H
