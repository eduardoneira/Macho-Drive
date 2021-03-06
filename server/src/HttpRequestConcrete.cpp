#include "HttpRequestConcrete.h"
#include "json/json.h"
#include "JsonSerializer.h"
#include <iostream>
#include "Logger.h"

using namespace Json;
using namespace rocksdb;

/// funciones de ultilidad para testeo

mg_str* new_mg_str(std::string s, mg_str* dest){
    if(dest == NULL)
        dest = new struct mg_str;
    char* temp_str = (char*)malloc(s.size());
    for(int i = 0, size = s.size(); i < size; ++i){
        temp_str[i] = s[i];
    }
    dest->p = temp_str;
    dest->len = s.size();

    return dest;
}

void delete_mg_str(struct mg_str* temp){
    if(temp != NULL){
        if(temp->p != NULL)
            free((void*)temp->p);
        delete temp;
    }
}

struct http_message* new_http_message(std::string method, std::string uri, std::string body, std::vector<std::string>* header_names, std::vector<std::string>* header_values, std::string query_string){
    struct http_message* temp = new struct http_message;

    new_mg_str(method, &(temp->method));
    new_mg_str(uri, &temp->uri);
    new_mg_str("HTTP/1.1", &temp->proto);
    new_mg_str(query_string, &temp->query_string);
    new_mg_str(body, &temp->body);

    if(header_names != NULL){
        for(int i = 0, size = header_names->size(); i < size; ++i){
            new_mg_str(header_names->at(i), temp->header_names+i);
            new_mg_str(header_values->at(i), temp->header_values+i);
        }
    }

    return temp;
}

void delete_http_message(struct http_message* temp){
    if(temp != NULL){
        delete temp;
    }
}

HttpRequestConcrete::HttpRequestConcrete() : nc(NULL), hmsg(NULL), response(""), statusCode(StatusCode::INTERNAL_ERROR)
{
    //ctor
}

void HttpRequestConcrete::init(struct mg_connection* n_conn, struct http_message* n_hmsg)
{
    this->nc = n_conn;
    this->hmsg = n_hmsg;

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Inicializando http request concrete",INFO);

    std::string body = "";
    body.append(n_hmsg->body.p, n_hmsg->body.len);

    Reader reader;
    reader.parse(body.c_str(), json_body);
}

HttpRequestConcrete::~HttpRequestConcrete()
{

}

/*void* HttpRequestConcrete::getSender(){
}

void* HttpRequestConcrete::getReceiver(){
}*/

void HttpRequestConcrete::setResponse(Status s, std::string r){
    // agregar aca si hay mas status
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("HttpRequest : seteando respuesta",INFO);

    if(s.ok()){                                 // todo ok
        log->Log("Respuesta OK",INFO);
        this->statusCode = StatusCode::OK;
    } else if(s.IsNotFound()){                  // no se encontro el registro
        log->Log("Respuesta ERROR: NOT FOUND",WARNING);
        this->statusCode = StatusCode::NOT_FOUND;
    } else if(s.IsInvalidArgument()){           // error de parametros en el request
        log->Log("Respuesta ERROR: BAD REQUEST",WARNING);
        this->statusCode = StatusCode::BAD_REQUEST;
    } else if(s.IsIOError()){                   // error de autorizacion
        log->Log("Respuesta ERROR: UNAUTHORIZED",WARNING);
        this->statusCode = StatusCode::UNAUTHORIZED;
    } else if(s.IsAborted()){                   // error interno
        log->Log("Respuesta ERROR: INTERNAL ERROR",WARNING);
        this->statusCode = StatusCode::INTERNAL_ERROR;
    } else if(s.IsCorruption()){                // error de logica (el request esta bien, pero no se puede cumplir por ej crear un usuario que ya existe)
        log->Log("Respuesta ERROR: LOGIC CONFLICT",WARNING);
        this->statusCode = StatusCode::CONFLICT;
    }

    if(r == ""){
        r = s.ToString();
        for(int i = 0; i < r.length(); i++){
            if(r[i] == ':'){
                r.erase(0, i+2);
                break;
            }
        }
    }

    if(r.size() > 0 && r[0] != '{'){
        this->response = "";
        JsonSerializer serializer;
        serializer.addValueToObjectList(response, "status", r);
        serializer.turnObjectListToObject(response);
    } else {
        this->response = r;
    }
}

std::string HttpRequestConcrete::getConnToken(){
    return this->getHeaderValue("conn_token");
}

std::string HttpRequestConcrete::getHeaderValue(std::string name){
    int index = -1;
    for(int i = 0; i < MG_MAX_HTTP_HEADERS; ++i){ // si busco un header q no existe se rompe
        std::string tmp = "";
        tmp.append(hmsg->header_names[i].p, hmsg->header_names[i].len);
        if(tmp == name){
            index = i;
            break;
        }
    }
    if(index == -1){
        return "";
    }
    Server_Logger* log = Server_Logger::getInstance();

    std::string tmp = "";
    tmp.append(hmsg->header_values[index].p, hmsg->header_values[index].len);

    log->Log("Http request : recuperando header = "+tmp,INFO);

    return tmp;
}

std::string HttpRequestConcrete::getQueryCampo(std::string name){
    std::string uri;
    uri.append(hmsg->uri.p,hmsg->uri.len);

    std::vector<std::string> parsed_queries;

    std::stringstream input;
    input << uri;

    std::string temp = "";
    std::string token = "";
    //ARREGLAR CASO VACIO
    while(getline(input, temp, '/')){
        token.append(temp);
        //std::cout << "lo q hay : " << token << std::endl;
        if(token.compare("") == 0){
            continue;
        }
        parsed_queries.push_back(token);
        token = "";

    }

    Server_Logger* log = Server_Logger::getInstance();
     log->Log("Http request : recuperando campos para queries ",INFO);



    if (parsed_queries.size() < 5){
        return "";
    }

    if( name == "word_to_search"){
        return parsed_queries[4];
    }
    if (name == "metadata_to_search"){
        return parsed_queries[3];
    }
    return "";
}

std::string HttpRequestConcrete::getCampo(std::string campo){
    Value temp_val;
    std::string temp_str_val;
    std::string ret = JsonSerializer::get(json_body, campo, "", temp_val, temp_str_val);

    Server_Logger* log = Server_Logger::getInstance();


    return ret;
    //return json_body[campo].toStyledString();
}

std::string HttpRequestConcrete::getCampoDeArray(std::string campo, int index){
    Value def("");
    Value res = json_body[campo].get(index, def);
    std::string temp_str_value;
    if(res == def){
        return "";
    }

    std::string ret = JsonSerializer::get(json_body[campo], index, "", def, temp_str_value);
    return ret;
    //return res.toStyledString();
}

std::string HttpRequestConcrete::getUri(){
    std::string uri = "";
    uri.append(hmsg->uri.p, hmsg->uri.len);
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Http request : recuperando uri = "+uri,INFO);

    return uri;
}

void HttpRequestConcrete::getUriParsed(std::vector<std::string>* parsed){
    std::stringstream input;
    std::string uri = this->getUri();
    input << uri;

    std::string temp = "";
    std::string token = "";
    while(getline(input, temp, '/')){
        token.append(temp);
        if(token.compare("") == 0){
            continue;
        }
        parsed->push_back(token);
        token = "";
    }
}

void HttpRequestConcrete::addValueToBody(std::string name, std::string val){
    JsonSerializer serializer;
    serializer.addValueToObject(response, name, val);
}


unsigned int HttpRequestConcrete::getStatusCode(){
    return this->statusCode;
}

int HttpRequestConcrete::getUriCantCampos(){
    std::vector<std::string> parsed;
    getUriParsed(&parsed);
    return parsed.size();
}

HttpRequestConcrete::UriField HttpRequestConcrete::getUriParsedByIndex(int index){
    std::vector<std::string> parsed;
    getUriParsed(&parsed);
    if((index >= parsed.size()) || (index < 0)){
        return INVALID_URI_FIELD;
    }
    std::string field = parsed[index];

    Server_Logger* log = Server_Logger::getInstance();

    log->Log("Http request : recuperando uri por indice = "+field,INFO);

    if(field.compare("users") == 0){
        return USERS;
    } else if (field.compare("sessions") == 0){
        return SESSIONS;
    } else if (field.compare("files") == 0){
        return FILES;
    } else if (field.compare("search") == 0){
        return SEARCH;
    }else if (field.compare("recycle_bin") == 0){
        return RECYCLE_BIN;
    }else if (field.compare("profile") == 0){
        return PROFILE;
    } else if (field.compare("metadata") == 0){
        return METADATA;
    } else {
        return INVALID_URI_FIELD;
    }
}

std::string HttpRequestConcrete::getUsername(){
    return this->getUriStringParsedByIndex(1);
}

std::string HttpRequestConcrete::getFilename(){
    return this->getUriStringParsedByIndex(2);
}

std::string HttpRequestConcrete::getUriStringParsedByIndex(int index){
    std::vector<std::string> parsed;
    getUriParsed(&parsed);

    if(index > parsed.size()-1 || index < 0){
        return "";
    }
    std::string field = parsed[index];
    return field;
}

HttpRequestConcrete::UriType HttpRequestConcrete::getUriType(){
    std::string uri = this->getUri();
    if(uri[uri.size()-1] == '/'){
        return COLLECTION_URI;
    }
    return ELEMENT_URI;
}

HttpRequestConcrete::MethodType HttpRequestConcrete::getMethod(){
    std::string method = "";
    method.append(hmsg->method.p, hmsg->method.len);
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Http request : recuperando uri por indice = "+method,INFO);

    if(method.compare("POST") == 0){
        return POST;
    } else if (method.compare("GET") == 0){
        return GET;
    } else if (method.compare("PUT") == 0){
        return PUT;
    } else if (method.compare("DELETE") == 0){
        return DELETE;
    } else {
        return INVALID_METHOD;
    }
}
