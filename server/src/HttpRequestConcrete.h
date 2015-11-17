#ifndef HTTPREQUESTCONCRETE_H
#define HTTPREQUESTCONCRETE_H

#include "HttpRequest.h"

//!Clase que maneja las httprequests.

class HttpRequestConcrete : public HttpRequest
{
    public:
        //!Funcion que inicializa la clase.
        /*!Comienza con los punteros en NULL, ressponse en "" y Status en ERROR.
        */
        HttpRequestConcrete();
        virtual ~HttpRequestConcrete();

        //!Establece los punteros nc y hmsg.
        void init(struct mg_connection* n_conn, struct http_message* n_hmsg);

        //!Devuelve el uri.
        /*!Genera el uri y lo devuelve.
        */
        std::string getUri();
        //!Funcion que parsea el uri en tokens.
        void getUriParsed(std::vector<std::string>& parsed);
        //!Funcion que devuelve un uri de acuerdo a un indice.
        UriField getUriParsedByIndex(int index);
        //!Funcion que devuelve un string con el uri de acuerdo a un indice.
        std::string getUriStringParsedByIndex(int index);
        //!Funcion que devuelve el tipo de uri.
        /*!se fija si se trata de un COLLECTION_URI o un ELEMENT_URI.
        */
        UriType getUriType();
        //!Funcion que devuelve el metodo de la request.
        MethodType getMethod();
        //!Funcion que busca el campo pedido.
        std::string getCampo(std::string);
        //!Funcion que busca el campo pedido del json.
        std::string getCampoDeArray(std::string campo, int index);
        //!Funcion que devuelve el username de la httprequest.
        std::string getUsername();
        //!Funcion que devuelve el filename de la httprequest.
        std::string getFilename();
        //!Funcion que establece la respuesta.
        /*!Recibe un status e inicializa la variable response en "",
        a la cual le agrega luego el estado.
        */
        void setResponse(Status s, std::string r = "");
        //!Funcion que devuelve la variable response.
        std::string getResponse() { return response; }
        //!Funcion que agrega valores a response.
        /*!Recibe dos string con nombre y valor y los agrega a la variable response.
        */
        void addValueToBody(std::string name, std::string val);
        //!Funcion que devuelve el codigo de status.
        unsigned int getStatusCode();

        //!Funcion que devueve el conntoken.
        std::string getConnToken();
        //!Funcion que devuelve el header que se corresponda con el string pasado.
        std::string getHeaderValue(std::string name);
        //!Funcion que devuelve el campo query.
        /*!Recibe un string y a partir de este devuelve un campo especifico de la httprequest.
        */
        std::string getQueryCampo(std::string);

    protected:
    private:
        //!Variable puntero a la estructura  mg_connection.
        struct mg_connection* nc;
        //!Variable puntero a la estructura  http_message.
        struct http_message* hmsg;
        //!Variable con el json del value.
        Json::Value json_body;
        //!Variable con la respuesta.
        std::string response;
        //!Variable con el codigo del estado.
        StatusCode statusCode;
};

#endif // HTTPREQUESTCONCRETE_H
