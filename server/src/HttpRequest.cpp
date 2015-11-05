#include "HttpRequest.h"
#include "json/json.h"
#include "JsonSerializer.h"

using namespace Json;
using namespace rocksdb;

HttpRequest::HttpRequest() : nc(NULL), hmsg(NULL), response(""), statusCode(StatusCode::ERROR)
{
    //ctor
}

void HttpRequest::init(struct mg_connection* n_conn, struct http_message* n_hmsg)
{
    this->nc = n_conn;
    this->hmsg = n_hmsg;

    std::string body = "";
    body.append(n_hmsg->body.p, n_hmsg->body.len);

    Reader reader;
    reader.parse(body.c_str(), json_body);
}

HttpRequest::~HttpRequest()
{

}

/*void* HttpRequest::getSender(){
}

void* HttpRequest::getReceiver(){
}*/

void HttpRequest::setResponse(Status s, std::string r){
    // agregar aca si hay mas status
    if(s.ok()){
        this->statusCode = StatusCode::OK;
    } else {
        this->statusCode = StatusCode::ERROR;
    }

    if(r == ""){
        r = s.ToString();
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

std::string HttpRequest::getHandlerType(){
    return json_body["handlerType"].toStyledString(); //handlerType deberia ser un define
}

std::string HttpRequest::getConnToken(){
    return this->getHeaderValue("conn_token");
}

std::string HttpRequest::getHeaderValue(std::string name){
    int index = -1;
    for(int i = 0; i < NS_MAX_HTTP_HEADERS; ++i){
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

    std::string tmp = "";
    tmp.append(hmsg->header_values[index].p, hmsg->header_values[index].len);
    return tmp;
}

std::string HttpRequest::getQueryCampo(std::string name){
    std::string query_string = "";
    query_string.append(hmsg->query_string.p, hmsg->query_string.len);

    std::vector<std::string> parsed_queries;

    std::stringstream input;
    input << query_string;

    std::string temp = "";
    std::string token = "";
    while(getline(input, temp, '&')){
        token.append(temp);
        if(token.compare("") == 0){
            continue;
        }
        parsed_queries.push_back(token);
        token = "";
    }

    for(std::vector<std::string>::iterator it = parsed_queries.begin(); it != parsed_queries.end(); ++it){
        input << *it;
        std::string tmp_name = "";
        getline(input, tmp_name, '=');
        std::string val = "";
        getline(input, name);
        if(tmp_name == name){
            return val;
        }
    }
    return "";
}

std::string HttpRequest::getCampo(std::string campo){
    Value temp_val;
    std::string temp_str_val;
    std::string ret = JsonSerializer::get(json_body, campo, "", temp_val, temp_str_val);
    if(ret.compare("") == 0){
        temp_str_val = "no se encontro el campo ";
        temp_str_val.append(campo);
        // hay varios como este q ni idea pq estan, pero para mi no van
        this->setResponse(Status::Aborted(temp_str_val));
    }
    return ret;
    //return json_body[campo].toStyledString();
}

std::string HttpRequest::getCampoDeArray(std::string campo, int index){
    Value def("");
    Value res = json_body[campo].get(index, def);
    std::string temp_str_value;
    if(res == def){
        temp_str_value = "no se encontro el campo ";
        temp_str_value.append(campo);
        this->setResponse(Status::Aborted(temp_str_value));
        return "";
    }

    std::string ret = JsonSerializer::get(json_body[campo], index, "", def, temp_str_value);
    if(ret.compare("") == 0){
        temp_str_value = "no se encontro el campo ";
        temp_str_value.append(campo);
        this->setResponse(Status::Aborted(temp_str_value));
    }
    return ret;
    //return res.toStyledString();
}

std::string HttpRequest::getUri(){
    std::string uri = "";
    uri.append(hmsg->uri.p, hmsg->uri.len);
    return uri;
}

void HttpRequest::getUriParsed(std::vector<std::string>& parsed){
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
        parsed.push_back(token);
        token = "";
    }
}

void HttpRequest::addValueToBody(std::string name, std::string val){
    JsonSerializer serializer;
    serializer.addValueToObject(response, name, val);
}


unsigned int HttpRequest::getStatusCode(){
    return this->statusCode;
}

void HttpRequest::setStatusCode(StatusCode statusCode){
    this->statusCode = statusCode;
}

HttpRequest::UriField HttpRequest::getUriParsedByIndex(int index){
    std::vector<std::string> parsed;
    getUriParsed(parsed);

    if(index > parsed.size()-1 || index < 0){
        return INVALID_URI_FIELD;
    }
    std::string field = parsed[index];

    if(field.compare("users") == 0){
        return USERS;
    } else if (field.compare("sessions") == 0){
        return SESSIONS;
    } else if (field.compare("username") == 0){
        return USERNAME;
    } else if (field.compare("files") == 0){
        return FILES;
    } else if (field.compare("filename") == 0){
        return FILENAME;
    } else if (field.compare("search") == 0){
        return SEARCH;
    } else {
        return INVALID_URI_FIELD;
    }
}

std::string HttpRequest::getUsername(){
    return this->getUriStringParsedByIndex(1);
}

std::string HttpRequest::getFilename(){
    return this->getUriStringParsedByIndex(2);
}

std::string HttpRequest::getUriStringParsedByIndex(int index){
    std::vector<std::string> parsed;
    getUriParsed(parsed);

    if(index > parsed.size()-1 || index < 0){
        return "";
    }
    std::string field = parsed[index];
    return field;
}

HttpRequest::UriType HttpRequest::getUriType(){
    std::string uri = this->getUri();
    if(uri[uri.size()-1] == '/'){
        return COLLECTION_URI;
    }
    return ELEMENT_URI;
}

HttpRequest::MethodType HttpRequest::getMethod(){
    std::string method = "";
    method.append(hmsg->method.p, hmsg->method.len);
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
