#include "gtest/gtest.h"
#include "RecyclebinEmptyHandler.h"
#include "DatabaseMockRAM.h"
#include "HttpRequestConcrete.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"

using namespace std;

TEST(RecyclebinEmptyHandlerTest, EmptyNoUser){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    RecyclebinEmptyHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("DELETE", "/files/gabriel/recycle_bin/","", &header_names, &header_values);

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::ERROR);

    delete_http_message(hmsg);
    delete conn;
}

TEST(RecyclebinEmptyHandlerTest, EmptyOk){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    RecyclebinEmptyHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("DELETE", "/files/gabriel/recycle_bin/","", &header_names, &header_values);

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

    UserMetadata user_metadata(db);
    user_metadata.setUsername("gabriel");
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());

    user_metadata.DB_move_to_bin("test.txt");
    EXPECT_TRUE(user_metadata.getAllFilesBin().size() == 1);
    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    delete_http_message(hmsg);
    delete conn;


}


