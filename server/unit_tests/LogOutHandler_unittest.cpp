#include "gtest/gtest.h"
#include "LogOutHandler.h"
#include "DatabaseMockRAM.h"
#include "HttpRequestConcrete.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"
#include "json/json.h"
#include "JsonSerializer.h"

using namespace std;
using namespace Json;

TEST(LogOutHandlerTests, LogOutOK){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    LogOutHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("DELETE", "/sessions/gabriel", "", &header_names, &header_values);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);
    EXPECT_FALSE(tk->isValidToken("gabriel", token));

    delete_http_message(hmsg);
    delete conn;
}

TEST(LogOutHandlerTests, LogOutFalla){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    LogOutHandler handler(db, tk);

    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("DELETE", "/sessions/gabriel", "");

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_FALSE(req.getStatusCode() == HttpRequest::OK);

    delete_http_message(hmsg);
    delete conn;
}
