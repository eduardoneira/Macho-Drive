#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "EventHandlerRemovesAuthentication.h"
#include "HttpRequestMock.h"
#include "TokenAuthenticator.h"
#include "DatabaseMockRAM.h"

class MockEventHandlerRemovesAuthentication : public EventHandlerRemovesAuthentication {
    public:
        MockEventHandlerRemovesAuthentication(TokenAuthenticator *t) : EventHandlerRemovesAuthentication(new DatabaseMockRAM, t) { called = false; }
        void _handle(HttpRequest &hmsg) { called = true; auth->removeToken(hmsg.getUsername()); }

        bool called;
};

TEST(EventHandlerRemovesAuthenticationTests, QuitaPermiso){
    TokenAuthenticator tka;
    MockEventHandlerRemovesAuthentication h(&tka);
    EXPECT_FALSE(h.called);

    std::string username = "gabriel";
    std::string token = tka.createToken(username);
    EXPECT_FALSE(h.called);

    h.handle(*(new HttpRequestMock(username, token)));
    EXPECT_TRUE(h.called);
    EXPECT_FALSE(tka.isValidToken(username, token));
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




