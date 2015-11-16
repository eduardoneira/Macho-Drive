#include "gtest/gtest.h"
#include "DBElementMock.h"
#include "DatabaseMockRAM.h"

TEST(DBElementTest, TestGetKeyEsDefinidoPorElEstadoDelHijo){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    DBElementMock e(db);

    std::string clave = "clave";
    e.setMockClave(clave);
    EXPECT_TRUE(e.getKey() == clave);

    e.setKey(clave);
    EXPECT_TRUE(e.getKeyToString() == clave);
}

TEST(DBElementTest, TestGetValueEsDefinidoPorElEstadoDelHijo){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    DBElementMock e(db);

    std::string value = "value";
    e.setMockValue(value);
    EXPECT_TRUE(e.getValue() == value);

    e.setValue(value);
    EXPECT_TRUE(e.getValueToString() == value);
}

TEST(DBElementTest, PutSeGuardaEnBaseDeDatos){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    DBElementMock e(db);

    std::string clave = "clave";
    e.setMockClave(clave);
    e.put();
    EXPECT_TRUE((db->get(e)).ok());
}

TEST(DBElementTest, GetSeBuscaEnBaseDeDatos){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    DBElementMock e(db);

    Status s;

    std::string clave = "clave";
    std::string value = "value";
    e.setMockClave(clave);
    s = e.get();
    EXPECT_FALSE(s.ok());
    e.setMockValue(value);
    s = e.put();
    EXPECT_TRUE(s.ok());
    e.setMockValue("");
    e.setMockClave(clave);
    e.reSync();
    s = e.get();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(e.getMockValue() == value);
}

TEST(DBElementTest, EraseSeBorraEnLaBaseDeDatos){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    DBElementMock e(db);

    std::string clave = "clave";
    std::string value = "value";
    e.setMockClave(clave);
    e.setMockValue(value);
    e.put();
    EXPECT_TRUE((db->get(e)).ok());
    e.erase();
    EXPECT_TRUE(!(db->get(e)).ok());
}

TEST(DBElementTest, ConBatchNoSeHacenApenasLasPido){
    DatabaseMockRAM* db = new DatabaseMockRAM;
    DBElementMock e(db);

    std::string clave = "clave";
    std::string value = "value";
    e.setMockClave(clave);
    e.setMockValue(value);

    e.startBatch();
    e.put();
    EXPECT_TRUE(!(db->get(e)).ok());
    e.endBatch();
    EXPECT_TRUE((db->get(e)).ok());
}
/*
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}*/



