#include "gtest/gtest.h"
#include "SignUpHandler.h"
#include "DatabaseMockRAM.h"
#include "HttpRequestConcrete.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"

using namespace std;

TEST(SignUpHandlerTests, SignUpFlujo){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    SignUpHandler handler(db, tk);

    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("POST", "/users/","{\"username\":\"gabriel\",\"password\":\"gayoso\"}");

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBget();
    EXPECT_TRUE(s.ok());

    delete_http_message(hmsg);
    delete conn;
}

