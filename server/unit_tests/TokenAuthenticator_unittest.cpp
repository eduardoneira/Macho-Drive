#include "gtest/gtest.h"
#include "TokenAuthenticator.h"

TEST(TokenAuthenticatorTest, crearTokenAgregaUnTokenNoVacio){
    TokenAuthenticator tka;
    EXPECT_FALSE(tka.createToken("gabriel") == "");
}

TEST(TokenAuthenticatorTest, isValidTokenDevuelveFalsoSiUserYTokenNoCoinciden){
    TokenAuthenticator tka;
    std::string tk1 = tka.createToken("gabriel");
    EXPECT_FALSE(tk1 == "");
    std::string tk2 = tka.createToken("nicolas");
    EXPECT_FALSE(tk2 == "");
    EXPECT_FALSE(tka.isValidToken("gabriel", tk2));
    EXPECT_FALSE(tka.isValidToken("nicolas", tk1));
    EXPECT_TRUE(tka.isValidToken("gabriel", tk1));
    EXPECT_TRUE(tka.isValidToken("nicolas", tk2));
}

TEST(TokenAuthenticatorTest, siYaHabiaTokenParaUnUserYCreoOtroSePisaElAnterior){
    TokenAuthenticator tka;
    std::string tk1 = tka.createToken("gabriel");
    EXPECT_FALSE(tk1 == "");
    std::string tk2 = tka.createToken("gabriel");
    EXPECT_FALSE(tk1 == tk2);
    EXPECT_FALSE(tka.isValidToken("gabriel", tk1));
    EXPECT_TRUE(tka.isValidToken("gabriel", tk2));
}

TEST(TokenAuthenticatorTest, borrarTokenFuncionaSiEseTokenExiste){
    TokenAuthenticator tka;
    EXPECT_FALSE(tka.removeToken("gabriel"));
    EXPECT_FALSE(tka.createToken("gabriel") == "");
    EXPECT_TRUE(tka.removeToken("gabriel"));
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
