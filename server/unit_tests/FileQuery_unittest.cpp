#include "gtest/gtest.h"
#include "FileQuery.h"
#include "DatabaseMockRAM.h"
#include "rocksdb/status.h"
#include "UserMetadata.h"
#include "FileData.h"
#include "User.h"
#include "json/json.h"

std::string user = "yo";
std::string user2 = "vos";
std::string name1 = "arch1";
std::string name2 = "arch2";
std::string name3 = "chile1";

using namespace Json;

TEST(FileQueryTest,TestEmptySearch){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    std::string word = "";
    std::string meta = "";

    User u(db);
    u.setUsername(user);
    u.setPassword("chile");
    Status s = u.DBcreate();
    EXPECT_TRUE(s.ok());

    FileData arch(db);
    arch.setOwnerUsername(user);
    arch.setFilename("boton");
    s = arch.DBcreate("asdasd", "chile");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE((db->get(arch)).ok());

    FileQuery query(user,meta,word,db);

    EXPECT_TRUE(query.getValidFiles().size() == 0);

    query.search_files();

    EXPECT_TRUE(query.getValidFiles().size() == 1);

    FileQuery query2(user,"INVALID","HI",db);
    query2.search_files();

    EXPECT_TRUE(query2.getValidFiles().size() == 0);
}

TEST(FileQueryTest,TestSearchMetadata){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    std::string word = "arch";
    std::string meta = "NAME";

    User u(db);
    u.setUsername(user);
    u.setPassword("chile");
    Status s = u.DBcreate();
    EXPECT_TRUE(s.ok());

    FileData arch(db);
    arch.setOwnerUsername(user);
    arch.setFilename(name1);
    s = arch.DBcreate("asddsadasfgaasd", "Unico");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE((db->get(arch)).ok());

    FileData arch2(db);
    arch2.setOwnerUsername(user);
    arch2.setFilename(name2);
    s = arch2.DBcreate("asdasd", "chile");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE((db->get(arch2)).ok());

    FileData arch3(db);
    arch3.setOwnerUsername(user);
    arch3.setFilename(name3);
    s = arch3.DBcreate("39", "gato");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE((db->get(arch3)).ok());

    FileQuery query(user,meta,word,db);
    query.search_files();

    EXPECT_TRUE(query.getValidFiles().size() == 2);

    Reader reader;
    Value response_parsed;
    std::string response;

    response = query.getResponse();
    reader.parse(response.c_str(), response_parsed);
    EXPECT_TRUE(response_parsed["word_searched"] == word);
    EXPECT_TRUE(response_parsed["metadata_field"] == meta);

    word="yo";
    meta="OWNER";
    FileQuery query2(user,meta,word,db);
    query2.search_files();

    EXPECT_TRUE(query2.getValidFiles().size() == 3);

    word="hola";
    meta="TAG";
    FileQuery query3(user,meta,word,db);
    query3.search_files();

    EXPECT_TRUE(query3.getValidFiles().size() == 0);

    word="hola";
    meta="EXTENSION";
    FileQuery query4(user,meta,word,db);
    query4.search_files();

    EXPECT_TRUE(query4.getValidFiles().size() == 0);


}
