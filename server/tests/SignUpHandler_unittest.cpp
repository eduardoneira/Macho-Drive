
#include "gtest/gtest.h"
#include "SignUpHandler.h"

TEST(SignUpHandlerTest, CrearSignUpHandlerNoFalla){
    //EXPECT_TRUE(new SignUpHandler());
}

/*TEST(SignUpHandlerTest, SignUpHandlerRegistraUnUsuario){

}*/

// por si queremos que no se acepten simbolos o algo en los nombres de usuario o passwords
/*TEST(SignUpHandlerTest, SignUpHandlerNoAceptaNombresRaros){

}*/

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
