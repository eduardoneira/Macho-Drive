#include "gtest/gtest.h"
#include "FilesGetHandler.h"
#include "DatabaseMockRAM.h"
#include "HttpRequestConcrete.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"
#include "json/json.h"

using namespace std;
using namespace Json;

TEST(FilesGetHandlerTests, GetFilesOK){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FilesGetHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("GET", "/files/gabriel/","", &header_names, &header_values);

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

    FileData arch2(db);
    arch2.setOwnerUsername("gabriel");
    arch2.setFilename("test2.txt");
    s = arch2.DBcreate("asdasdcontent2", "chile");
    EXPECT_TRUE(s.ok());

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    Reader reader;
    Value response_parsed;
    std::string response_str = req.getResponse();
    reader.parse(response_str.c_str(), response_parsed);
    EXPECT_TRUE(response_parsed["my_file_tokens"][0] == "test.txt");
    EXPECT_TRUE(response_parsed["my_file_tokens"][1] == "test2.txt");

    delete_http_message(hmsg);
    delete conn;
}

TEST(FilesGetHandlerTests, GetFilesFalla){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FilesGetHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("GET", "/files/gabriel/","", &header_names, &header_values);

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_FALSE(req.getStatusCode() == HttpRequest::OK);

    delete_http_message(hmsg);
    delete conn;
}

