#include "gtest/gtest.h"
#include "DatabaseWriteBatch.h"
#include "DatabaseMockRAM.h"
#include "DBElementMock.h"

TEST(DatabaseWriteBatchTests, conPutAgregoPuts){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    DatabaseWriteBatch batch(db);

    std::vector< std::string >* keys;
    std::vector< std::string >* values;
    std::vector< std::string >* operations;

    keys = batch.getKeys();
    values = batch.getValues();
    operations = batch.getOperations();
    EXPECT_TRUE(keys->size() == 0);
    EXPECT_TRUE(values->size() == 0);
    EXPECT_TRUE(operations->size() == 0);

    DBElementMock m(db, "gabriel", "gayoso");

    batch.Put(&m);

    keys = batch.getKeys();
    values = batch.getValues();
    operations = batch.getOperations();
    EXPECT_TRUE(keys->at(0) == m.getMockClave());
    EXPECT_TRUE(values->at(0) == m.getMockValue());
    EXPECT_TRUE(operations->at(0) == "put");

    DBElementMock m2(db, "nicolas", "keklikian");

    batch.Put(&m2);

    keys = batch.getKeys();
    values = batch.getValues();
    operations = batch.getOperations();
    EXPECT_TRUE(keys->at(0) == m.getMockClave());
    EXPECT_TRUE(values->at(0) == m.getMockValue());
    EXPECT_TRUE(operations->at(0) == "put");
    EXPECT_TRUE(keys->at(1) == m2.getMockClave());
    EXPECT_TRUE(values->at(1) == m2.getMockValue());
    EXPECT_TRUE(operations->at(1) == "put");
}
/*
TEST(DatabaseWriteBatchTests, conEraseAgregoErases){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    DatabaseWriteBatch batch(db);

    std::vector< std::string >* keys;
    std::vector< std::string >* values;
    std::vector< std::string >* operations;

    keys = batch.getKeys();
    values = batch.getValues();
    operations = batch.getOperations();
    EXPECT_TRUE(keys->size() == 0);
    EXPECT_TRUE(values->size() == 0);
    EXPECT_TRUE(operations->size() == 0);

    DBElementMock m(db, "gabriel", "gayoso");

    batch.Erase(&m);

    keys = batch.getKeys();
    values = batch.getValues();
    operations = batch.getOperations();
    EXPECT_TRUE(keys->at(0) == m.getMockClave());
    EXPECT_TRUE(values->at(0) == m.getMockValue());
    EXPECT_TRUE(operations->at(0) == "erase");

    DBElementMock m2(db, "nicolas", "keklikian");

    batch.Erase(&m2);

    keys = batch.getKeys();
    values = batch.getValues();
    operations = batch.getOperations();
    EXPECT_TRUE(keys->at(0) == m.getMockClave());
    EXPECT_TRUE(values->at(0) == m.getMockValue());
    EXPECT_TRUE(operations->at(0) == "erase");
    EXPECT_TRUE(keys->at(1) == m2.getMockClave());
    EXPECT_TRUE(values->at(1) == m2.getMockValue());
    EXPECT_TRUE(operations->at(1) == "erase");
}

TEST(DatabaseWriteBatchTests, puedoMezclarPutsYErases){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    DatabaseWriteBatch batch(db);

    std::vector< std::string >* keys;
    std::vector< std::string >* values;
    std::vector< std::string >* operations;

    keys = batch.getKeys();
    values = batch.getValues();
    operations = batch.getOperations();
    EXPECT_TRUE(keys->size() == 0);
    EXPECT_TRUE(values->size() == 0);
    EXPECT_TRUE(operations->size() == 0);

    DBElementMock m(db, "gabriel", "gayoso");

    batch.Erase(&m);

    keys = batch.getKeys();
    values = batch.getValues();
    operations = batch.getOperations();
    EXPECT_TRUE(keys->at(0) == m.getMockClave());
    EXPECT_TRUE(values->at(0) == m.getMockValue());
    EXPECT_TRUE(operations->at(0) == "erase");

    DBElementMock m2(db, "nicolas", "keklikian");

    batch.Put(&m2);

    keys = batch.getKeys();
    values = batch.getValues();
    operations = batch.getOperations();
    EXPECT_TRUE(keys->at(0) == m.getMockClave());
    EXPECT_TRUE(values->at(0) == m.getMockValue());
    EXPECT_TRUE(operations->at(0) == "erase");
    EXPECT_TRUE(keys->at(1) == m2.getMockClave());
    EXPECT_TRUE(values->at(1) == m2.getMockValue());
    EXPECT_TRUE(operations->at(1) == "put");
}*/

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
