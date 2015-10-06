#include "HttpRequest.h"
#include "json/json.h"
#include "JsonSerializer.h"

using namespace Json;

HttpRequest::HttpRequest() : nc(NULL), hmsg(NULL), response("")
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

std::string HttpRequest::getHandlerType(){
    return json_body["handlerType"].toStyledString(); //handlerType deberia ser un define
}

std::string HttpRequest::getCampo(std::string campo){
    Value temp_val;
    std::string temp_str_val;
    return JsonSerializer::get(json_body, campo, "", temp_val, temp_str_val);
    //return json_body[campo].toStyledString();
}

std::string HttpRequest::getCampoDeArray(std::string campo, int index){
    Value def("");
    Value res = json_body[campo].get(index, def);
    if(res == def){
        return "";
    }
    std::string temp_str_value;
    return JsonSerializer::get(json_body[campo], index, "", def, temp_str_value);
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
    } else {
        return INVALID_URI_FIELD;
    }
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
