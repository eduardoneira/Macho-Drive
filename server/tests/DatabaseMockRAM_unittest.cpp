#include "DatabaseMockRAM.h"
#include "gtest/gtest.h"

class MockDBElement : public DBElement {
    public:
        void _setKey() { key = "clave"; }
        void _setValue() { value = "value"; }
};

TEST(DatabaseMockRAMTests, getTest){
    DatabaseMockRAM db;
    db.clear_all();

    MockDBElement elem_in;
    db.put(elem_in);

    MockDBElement elem_out;
    elem_out.setKey("clave");
    db.get(elem_out);

    EXPECT_TRUE(elem_in.getValueToString().compare(elem_out.getValueToString()) == 0);
}

TEST(DatabaseMockRAMTests, eraseTest){
    DatabaseMockRAM db;
    db.clear_all();

    MockDBElement elem_in;
    db.put(elem_in);

    MockDBElement elem_out;
    elem_out.setKey("clave");
    db.get(elem_out);

    EXPECT_TRUE(elem_in.getValueToString().compare(elem_out.getValueToString()) == 0);

    db.erase(elem_in);
    db.get(elem_out);
    EXPECT_FALSE(elem_in.getValueToString().compare(elem_out.getValueToString()) == 0);
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
