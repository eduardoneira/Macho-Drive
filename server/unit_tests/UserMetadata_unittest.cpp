#include "gtest/gtest.h"
#include "UserMetadata.h"
#include "User.h"
#include "DatabaseMockRAM.h"
#include "FileData.h"
#include "DatabaseRocksDB.h"
#include "json/json.h"

std::string username = "gabriel";
std::string password = "gayoso";
std::string username2 = "nicolas";
std::string password2 = "keklikian";
std::string my_file = "my_file.txt";
std::string my_file2 = "my_file2.jpg";
std::string sharing_user = "sharing_user";
std::string shared_file = "shared_file";
std::string hoy = "hoy";
double cuota_max = 100;
double cuota_actual = 20;
std::string email = "ga-yo-so@hotmail.com";
std::string ubicacion = "ubicacion";

TEST(UserMetadataTest, GettersYSettersTest){
    DatabaseMockRAM* db = new DatabaseMockRAM;

    UserMetadata user_metadata(db);

    user_metadata.setUsername(username);
    EXPECT_TRUE(user_metadata.getUsername() == username);

    std::vector<std::string>* my_files = NULL;
    my_files = user_metadata.getMy_files();
    EXPECT_TRUE(my_files != NULL);

    std::vector<std::pair<std::string, std::string>>* shared_files = NULL;
    shared_files = user_metadata.getShared_files();
    EXPECT_TRUE(shared_files != NULL);

    user_metadata.DB_move_to_bin(my_file);
    EXPECT_TRUE(user_metadata.getAllFilesBin().size() == 0);
    user_metadata.addMyFile(my_file);
    EXPECT_TRUE(my_files->at(0) == my_file);

    user_metadata.DB_move_to_bin(my_file);
    EXPECT_TRUE(user_metadata.getAllFilesBin().size() == 1);

    user_metadata.removeMyFile(my_file);
    EXPECT_TRUE(user_metadata.getAllFilesBin().size() == 0);

    user_metadata.addSharedFile(shared_file, sharing_user);
    EXPECT_TRUE(shared_files->at(0) == std::make_pair(sharing_user, shared_file));
    user_metadata.removeSharedFile(shared_file, sharing_user);
    EXPECT_TRUE(shared_files->size() == 0);

    user_metadata.setJoinDate(hoy);
    EXPECT_TRUE(user_metadata.getJoinDate() == hoy);

    user_metadata.setCuotaMax(cuota_max);
    EXPECT_TRUE(user_metadata.getCuotaMax() == cuota_max);

    user_metadata.changeEmail(email);
    EXPECT_TRUE(user_metadata.getEmail() == email);

    user_metadata.changeUltimaUbicacion(ubicacion);
    EXPECT_TRUE(user_metadata.getUltimaUbicacion() == ubicacion);

    user_metadata.add_to_cuota(cuota_actual);
    EXPECT_TRUE(user_metadata.getCuotaActual() == cuota_actual);

    user_metadata.remove_from_cuota(cuota_actual);
    EXPECT_TRUE(user_metadata.getCuotaActual() == 0);

    user_metadata.setCuotaActual(cuota_actual);
    EXPECT_TRUE(user_metadata.getCuotaActual() == cuota_actual);
}

TEST(UserMetadataTest, CrearUsuarioCreaMetadata){
    DatabaseMockRAM* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername(username);
    user.setPassword(password);

    user.DBcreate();

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    Status s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());
}

TEST(UserMetadataTest, BorrarUsuarioBorraMetadataQueBorraMisArchivos){
    DatabaseMockRAM* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername(username);
    user.setPassword(password);
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    User user2(db);
    user2.setUsername(username2);
    user2.setPassword(password2);
    s = user2.DBcreate();
    EXPECT_TRUE(s.ok());

    FileData arch(db);
    arch.setOwnerUsername(username);
    arch.setFilename(my_file);
    s = arch.DBcreate("asdasd", "chile");
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE((db->get(arch)).ok());
    std::vector<std::string> permisos, empty;
    std::vector<int> empty2;
    permisos.push_back(username2);
    arch.reSync();
    s = arch.DBmodify(username, "", "", "", permisos, empty, empty, empty, empty, empty, empty2);
    EXPECT_TRUE(s.ok());

    UserMetadata um2(db);
    um2.setUsername(username2);
    s = um2.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um2.getShared_files()->at(0).first == username);
    EXPECT_TRUE(um2.getShared_files()->at(0).second == my_file);

    s = user.DBerase();
    EXPECT_TRUE(s.ok());
    EXPECT_FALSE((db->get(arch)).ok());
    um2.reSync();
    s = um2.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um2.getShared_files()->size() == 0);
}

TEST(UserMetadataTest, ChangeEmailTest){
    DatabaseMockRAM* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername(username);
    user.setPassword(password);

    user.DBcreate();

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    Status s = user_metadata.DBchange_email(email);
    s = user_metadata.DBget();
    EXPECT_TRUE(user_metadata.getEmail() == email);
}

TEST(UserMetadataTest, MyFileTest){
    Database* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername(username);
    user.setPassword(password);

    user.DBcreate();

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    bool check = user_metadata.DBisMyFile(my_file);
    EXPECT_FALSE(check);

    FileData arch(db);
    arch.setOwnerUsername(username);
    arch.setFilename(my_file);
    Status s = arch.DBcreate("asd", ubicacion);
    EXPECT_TRUE(s.ok());
    user_metadata.reSync();
    check = user_metadata.DBisMyFile(my_file);
    EXPECT_TRUE(check);

    // cambio de nombre
    std::vector<std::string> permisos, empty;
    std::vector<int> empty2;
    user_metadata.reSync();
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());
    std::string filename_old = user_metadata.getMy_files()->at(0);
    std::string filename_new = "asdasd.txt";
    arch.reSync();
    s = arch.DBmodify(username, filename_new, "", "", empty, empty, empty, empty, empty, empty, empty2);
    EXPECT_TRUE(s.ok());
    user_metadata.reSync();
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(user_metadata.getMy_files()->at(0) == filename_new);
}

TEST(UserMetadataTest, ModificarArchivoCambiaUltimaUbicacion){
    Database* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername(username);
    user.setPassword(password);
    user.DBcreate();
    user.reSync();
    user.setUsername(username2);
    user.setPassword(password2);
    user.DBcreate();

    FileData arch1(db);
    arch1.setOwnerUsername(username);
    arch1.setFilename(my_file);
    Status s = arch1.DBcreate("asd", "argentina");
    EXPECT_TRUE(s.ok());

    FileData arch2(db);
    arch2.setOwnerUsername(username2);
    arch2.setFilename(my_file2);
    s = arch2.DBcreate("asdasd", "chile");
    EXPECT_TRUE(s.ok());
    std::vector<std::string> permisos, empty;
    std::vector<int> empty2;
    permisos.push_back(username);
    arch2.reSync();
    s = arch2.DBmodify(username2, "", "", "", permisos, empty, empty, empty, empty, empty, empty2);
    EXPECT_TRUE(s.ok());

    std::string ultima_ubicacion_1 = "chile";

    arch1.reSync();
    s = arch1.DBmodify(username, "", ultima_ubicacion_1, "cambios", empty, empty, empty, empty, empty, empty, empty2);
    EXPECT_TRUE(s.ok());

    UserMetadata user_metadata1(db);
    user_metadata1.setUsername(username);
    s = user_metadata1.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(ultima_ubicacion_1 == user_metadata1.getUltimaUbicacion());

    std::string ultima_ubicacion_2 = "peru";

    arch1.reSync();
    s = arch1.DBmodify(username, "", ultima_ubicacion_2, "cambios2", empty, empty, empty, empty, empty, empty, empty2);
    EXPECT_TRUE(s.ok());

    user_metadata1.reSync();
    s = user_metadata1.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(ultima_ubicacion_2 == user_metadata1.getUltimaUbicacion());
}

TEST(UserMetadataTest, TenerArchivosAumentaCuota){
    Database* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername(username);
    user.setPassword(password);
    user.DBcreate();

    FileData arch(db);
    arch.setOwnerUsername(username);
    arch.setFilename(my_file);
    Status s = arch.DBcreate(my_file, ubicacion);
    EXPECT_TRUE(s.ok());

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(user_metadata.getCuotaActual() > 0);
}

TEST(UserMetadataTest, BorrarArchivosAumentaCuota){
    Database* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername(username);
    user.setPassword(password);
    user.DBcreate();

    FileData arch(db);
    arch.setOwnerUsername(username);
    arch.setFilename(my_file);
    Status s = arch.DBcreate(my_file, ubicacion);
    EXPECT_TRUE(s.ok());

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(user_metadata.getCuotaActual() > 0);

    arch.reSync();
    s = arch.DBdelete_file();
    EXPECT_TRUE(s.ok());

    user_metadata.reSync();
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(user_metadata.getCuotaActual() == 0);
}

TEST(UserMetadataTest, SharedFileTest){
    Database* db = new DatabaseMockRAM;

    User user(db);
    user.setUsername(username);
    user.setPassword(password);
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    User user2(db);
    user2.setUsername(username2);
    user2.setPassword(password2);
    s = user2.DBcreate();
    EXPECT_TRUE(s.ok());

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    std::pair<std::string, std::string> check = user_metadata.DBisSharedFile(my_file);
    EXPECT_FALSE(check.first != "");

    FileData arch(db);
    arch.setOwnerUsername(username2);
    arch.setFilename(my_file);
    s = arch.DBcreate("asd", ubicacion);
    EXPECT_TRUE(s.ok());
    user_metadata.reSync();
    check = user_metadata.DBisSharedFile(my_file);
    EXPECT_FALSE(check.first != "");

    std::vector<std::string> permisos, empty;
    std::vector<int> empty2;
    permisos.push_back(username);
    arch.reSync();
    s = arch.DBmodify(username2, "", ubicacion, "", permisos, empty, empty, empty, empty, empty, empty2);
    EXPECT_TRUE(s.ok());
    user_metadata.reSync();
    check = user_metadata.DBisSharedFile(my_file);
    EXPECT_TRUE(check.first != "");

    // cambio de nombre
    user_metadata.reSync();
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());
    std::string filename_old = user_metadata.getShared_files()->at(0).second;
    std::string filename_new = "asdasd.txt";
    arch.reSync();
    s = arch.DBmodify(username2, filename_new, "", "", empty, empty, empty, empty, empty, empty, empty2);
    EXPECT_TRUE(s.ok());
    user_metadata.reSync();
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(user_metadata.getShared_files()->at(0).second == filename_new);
}

using namespace Json;

TEST(UserMetadataTest, ProfileTest){
    Database* db = new DatabaseMockRAM;
    std::string picture = "hola";
    std::string place = "Chile";
    std::string name = "Lauti";

    User user(db);
    user.setUsername(username);
    user.setPassword(password);
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());

    s = user_metadata.DBchange_email(email);
    EXPECT_TRUE(s.ok());
    s = user_metadata.DBchange_profile_pic(picture);
    EXPECT_TRUE(s.ok());
    s = user_metadata.DBchange_last_place(place);
    EXPECT_TRUE(s.ok());
    s = user_metadata.DBchange_name(name);
    EXPECT_TRUE(s.ok());

    Reader reader;
    Value response_parsed;
    std::string profile = user_metadata.getProfile();

    reader.parse(profile.c_str(), response_parsed);
    EXPECT_TRUE(response_parsed["email"] == email);
    EXPECT_TRUE(response_parsed["picture"] == picture);
    EXPECT_TRUE(response_parsed["name"] == name);
    EXPECT_TRUE(response_parsed["place"] == place);

}

TEST(UserMetadataTest, JsonInvolvedTest){
    Database* db = new DatabaseMockRAM;
    std::string picture = "hola";
    std::string place = "Chile";
    std::string name = "Lauti";

    User user(db);
    user.setUsername(username);
    user.setPassword(password);
    Status s = user.DBcreate();
    EXPECT_TRUE(s.ok());

    FileData arch(db);
    arch.setOwnerUsername(username);
    arch.setFilename(my_file);
    s = arch.DBcreate("asd", ubicacion);

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    s = user_metadata.DBget();
    EXPECT_TRUE(s.ok());

    Reader reader;
    Value response_parsed;
    std::string response = user_metadata.getFileTreeJson();
    reader.parse(response.c_str(), response_parsed);

    EXPECT_TRUE(response_parsed["my_file_tokens"] != "");

    user_metadata.DB_move_to_bin(my_file);
    response = user_metadata.getRecycleBin();
    reader.parse(response.c_str(), response_parsed);
    EXPECT_TRUE(user_metadata.getAll_files().size() == 0);
    EXPECT_TRUE(response_parsed["username"] == username);
    EXPECT_TRUE(response_parsed["files_in_bin"] != "");

    user_metadata.recoverFileRecycleBin(my_file);
    EXPECT_TRUE(user_metadata.getAll_files().size() == 1);
}
/*
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}*/
