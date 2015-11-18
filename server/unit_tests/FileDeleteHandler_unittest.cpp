#include "gtest/gtest.h"
#include "FileDeleteHandler.h"
#include "DatabaseMockRAM.h"
#include "HttpRequestConcrete.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"

using namespace std;

TEST(FileDeleteHandlerTests, FileDeleteOK){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileDeleteHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("DELETE", "/files/gabriel/test.txt","", &header_names, &header_values);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    FileData arch(db);
    arch.setOwnerUsername("gabriel");
    arch.setFilename("test.txt");
    s = arch.DBcreate("asdasdasd", "chile");
    EXPECT_TRUE(s.ok());

    UserMetadata um(db);
    um.setUsername("gabriel");
    s = um.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um.getMy_files()->size() == 1);
    EXPECT_TRUE(um.getMy_files()->at(0) == "test.txt");

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    um.reSync();
    s = um.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um.getMy_files()->size() == 0);

    delete_http_message(hmsg);
    delete conn;
}

