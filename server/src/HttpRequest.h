#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "json/json.h"
#include "mongoose/mongoose.h"
#include "rocksdb/status.h"
#include <vector>
#include "Logger.h"

using namespace rocksdb;
//!Clase que se encarga de manejar httprequests.
/*!Es una interface para las clases httprequestmock y httprequestconcrete.
*/
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
            RECYCLE_BIN,
            PROFILE,
            INVALID_URI_FIELD
        } UriField;

        typedef enum StatusCode{
            OK = 200,
            BAD_REQUEST = 400,
            UNAUTHORIZED = 401,
            NOT_FOUND = 404,
            CONFLICT = 409,
            INTERNAL_ERROR = 500
        } StatusCode;

        //!Funcion que inicializa la clase.
        HttpRequest();
        virtual ~HttpRequest();
        //!Establece los punteros nc y hmsg.
        virtual void init(struct mg_connection* n_conn, struct http_message* n_hmsg) = 0;

        //!Devuelve el uri.
        virtual std::string getUri() = 0;
        //!Funcion que parsea el uri en tokens.
        virtual void getUriParsed(std::vector<std::string>* parsed) = 0;
        //!Funcion que devuelve un uri de acuerdo a un indice.
        virtual UriField getUriParsedByIndex(int index) = 0;
        //!Funcion que devuelve cuantos campos hay en el uri
        virtual int getUriCantCampos() = 0;
        //!Funcion que devuelve un string con el uri de acuerdo a un indice.
        virtual std::string getUriStringParsedByIndex(int index) = 0;
        //!Funcion que devuelve el tipo de uri.
        virtual UriType getUriType() = 0;
        //!Funcion que devuelve el metodo de la request.
        virtual MethodType getMethod() = 0;
        //!Funcion que busca el campo pedido.
        virtual std::string getCampo(std::string) = 0;
        //!Funcion que busca el campo pedido del json.
        virtual std::string getCampoDeArray(std::string campo, int index) = 0;
        //!Funcion que devuelve el username de la httprequest.
        virtual std::string getUsername() = 0;
        //!Funcion que devuelve el filename de la httprequest.
        virtual std::string getFilename() = 0;

        //!Funcion que establece la respuesta.
        virtual void setResponse(Status s, std::string r = "") = 0;
        //!Funcion que devuelve la variable response.
        virtual std::string getResponse() = 0;

        //!Funcion que agrega valores a response.
        virtual void addValueToBody(std::string name, std::string val) = 0;
        //!Funcion que devuelve el codigo de status.
        virtual unsigned int getStatusCode() = 0;

        //!Funcion que devueve el conntoken.
        virtual std::string getConnToken() = 0;
        //!Funcion que devuelve el header que se corresponda con el string pasado.
        virtual std::string getHeaderValue(std::string name) = 0;
        //!Funcion que devuelve el campo query.
        virtual std::string getQueryCampo(std::string) = 0;

    protected:
    private:
};

#endif // HTTPREQUEST_H
