#include "DatabaseMockRAM.h"
#include "gtest/gtest.h"

class MockDBElement : public DBElement {
    public:
	MockDBElement(Database* db) : DBElement(db){}
        void _setKey() { key = "clave"; }
        void _setValue() { value = "value"; }
        void _setValueVars() {}
};

TEST(DatabaseMockRAMTests, funcionesTriviales){
    DatabaseMockRAM db;
    db.clear_all();

    db.config("", true);
    db.open();
    db.close();

    EXPECT_TRUE(true);
}

TEST(DatabaseMockRAMTests, getTest){
    DatabaseMockRAM db;
    db.clear_all();

    MockDBElement elem_in(&db);
    db.put(elem_in);

    MockDBElement elem_out(&db);
    elem_out.setKey("clave");
    db.get(elem_out);

    EXPECT_TRUE(elem_in.getValueToString().compare(elem_out.getValueToString()) == 0);
}

TEST(DatabaseMockRAMTests, eraseTest){
    DatabaseMockRAM db;
    db.clear_all();

    MockDBElement elem_in(&db);
    db.put(elem_in);

    MockDBElement elem_out(&db);
    elem_out.setKey("clave");
    db.get(elem_out);

    EXPECT_TRUE(elem_in.getValueToString().compare(elem_out.getValueToString()) == 0);

    db.erase(elem_in);

    MockDBElement elem_out2(&db);
    db.get(elem_out2);
    EXPECT_FALSE(elem_in.getValueToString().compare(elem_out2.getValueToString()) == 0);
}
/*
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
<<<<<<< HEAD
}




=======
}*/

