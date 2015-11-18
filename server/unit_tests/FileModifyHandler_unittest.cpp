#include "gtest/gtest.h"
#include "FileModifyHandler.h"
#include "DatabaseMockRAM.h"
#include "HttpRequestConcrete.h"
#include "User.h"
#include "UserMetadata.h"
#include "FileData.h"

using namespace std;

TEST(FileModifyHandlerTests, ModifyFilename){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileModifyHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"filename_change\":\"teste.txt\"}",
        &header_names, &header_values);

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
    EXPECT_TRUE(um.getMy_files()->size() == 1);
    EXPECT_TRUE(um.getMy_files()->at(0) == "teste.txt");

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileModifyHandlerTests, ModifyContent){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileModifyHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"content_change\":\"newcontent\"}",
        &header_names, &header_values);

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

    UserMetadata um(db);
    um.setUsername("gabriel");
    s = um.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um.getMy_files()->size() == 1);
    EXPECT_TRUE(um.getMy_files()->at(0) == "test.txt");

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getContent() == "asdasdcontent");

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getContent() == "newcontent");

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileModifyHandlerTests, DeleteVersion){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileModifyHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"content_change\":\"newcontent\"}",
        &header_names, &header_values);

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

    UserMetadata um(db);
    um.setUsername("gabriel");
    s = um.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um.getMy_files()->size() == 1);
    EXPECT_TRUE(um.getMy_files()->at(0) == "test.txt");

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getVersions()->size() == 1);
    EXPECT_TRUE(arch.getVersions()->at(0) == "asdasdcontent");

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getVersions()->size() == 2);
    EXPECT_TRUE(arch.getVersions()->at(0) == "asdasdcontent");
    EXPECT_TRUE(arch.getVersions()->at(1) == "newcontent");

    hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"delete_versions\":[0]}",
        &header_names, &header_values);

    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getVersions()->size() == 1);
    EXPECT_TRUE(arch.getVersions()->at(0) == "newcontent");

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileModifyHandlerTests, UsersReadPermAddAndRemove){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileModifyHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"users_with_read_permission_add\":[\"nicolas\"]}",
        &header_names, &header_values);

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

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithReadPermission()->size() == 0);

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithReadPermission()->size() == 1);
    EXPECT_TRUE(arch.getUsersWithReadPermission()->at(0) == "nicolas");

    hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"users_with_read_permission_remove\":[\"nicolas\"]}",
        &header_names, &header_values);

    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithReadPermission()->size() == 0);

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileModifyHandlerTests, UsersWritePermAddAndRemove){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileModifyHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"users_with_write_permission_add\":[\"nicolas\"]}",
        &header_names, &header_values);

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

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithWritePermission()->size() == 0);

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithWritePermission()->size() == 1);
    EXPECT_TRUE(arch.getUsersWithWritePermission()->at(0) == "nicolas");

    hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"users_with_write_permission_remove\":[\"nicolas\"]}",
        &header_names, &header_values);

    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithWritePermission()->size() == 0);

    delete_http_message(hmsg);
    delete conn;
}

TEST(FileModifyHandlerTests, TagsAddAndRemove){
    Database* db = new DatabaseMockRAM;
    TokenAuthenticator* tk = new TokenAuthenticator;
    string token = tk->createToken("gabriel");
    FileModifyHandler handler(db, tk);

    vector<string> header_names;
    header_names.push_back("conn_token");
    vector<string> header_values;
    header_values.push_back(token);
    struct mg_connection* conn = new struct mg_connection;
    struct http_message* hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"tags_add\":[\"test\"]}",
        &header_names, &header_values);

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

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getTags()->size() == 0);

    HttpRequestConcrete req;
    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getTags()->size() == 1);
    EXPECT_TRUE(arch.getTags()->at(0) == "test");

    hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
        "{\"owner_username\":\"gabriel\","
        "\"ubicacion\":\"chile\","
        "\"tags_delete\":[\"test\"]}",
        &header_names, &header_values);

    req.init(conn, hmsg);

    handler.handle(req);
    EXPECT_TRUE(req.getStatusCode() == HttpRequest::OK);

    arch.reSync();
    s = arch.DBget_for_read("gabriel");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getTags()->size() == 0);

    delete_http_message(hmsg);
    delete conn;
}

/*
struct http_message* hmsg = new_http_message("PUT", "/files/gabriel/test.txt",
    "{\"owner_username\":\"gabriel\","
    "\"ubicacion\":\"chile\","
    "\"filename_change\":\"teste.txt\","
    "\"content_change\":\"newcontent\","
    "\"delete_versions\":[0],"
    "\"users_with_read_permission_add\":[\"nicolas\"],"
    "\"users_with_read_permission_remove\":[\"nicolas\"],"
    "\"users_with_write_permission_add\":[\"nicolas\"],"
    "\"users_with_write_permission_remove\":[\"nicolas\"],"
    "\"tags_add\":[\"test\"],"
    "\"tags_remove\":[\"test\"]}",
    &header_names, &header_values);
*/


