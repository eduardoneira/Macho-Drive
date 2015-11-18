#include "gtest/gtest.h"
#include "FileAddHandler.h"
#include "DatabaseMockRAM.h"
#include "HttpRequestConcrete.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"

using namespace std;

TEST(FileAddHandlerTests, SinFilenameFalla){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileAddHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("POST", "/files/gabriel/",
        "{\"username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"content\":\"asdasdtest\"}", &header_names, &header_values);

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

TEST(FileAddHandlerTests, SinContentNoHaceNada){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileAddHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("POST", "/files/gabriel/",
        "{\"filename\":\"test.txt\","
        "\"username\":\"gabriel\","
        "\"ubicacion\":\"chile\"}", &header_names, &header_values);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);

    UserMetadata um(db);
    um.setUsername("gabriel");
    s = um.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um.getMy_files()->size() == 0);

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileAddHandlerTests, SiNoExisteUsuarioFalla){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileAddHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("POST", "/files/gabriel/",
        "{\"filename\":\"test.txt\","
        "\"username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"content\":\"asdasdtest\"}", &header_names, &header_values);

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_FALSE(req.getStatusCode() == HttpRequest::OK);

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileAddHandlerTests, SiExisteUsuarioCreaElArchEnLaDB){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileAddHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("POST", "/files/gabriel/",
        "{\"filename\":\"test.txt\","
        "\"username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"content\":\"asdasdtest\"}", &header_names, &header_values);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);

    UserMetadata um(db);
    um.setUsername("gabriel");
    s = um.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um.getMy_files()->size() == 1);
    EXPECT_TRUE(um.getMy_files()->at(0) == "test.txt");

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileAddHandlerTests, SiPasoTagsLosAgrega){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileAddHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("POST", "/files/gabriel/",
        "{\"filename\":\"test.txt\","
        "\"username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"content\":\"asdasdtest\","
        "\"tags\":[\"test\",\"archivo\"]}", &header_names, &header_values);

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);

    FileData arch(db);
    arch.setOwnerUsername("gabriel");
    arch.setFilename("test.txt");
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getTags()->size() == 2);
    EXPECT_TRUE(arch.getTags()->at(0) == "test");
    EXPECT_TRUE(arch.getTags()->at(1) == "archivo");

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileAddHandlerTests, SiPasoPermisosDeLecturaLosAgrega){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileAddHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("POST", "/files/gabriel/",
        "{\"filename\":\"test.txt\","
        "\"username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"content\":\"asdasdtest\","
        "\"users_with_read_permission\":[\"nicolas\"]}", &header_names, &header_values);

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

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);

    FileData arch(db);
    arch.setOwnerUsername("gabriel");
    arch.setFilename("test.txt");
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithReadPermission()->size() == 1);
    EXPECT_TRUE(arch.getUsersWithReadPermission()->at(0) == "nicolas");

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileAddHandlerTests, SiPasoPermisosDeEscrituraLosAgrega){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileAddHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("POST", "/files/gabriel/",
        "{\"filename\":\"test.txt\","
        "\"username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"content\":\"asdasdtest\","
        "\"users_with_write_permission\":[\"nicolas\"]}", &header_names, &header_values);

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

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);

    FileData arch(db);
    arch.setOwnerUsername("gabriel");
    arch.setFilename("test.txt");
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithWritePermission()->size() == 1);
    EXPECT_TRUE(arch.getUsersWithWritePermission()->at(0) == "nicolas");

    delete_http_message(hmsg);
    delete conn;
}
