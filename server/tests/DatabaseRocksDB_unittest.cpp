
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "DatabaseRocksDB.h"
#include "rocksdb/status.h"
#include "DBElement.h"

using namespace rocksdb;
using namespace testing;

TEST(DatabaseTest, NoPuedoAbrirDBSinConfigurarla){
    DatabaseRocksDB db;
    rocksdb::Status s = db.open();
    EXPECT_FALSE(s.ok());
}

TEST(DatabaseTest, CrearDBConfiguradaNoFalla){
    DatabaseRocksDB db;
    rocksdb::Status s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.open();
    EXPECT_TRUE(s.ok());
}

TEST(DatabaseTest, NoPuedoConfigurarDBCuandoFueAbierta){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.open();
    EXPECT_TRUE(s.ok());

    s = db.config("/tmp/otrodir");
    EXPECT_FALSE(s.ok());
}

TEST(DatabaseTest, NoPuedoAbrirDBSiYaEstaAbierta){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.open();
    EXPECT_TRUE(s.ok());

    s = db.open(); // ya lo maneja rocksdb parece
    EXPECT_FALSE(s.ok());
}

// no deberia ir aca, segun google deberia ir con la clase que mockea. que chupen limon
class MockDBElement : public DBElement {
    public:
        void _setKey() { key = "clave"; }
        void _setValue() { value = "value"; }
};

class MockOtroDBElement : public DBElement {
    public:
        void _setKey() { key = "clave"; }
        void _setValue() { value = "value2"; }
};

class MockUnoMasDBElement : public DBElement {
    public:
        void _setKey() { key = "asd"; }
        void _setValue() { value = "asdasd"; }
};

TEST(DatabaseTest, PuedoGuardarYRecuperarElemento){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.open();
    EXPECT_TRUE(s.ok());

    s = db.clear_all();
    EXPECT_TRUE(s.ok());

    MockDBElement e_in;
    s = db.put(e_in);
    EXPECT_TRUE(s.ok());

    MockDBElement e_out;
    s = db.get(e_out);
    EXPECT_TRUE(s.ok());

    //std::cout << "in: " << e_in.getKeyToString() << ", out: " << e_out.getKeyToString() << std::endl;
    //std::cout << "in: " << e_in.getValueToString() << ", out: " << e_out.getValueToString() << std::endl;

    EXPECT_TRUE(e_in.getKeyToString().compare(e_out.getKeyToString()) == 0);
    EXPECT_TRUE(e_in.getValueToString().compare(e_out.getValueToString()) == 0);
}

TEST(DatabaseTest, GuardarDosClavesIgualesPisaElValorAnterior){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.open();
    EXPECT_TRUE(s.ok());

    s = db.clear_all();
    EXPECT_TRUE(s.ok());

    MockDBElement e_in1;
    s = db.put(e_in1);
    EXPECT_TRUE(s.ok());

    MockOtroDBElement e_in2;
    s = db.put(e_in2);
    EXPECT_TRUE(s.ok());

    MockOtroDBElement e_out2;
    s = db.get(e_out2);
    EXPECT_TRUE(s.ok());

    EXPECT_FALSE(e_in1.getValueToString().compare(e_out2.getValueToString()) == 0);
    EXPECT_TRUE(e_in2.getValueToString().compare(e_out2.getValueToString()) == 0);
}

TEST(DatabaseTest, NoPuedoRecuperarDatoQueNoIngreseAntes){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.open();
    EXPECT_TRUE(s.ok());

    s = db.clear_all();
    EXPECT_TRUE(s.ok());

    MockUnoMasDBElement e_out;
    s = db.get(e_out);
    EXPECT_FALSE(s.ok());
}

TEST(DatabaseTest, LimpiarDBBorraTodasLasClaves){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.open();
    EXPECT_TRUE(s.ok());

    MockDBElement e_in;
    s = db.put(e_in);
    EXPECT_TRUE(s.ok());

    s = db.clear_all();
    EXPECT_TRUE(s.ok());

    MockDBElement e_out;
    s = db.get(e_out);
    EXPECT_FALSE(s.ok());
    EXPECT_TRUE(s.IsNotFound());
}

TEST(DatabaseTest, NoPuedoLimpiarSiNoAbriDB){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.clear_all();
    EXPECT_FALSE(s.ok());
}

TEST(DatabaseTest, NoPuedoGuardarElemSiNoAbriDB){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    MockDBElement e_in;
    s = db.put(e_in);
    EXPECT_FALSE(s.ok());
}

TEST(DatabaseTest, NoPuedoRecuperarElemSiNoAbriDB){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    MockDBElement e_out;
    s = db.get(e_out);
    EXPECT_FALSE(s.ok());
}

TEST(DatabaseTest, NoPuedoBorrarElemSiNoAbriDB){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    MockDBElement e_out;
    s = db.erase(e_out);
    EXPECT_FALSE(s.ok());
}

TEST(DatabaseTest, BorrarNoDaErrorSiNoExisteLaClave){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.open();
    EXPECT_TRUE(s.ok());

    s = db.clear_all();
    EXPECT_TRUE(s.ok());

    MockDBElement e_out;
    s = db.erase(e_out);
    EXPECT_TRUE(s.ok());
}

TEST(DatabaseTest, SiBorroClaveNoEstaMasEnLaDB){
    DatabaseRocksDB db;
    rocksdb::Status s;

    s = db.config("/tmp/test");
    EXPECT_TRUE(s.ok());

    s = db.open();
    EXPECT_TRUE(s.ok());

    MockDBElement e_in;
    s = db.put(e_in);
    EXPECT_TRUE(s.ok());

    s = db.erase(e_in);
    EXPECT_TRUE(s.ok());

    MockDBElement e_out;
    s = db.get(e_out);
    EXPECT_FALSE(s.ok());
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
