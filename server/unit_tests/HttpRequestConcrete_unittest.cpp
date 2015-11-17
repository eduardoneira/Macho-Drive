#include "gtest/gtest.h"
#include "HttpRequestConcrete.h"
#include "json/json.h"

using namespace std;
using namespace rocksdb;
using namespace Json;

mg_str* new_mg_str(string s, mg_str* dest = NULL){
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

struct http_message* new_http_message(string method, string uri, string body, vector<string>* header_names = NULL, vector<string>* header_values = NULL, string query_string = ""){
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

TEST(HttpRequestConcreteTests, InitOK){
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("GET", "users/", "");

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    delete_http_message(hmsg);
}


TEST(HttpRequestConcreteTests, getUri){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    struct http_message* hmsg = new_http_message("GET", "/users/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUri().compare("/users/") == 0);
    delete_http_message(hmsg);

    hmsg = new_http_message("POST", "/files/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUri().compare("/files/") == 0);
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getUriParsed){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;
    vector<string> parsed;

    struct http_message* hmsg = new_http_message("GET", "users/gabriel", "");
    req.init(conn, hmsg);
    req.getUriParsed(parsed);
    EXPECT_TRUE(parsed[0] == "users");
    EXPECT_TRUE(parsed[1] == "gabriel");
    delete_http_message(hmsg);

    parsed.clear();
    hmsg = new_http_message("POST", "/files/", "");
    req.init(conn, hmsg);
    req.getUriParsed(parsed);
    EXPECT_TRUE(parsed[0] == "files");
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getUriParsedByIndex){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    struct http_message* hmsg = new_http_message("GET", "users/gabriel", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriParsedByIndex(0) == HttpRequest::USERS);
    EXPECT_TRUE(req.getUriParsedByIndex(1) == HttpRequest::INVALID_URI_FIELD);
    delete_http_message(hmsg);

    hmsg = new_http_message("POST", "/files/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriParsedByIndex(0) == HttpRequest::FILES);
    delete_http_message(hmsg);

    hmsg = new_http_message("POST", "/sessions/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriParsedByIndex(0) == HttpRequest::SESSIONS);
    delete_http_message(hmsg);

    hmsg = new_http_message("POST", "/search/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriParsedByIndex(0) == HttpRequest::SEARCH);
    delete_http_message(hmsg);

    hmsg = new_http_message("POST", "/recycle_bin/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriParsedByIndex(0) == HttpRequest::RECYCLE_BIN);
    delete_http_message(hmsg);

    hmsg = new_http_message("POST", "/profile/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriParsedByIndex(0) == HttpRequest::PROFILE);
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getUriStringParsedByIndex){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    struct http_message* hmsg = new_http_message("GET", "users/gabriel", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriStringParsedByIndex(0) == "users");
    EXPECT_TRUE(req.getUriStringParsedByIndex(1) == "gabriel");
    delete_http_message(hmsg);

    hmsg = new_http_message("POST", "/files/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriStringParsedByIndex(0) == "files");
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getUriType){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    struct http_message* hmsg = new_http_message("GET", "users/gabriel", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriType() == HttpRequest::ELEMENT_URI);
    delete_http_message(hmsg);

    hmsg = new_http_message("POST", "/files/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUriType() == HttpRequest::COLLECTION_URI);
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getMethod){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    struct http_message* hmsg = new_http_message("GET", "users/gabriel", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getMethod() == HttpRequest::GET);
    delete_http_message(hmsg);

    hmsg = new_http_message("POST", "/files/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getMethod() == HttpRequest::POST);
    delete_http_message(hmsg);

    hmsg = new_http_message("PUT", "/files/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getMethod() == HttpRequest::PUT);
    delete_http_message(hmsg);

    hmsg = new_http_message("DELETE", "/files/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getMethod() == HttpRequest::DELETE);
    delete_http_message(hmsg);

    hmsg = new_http_message("ASD", "/files/", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getMethod() == HttpRequest::INVALID_METHOD);
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getCampo){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    struct http_message* hmsg = new_http_message("GET", "users/gabriel", "{\"user\":\"gabriel\"}");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getCampo("user") == "gabriel");
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getCampoDeArray){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    struct http_message* hmsg = new_http_message("GET", "users/gabriel", "{\"users\":[\"gabriel\", \"edu\"]}");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getCampoDeArray("users", 0) == "gabriel");
    EXPECT_TRUE(req.getCampoDeArray("users", 1) == "edu");
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getUsername){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    struct http_message* hmsg = new_http_message("GET", "users/gabriel", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUsername() == "gabriel");
    delete_http_message(hmsg);

    hmsg = new_http_message("GET", "users/nico", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getUsername() == "nico");
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getFilename){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    struct http_message* hmsg = new_http_message("GET", "files/gabriel/test.txt", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getFilename() == "test.txt");
    delete_http_message(hmsg);

    hmsg = new_http_message("GET", "files/nico/test2.txt", "");
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getFilename() == "test2.txt");
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, setResponseOK){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;
    Reader reader;
    Value response_parsed;
    string response;

    struct http_message* hmsg = new_http_message("GET", "files/gabriel/test.txt", "");
    req.init(conn, hmsg);

    req.setResponse(Status::OK());
    response = req.getResponse();
    reader.parse(response.c_str(), response_parsed);
    EXPECT_TRUE(response_parsed["status"] == Status::OK().ToString());
    EXPECT_TRUE(req.getStatusCode() == 200);

    req.setResponse(Status::OK(), "todo bien");
    response = req.getResponse();
    reader.parse(response.c_str(), response_parsed);
    EXPECT_TRUE(response_parsed["status"] == "todo bien");
    EXPECT_TRUE(req.getStatusCode() == 200);
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, setResponseError){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;
    Reader reader;
    Value response_parsed;
    string response;

    struct http_message* hmsg = new_http_message("GET", "files/gabriel/test.txt", "");
    req.init(conn, hmsg);

    req.setResponse(Status::Aborted());
    response = req.getResponse();
    reader.parse(response.c_str(), response_parsed);
    EXPECT_TRUE(response_parsed["status"] == Status::Aborted().ToString());
    EXPECT_TRUE(req.getStatusCode() == 404);

    req.setResponse(Status::Aborted(), "todo mal");
    response = req.getResponse();
    reader.parse(response.c_str(), response_parsed);
    EXPECT_TRUE(response_parsed["status"] == "todo mal");
    EXPECT_TRUE(req.getStatusCode() == 404);
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, addValueToBody){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;
    Reader reader;
    Value response_parsed;
    string response;

    struct http_message* hmsg = new_http_message("GET", "files/gabriel/test.txt", "");
    req.init(conn, hmsg);

    req.setResponse(Status::Aborted());
    req.addValueToBody("conn_token", "1234");
    response = req.getResponse();
    reader.parse(response.c_str(), response_parsed);
    EXPECT_TRUE(response_parsed["status"] == Status::Aborted().ToString());
    EXPECT_TRUE(response_parsed["conn_token"] == "1234");
    EXPECT_TRUE(req.getStatusCode() == 404);

    req.setResponse(Status::Aborted(), "todo mal");
    req.addValueToBody("conn_token", "12345");
    response = req.getResponse();
    reader.parse(response.c_str(), response_parsed);
    EXPECT_TRUE(response_parsed["status"] == "todo mal");
    EXPECT_TRUE(response_parsed["conn_token"] == "12345");
    EXPECT_TRUE(req.getStatusCode() == 404);
    delete_http_message(hmsg);
}

TEST(HttpRequestConcreteTests, getHeaderValue){
    struct mg_connection* conn = new struct mg_connection;
    HttpRequestConcrete req;

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back("1234");
    struct http_message* hmsg = new_http_message("GET", "files/gabriel/test.txt", "", &header_names, &header_values);
    req.init(conn, hmsg);
    EXPECT_TRUE(req.getConnToken() == "1234");
    EXPECT_TRUE(req.getHeaderValue("conn_token") == "1234");
    //EXPECT_TRUE(req.getHeaderValue("invalido") == ""); ver esto
    delete_http_message(hmsg);
}

// FALTA TESTEAR GET QUERY CAMPO

