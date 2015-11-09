#ifndef HTTPREQUESTCONCRETE_H
#define HTTPREQUESTCONCRETE_H

#include "HttpRequest.h"

class HttpRequestConcrete : public HttpRequest
{
    public:

        HttpRequestConcrete();
        virtual ~HttpRequestConcrete();

        void init(struct mg_connection* n_conn, struct http_message* n_hmsg);

        std::string getUri();
        void getUriParsed(std::vector<std::string>& parsed);
        UriField getUriParsedByIndex(int index);
        std::string getUriStringParsedByIndex(int index);
        UriType getUriType();
        MethodType getMethod();
        std::string getHandlerType();
        std::string getCampo(std::string);
        std::string getCampoDeArray(std::string campo, int index);
        std::string getUsername();
        std::string getFilename();

        void setResponse(Status s, std::string r = "");
        std::string getResponse() { return response; }

        void addValueToBody(std::string name, std::string val);
        unsigned int getStatusCode();
        void setStatusCode(StatusCode statusCode);

        std::string getConnToken();
        std::string getHeaderValue(std::string name);
        std::string getQueryCampo(std::string);

    protected:
    private:

        struct mg_connection* nc;
        struct http_message* hmsg;
        Json::Value json_body;
        std::string response;
        StatusCode statusCode;
};

#endif // HTTPREQUESTCONCRETE_H
