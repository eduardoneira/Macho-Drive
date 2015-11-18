#include "gtest/gtest.h"
#include "FileGetHandler.h"
#include "DatabaseMockRAM.h"
#include "HttpRequestConcrete.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"

using namespace std;

TEST(FileGetHandlerTests, GetDeMiFileOK){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileGetHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("GET", "/files/gabriel/test.txt","", &header_names, &header_values);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    FileData arch(db);
    arch.setOwnerUsername("gabriel");
    arch.setFilename("test.txt");
    s = arch.DBcreate("asdasdcontent", "chile");
    EXPECT_TRUE(s.ok());

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileGetHandlerTests, GetDeMiFileFalla){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileGetHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("GET", "/files/gabriel/test.txt","", &header_names, &header_values);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_FALSE(req.getStatusCode() == HttpRequest::OK);

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileGetHandlerTests, GetDeFileCompartidoOK){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("nicolas");
    FileGetHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("GET", "/files/nicolas/test.txt","", &header_names, &header_values);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    User user2(db);
    user2.setUsername("nicolas");
    user2.setPassword("keklikian");
    s = user2.DBcreate();
    EXPECT_TRUE(s.ok());

    FileData arch(db);
    arch.setOwnerUsername("gabriel");
    arch.setFilename("test.txt");
    s = arch.DBcreate("asdasdcontent", "chile");
    EXPECT_TRUE(s.ok());
    vector<string> empty_str;
    vector<int> empty_int;
    vector<string> permisos;
    permisos.push_back("nicolas");
    arch.reSync();
    s = arch.DBmodify("gabriel", "", "", "", permisos, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileGetHandlerTests, GetDeFileCompartidoFalla){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("nicolas");
    FileGetHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("GET", "/files/nicolas/test.txt","", &header_names, &header_values);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    User user2(db);
    user2.setUsername("nicolas");
    user2.setPassword("keklikian");
    s = user2.DBcreate();
    EXPECT_TRUE(s.ok());

    FileData arch(db);
    arch.setOwnerUsername("gabriel");
    arch.setFilename("test.txt");
    s = arch.DBcreate("asdasdcontent", "chile");
    EXPECT_TRUE(s.ok());

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_FALSE(req.getStatusCode() == HttpRequest::OK);

    delete_http_message(hmsg);
    delete conn;
}

