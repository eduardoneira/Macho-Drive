#ifndef HTTPREQUESTMOCK_H
#define HTTPREQUESTMOCK_H

#include "HttpRequest.h"

class HttpRequestMock : public HttpRequest{
    public:
        void init(struct mg_connection* n_conn, struct http_message* n_hmsg) {};

        std::string getUri() {};
        void getUriParsed(std::vector<std::string>& parsed) {};
        UriField getUriParsedByIndex(int index) {};
        std::string getUriStringParsedByIndex(int index) {};
        UriType getUriType() {};
        MethodType getMethod() {};
        std::string getHandlerType() {};
        std::string getCampo(std::string) {};
        std::string getCampoDeArray(std::string campo, int index) {};
        std::string getFilename() {};

        std::string getResponse() {};

        void addValueToBody(std::string name, std::string val) {};
        unsigned int getStatusCode() {};
        void setStatusCode(StatusCode statusCode) {};

        std::string getHeaderValue(std::string name) {};
        std::string getQueryCampo(std::string) {};

        HttpRequestMock();
        HttpRequestMock(std::string u, std::string t) { this->mock_username = u; this->mock_token = t; }
        virtual ~HttpRequestMock();
        void setResponse(Status s, std::string r = "") {}
        std::string getUsername() { return this->mock_username; }
        std::string getConnToken() { return this->mock_token; }
    private:
        std::string mock_username;
        std::string mock_token;
};

#endif // HTTPREQUESTMOCK_H
