
#include "gtest/gtest.h"
#include "Database.h"
#include "rocksdb/status.h"
#include "DBElement.h"

TEST(DatabaseTest, NoPuedoCrearDBSinConfigurarla){
    Database db;
    rocksdb::Status s = db.create();
    EXPECT_FALSE(s.ok());
}

TEST(DatabaseTest, CrearDBConfiguradaNoFalla){
    Database db;
    rocksdb::Status s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.create();
    EXPECT_TRUE(s.ok());
}

TEST(DatabaseTest, NoPuedoConfigurarDBCuandoFueCreada){
    Database db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.create();
    EXPECT_TRUE(s.ok());

    s = db.config("/tmp/otrodir");
    EXPECT_FALSE(s.ok());
}

/*class MockDBelement : public DBelement {
    public:
        MOCK_METHOD1(put, void(Database &db));
};*/

TEST(DatabaseTest, PuedoGuardarElemento){
    Database db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.create();
    EXPECT_TRUE(s.ok());

    EXPECT_TRUE(false); // completar
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
