#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "EventHandlerGrantsAuthentication.h"
#include "HttpRequestMock.h"
#include "TokenAuthenticator.h"
#include "DatabaseMockRAM.h"

class MockEventHandlerGrantsAuthentication : public EventHandlerGrantsAuthentication {
    public:
        MockEventHandlerGrantsAuthentication(TokenAuthenticator *t) : EventHandlerGrantsAuthentication(new DatabaseMockRAM, t) { called = false; }
        void _handle(HttpRequest &hmsg) { called = true; token = auth->createToken(hmsg.getUsername()); }

        bool called;
        std::string token;
    private:
        bool isMyRequest(HttpRequest &hmsg) { return true; }
};

TEST(EventHandlerGrantsAuthenticationTests, OtorgaPermiso){
    TokenAuthenticator tka;
    MockEventHandlerGrantsAuthentication h(&tka);
    EXPECT_FALSE(h.called);

    std::string username = "gabriel";
    EXPECT_FALSE(tka.removeToken(username));
    EXPECT_FALSE(h.called);

    h.handle(*(new HttpRequestMock(username, "")));
    EXPECT_TRUE(h.called);
    EXPECT_TRUE(tka.isValidToken(username, h.token));
}
/*
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
<<<<<<< HEAD
}




=======
}*/

