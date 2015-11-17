#include "HttpRequestConcrete.h"
#include "json/json.h"
#include "JsonSerializer.h"

using namespace Json;
using namespace rocksdb;

HttpRequestConcrete::HttpRequestConcrete() : nc(NULL), hmsg(NULL), response(""), statusCode(StatusCode::ERROR)
{
    //ctor
}

void HttpRequestConcrete::init(struct mg_connection* n_conn, struct http_message* n_hmsg)
{
    this->nc = n_conn;
    this->hmsg = n_hmsg;

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

std::string HttpRequestConcrete::getConnToken(){
    return this->getHeaderValue("conn_token");
}

std::string HttpRequestConcrete::getHeaderValue(std::string name){
    int index = -1;
    for(int i = 0; i < MG_MAX_HTTP_HEADERS; ++i){
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
    if(ret.compare("") == 0){
        temp_str_val = "no se encontro el campo ";
        temp_str_val.append(campo);
        // hay varios como este q ni idea pq estan, pero para mi no van
        this->setResponse(Status::Aborted(temp_str_val));
    }
    return ret;
    //return json_body[campo].toStyledString();
}

std::string HttpRequestConcrete::getCampoDeArray(std::string campo, int index){
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

std::string HttpRequestConcrete::getUri(){
    std::string uri = "";
    uri.append(hmsg->uri.p, hmsg->uri.len);
    return uri;
}

void HttpRequestConcrete::getUriParsed(std::vector<std::string>& parsed){
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

void HttpRequestConcrete::addValueToBody(std::string name, std::string val){
    JsonSerializer serializer;
    serializer.addValueToObject(response, name, val);
}


unsigned int HttpRequestConcrete::getStatusCode(){
    return this->statusCode;
}

HttpRequestConcrete::UriField HttpRequestConcrete::getUriParsedByIndex(int index){
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
    }else if (field.compare("recycle_bin") == 0){
        return RECYCLE_BIN;
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
    getUriParsed(parsed);

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
