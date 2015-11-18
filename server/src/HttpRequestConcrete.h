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
        virtual void init(struct mg_connection* n_conn, struct http_message* n_hmsg);

        //!Devuelve el uri.
        /*!Genera el uri y lo devuelve.
        */
        virtual std::string getUri();
        //!Funcion que parsea el uri en tokens.
        virtual void getUriParsed(std::vector<std::string>& parsed);
        //!Funcion que devuelve un uri de acuerdo a un indice.
        virtual UriField getUriParsedByIndex(int index);
        //!Funcion que devuelve un string con el uri de acuerdo a un indice.
        virtual std::string getUriStringParsedByIndex(int index);
        //!Funcion que devuelve el tipo de uri.
        /*!se fija si se trata de un COLLECTION_URI o un ELEMENT_URI.
        */
        virtual UriType getUriType();
        //!Funcion que devuelve el metodo de la request.
        virtual MethodType getMethod();
        //!Funcion que busca el campo pedido.
        virtual std::string getCampo(std::string);
        //!Funcion que busca el campo pedido del json.
        virtual std::string getCampoDeArray(std::string campo, int index);
        //!Funcion que devuelve el username de la httprequest.
        virtual std::string getUsername();
        //!Funcion que devuelve el filename de la httprequest.
        virtual std::string getFilename();
        //!Funcion que establece la respuesta.
        /*!Recibe un status e inicializa la variable response en "",
        a la cual le agrega luego el estado.
        */
        virtual void setResponse(Status s, std::string r = "");
        //!Funcion que devuelve la variable response.
        virtual std::string getResponse() { return response; }
        //!Funcion que agrega valores a response.
        /*!Recibe dos string con nombre y valor y los agrega a la variable response.
        */
        virtual void addValueToBody(std::string name, std::string val);
        //!Funcion que devuelve el codigo de status.
        virtual unsigned int getStatusCode();

        //!Funcion que devueve el conntoken.
        virtual std::string getConnToken();
        //!Funcion que devuelve el header que se corresponda con el string pasado.
        virtual std::string getHeaderValue(std::string name);
        //!Funcion que devuelve el campo query.
        /*!Recibe un string y a partir de este devuelve un campo especifico de la httprequest.
        */
        virtual std::string getQueryCampo(std::string);

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

/// funciones de ultilidad para testeo

mg_str* new_mg_str(std::string s, mg_str* dest = NULL);

void delete_mg_str(struct mg_str* temp);

struct http_message* new_http_message(std::string method, std::string uri, std::string body, std::vector<std::string>* header_names = NULL, std::vector<std::string>* header_values = NULL, std::string query_string = "");

void delete_http_message(struct http_message* temp);

#endif // HTTPREQUESTCONCRETE_H
