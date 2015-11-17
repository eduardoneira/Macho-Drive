#ifndef HTTPREQUESTMOCK_H
#define HTTPREQUESTMOCK_H

#include "HttpRequest.h"

//!Clase que maneja las httprequests.
/*!Sirve como mock para correr pruebas sin rocksdb.
*/

class HttpRequestMock : public HttpRequest{
    public:
        //!Establece los punteros nc y hmsg.
        void init(struct mg_connection* n_conn, struct http_message* n_hmsg) {};

        //!Devuelve el uri.
        std::string getUri() {};
        //!Funcion que parsea el uri en tokens.
        void getUriParsed(std::vector<std::string>& parsed) {};
        //!Funcion que devuelve un uri de acuerdo a un indice.
        UriField getUriParsedByIndex(int index) {};
        //!Funcion que devuelve un string con el uri de acuerdo a un indice.
        std::string getUriStringParsedByIndex(int index) {};
        //!Funcion que devuelve el tipo de uri.
        UriType getUriType() {};
        //!Funcion que devuelve el metodo de la request.
        MethodType getMethod() {};
        //!Funcion que busca el campo pedido.
        std::string getCampo(std::string) {};
        //!Funcion que busca el campo pedido del json.
        std::string getCampoDeArray(std::string campo, int index) {};
        //!Funcion que devuelve el filename de la httprequest.
        std::string getFilename() {};
        //!Funcion que devuelve la respuesta.
        std::string getResponse() {};
        //!Funcion que agrega valores a response.
        void addValueToBody(std::string name, std::string val) {};
        //!Funcion que devuelve el codigo de status.
        unsigned int getStatusCode() {};
        //!Funcion que establece el codigo de status.
        void setStatusCode(StatusCode statusCode) {};
        //!Funcion que devuelve el header que se corresponda con el string pasado.
        std::string getHeaderValue(std::string name) {};
        //!Funcion que devuelve el campo query.
        std::string getQueryCampo(std::string) {};

        //!Funcion que inicializa la clase.
        HttpRequestMock();
        //!Funcion que inicializa la clase.
        /*!Recibe dos string y los asigna a mock_username y a mock_token.
        */
        HttpRequestMock(std::string u, std::string t) { this->mock_username = u; this->mock_token = t; }
        virtual ~HttpRequestMock();

        void setResponse(Status s, std::string r = "") {}
        //!Funcion que devuelve el username de la httprequest.
        std::string getUsername() { return this->mock_username; }
        //!Funcion que devuelve el conn token de la httprequest.
        std::string getConnToken() { return this->mock_token; }
    private:
        //!Variable string con username.
        std::string mock_username;
        //!Variable string con token.
        std::string mock_token;
};

#endif // HTTPREQUESTMOCK_H
