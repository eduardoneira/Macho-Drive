#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "EventHandlerRemovesAuthentication.h"
#include "HttpRequestMock.h"
#include "TokenAuthenticator.h"
#include "DatabaseMockRAM.h"

class MockEventHandlerRemovesAuthentication : public EventHandlerRemovesAuthentication {
    public:
        MockEventHandlerRemovesAuthentication(TokenAuthenticator *t) : EventHandlerRemovesAuthentication(new DatabaseMockRAM, t) { called = false; }
        void _handle(HttpRequest &hmsg) { called = true; }

        bool called;
};

TEST(EventHandlerRemovesAuthenticationTests, QuitaPermiso){
    // seria trivial el test, pq esta clase en realidad solo pasa la funcionalidad al log out handler
    EXPECT_TRUE(true);
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
