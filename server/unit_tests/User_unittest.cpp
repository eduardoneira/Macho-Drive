#include "gtest/gtest.h"
#include "User.h"
#include "DatabaseMockRAM.h"
#include "UserMetadata.h"
#include "FileData.h"

TEST(UserTest, GettersYSettersTest){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    User user(db);
    user.setUsername("gabriel");
    EXPECT_TRUE(user.getUsername() == "gabriel");
    user.setPassword("gayoso");
    EXPECT_TRUE(user.getPassword() == "gayoso");
}

TEST(UserTest, CrearTest){
    DatabaseMockRAM* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");

    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());
    s = user.DBcreate();
    EXPECT_FALSE(s.ok());
}

TEST(UserTest, CrearUsuarioCreaMetadata){
    DatabaseMockRAM* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    UserMetadata um(db);
    um.setUsername("gabriel");
    s = um.DBget();
    EXPECT_TRUE(s.ok());
}

TEST(UserTest, GetTest){
    DatabaseMockRAM* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername("gabriel");

    Status s = user.DBget();
    EXPECT_FALSE(s.ok());

    user.setPassword("gayoso");
    user.reSync();
    s = user.DBcreate();
    EXPECT_TRUE(s.ok());
    user.setPassword("asd");
    user.reSync();
    s = user.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(user.getPassword() == "gayoso");
}

TEST(UserTest, EraseTest){
    DatabaseMockRAM* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");

    Status s = user.DBerase();
    EXPECT_FALSE(s.ok());
    s = user.DBcreate();
    EXPECT_TRUE((db->get(user)).ok());
    s = user.DBerase();
    EXPECT_FALSE((db->get(user)).ok());
}

TEST(UserTest, BorrarUsuarioBorraMetadata){
    DatabaseMockRAM* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername("gabriel");
    user.setPassword("gayoso");
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    UserMetadata um(db);
    um.setUsername("gabriel");
    s = um.DBget();
    EXPECT_TRUE(s.ok());

    s = user.DBerase();
    EXPECT_FALSE((db->get(user)).ok());
    um.reSync();
    s = um.DBget();
    EXPECT_FALSE(s.ok());
}
/*
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}*/

