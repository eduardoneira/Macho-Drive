#include "HttpRequest.h"
#include "json/json.h"

using namespace Json;

HttpRequest::HttpRequest() : nc(NULL), hmsg(NULL)
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
    return json_body[campo].toStyledString();
}

std::string HttpRequest::getUri(){
    std::string uri = "";
    uri.append(hmsg->uri.p, hmsg->uri.len);
    return uri;
}

std::string HttpRequest::getMethod(){
    std::string method = "";
    method.append(hmsg->method.p, hmsg->method.len);
    return method;
}
