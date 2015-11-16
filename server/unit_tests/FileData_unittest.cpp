#include "gtest/gtest.h"
#include "FileData.h"
#include "DatabaseMockRAM.h"
#include "User.h"
#include "UserMetadata.h"

using namespace std;

Database* db = new DatabaseMockRAM;
std::string gabo = "gabriel";
std::string edu = "eduardo";
std::string nico = "nicolas";
std::string cristian = "cristian";
User u_gabo(db);
User u_edu(db);
User u_nico(db);
User u_cristian(db);
vector<string> empty_str;
vector<int> empty_int;

void setUp(){
    db->clear_all();

    u_gabo.setUsername(gabo);
    u_gabo.setPassword("gayoso");
    u_gabo.DBcreate();
    u_gabo.reSync();

    u_edu.setUsername(edu);
    u_edu.setPassword("neira");
    u_edu.DBcreate();
    u_edu.reSync();

    u_nico.setUsername(nico);
    u_nico.setPassword("keklikian");
    u_nico.DBcreate();
    u_nico.reSync();

    u_cristian.setUsername(cristian);
    u_cristian.setPassword("salas");
    u_cristian.DBcreate();
    u_cristian.reSync();
}

TEST(FileDataTests, GetForReadFallaSiUsuarioNoExiste){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read("DecanoRosito");
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, GetForReadFallaSiUsuarioNoTienePermiso){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(nico);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, GetForReadOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> permisos_read;
    permisos_read.push_back(nico);
    vector<string> permisos_write;
    permisos_write.push_back(edu);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", permisos_read, empty_str, permisos_write, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(nico);
    EXPECT_TRUE(s.ok());
    arch.reSync();
    s = arch.DBget_for_read(edu);
    EXPECT_TRUE(s.ok());
}

TEST(FileDataTests, ModifyFallaSiUsuarioNoTieneNingunPermiso){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(nico, "asd.txt", "chile", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, ModifyFallaSiUsuarioNoExiste){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify("OsoArturo", "asd.txt", "chile", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, ModifyFallaSiUsuarioNoTienePermisoEscritura){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());
    vector<string> permisos_read;
    permisos_read.push_back(nico);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", permisos_read, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(nico, "asd.txt", "chile", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, ModifyCambioDeNombreOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(gabo, "asd.txt", "", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    UserMetadata um_gabo(db);
    um_gabo.setUsername(gabo);
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_gabo.getMy_files()->at(0) == "asd.txt");

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getFilename() == "asd.txt");
}

TEST(FileDataTests, ModifyCambioDeNombreFallaSiYaTengoArchConEseNombre){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    FileData arch2(db);
    arch2.setOwnerUsername(gabo);
    arch2.setFilename("asd.txt");
    s = arch2.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(gabo, "asd.txt", "", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());

    UserMetadata um_gabo(db);
    um_gabo.setUsername(gabo);
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_gabo.getMy_files()->at(0) == "arch1.txt");
    EXPECT_TRUE(um_gabo.getMy_files()->at(1) == "asd.txt");

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getFilename() == "arch1.txt");
}

TEST(FileDataTests, ModifyCambioDeNombreOKConPermisoEscritura){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());
    vector<string> permisos_write;
    permisos_write.push_back(nico);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, permisos_write, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(nico, "asd.txt", "", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    UserMetadata um_gabo(db);
    um_gabo.setUsername(gabo);
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_gabo.getMy_files()->at(0) == "asd.txt");

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getFilename() == "asd.txt");
}

TEST(FileDataTests, ModifyCambioDeNombreCambiaExtensionOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    FileData arch2(db);
    arch2.setOwnerUsername(gabo);
    arch2.setFilename("asd.txt");
    s = arch2.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(gabo, "asd.jpg", "", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    UserMetadata um_gabo(db);
    um_gabo.setUsername(gabo);
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_gabo.getMy_files()->at(0) == "asd.jpg");
    EXPECT_TRUE(um_gabo.getMy_files()->at(1) == "asd.txt");

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getFilename() == "asd.jpg");
}

TEST(FileDataTests, ModifyCambioDeContenidoFallaSiNoHayCuota){
    setUp();

    string content = "contendioasdasdas";
    string n_content = "cambios";

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate(content, "argentina");
    EXPECT_TRUE(s.ok());

    UserMetadata um_gabo(db);
    um_gabo.setUsername(gabo);
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    double cuota_actual = um_gabo.getCuotaActual();
    um_gabo.reSync();
    s = um_gabo.DBsetCuotaMax(content.size()+2);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(gabo, "", "", n_content, empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
    // veo q no hay cambio
    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getContent() == content);

    um_gabo.reSync();
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_gabo.getCuotaActual() == cuota_actual);
}

TEST(FileDataTests, ModifyCambioDeContenidoOK){
    setUp();

    string content = "contendioasdasdas";
    string n_content = "cambios";

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate(content, "argentina");
    EXPECT_TRUE(s.ok());

    UserMetadata um_gabo(db);
    um_gabo.setUsername(gabo);
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    double cuota_actual = um_gabo.getCuotaActual();

    arch.reSync();
    s = arch.DBmodify(gabo, "", "", n_content, empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
    // veo q no hay cambio
    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getContent() == n_content);

    um_gabo.reSync();
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_gabo.getCuotaActual() >= cuota_actual);
}

TEST(FileDataTests, ModifyCambioDeContenidoAgregaVersion){
    setUp();

    string content = "contendioasdasdas";
    string n_content = "cambios";

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate(content, "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(gabo, "", "", n_content, empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
    // veo q no hay cambio
    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getVersions()->size() == 2);
}

TEST(FileDataTests, ModifyCambioDeContenidoOKConPermisoEscritura){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());
    vector<string> permisos_write;
    permisos_write.push_back(nico);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, permisos_write, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    string n_content = "cambios";
    arch.reSync();
    s = arch.DBmodify(nico, "", "", n_content, empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getContent() == n_content);
}

TEST(FileDataTests, ModifyCambiarPermisosFallaAunConPermisoDeEscritura){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());
    vector<string> permisos_write;
    permisos_write.push_back(nico);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, permisos_write, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    vector<string> perm;
    perm.push_back(cristian);
    arch.reSync();
    s = arch.DBmodify(nico, "", "", "", perm, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
    arch.reSync();
    s = arch.DBmodify(nico, "", "", "", empty_str, perm, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
    arch.reSync();
    s = arch.DBmodify(nico, "", "", "", empty_str, empty_str, perm, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
    arch.reSync();
    s = arch.DBmodify(nico, "", "", "", empty_str, empty_str, empty_str, perm, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, ModifyDarPermisoFallaSiElUsuarioNoExiste){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> perm;
    perm.push_back("Lorthcaramon");
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", perm, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, perm, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, ModifyDarPermisoFallaSiElUsuarioEsDuenio){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> perm;
    perm.push_back(gabo);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", perm, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, ModifyDarPermisoLecturaOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> perm;
    perm.push_back(nico);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", perm, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithReadPermission()->at(0) == nico);

    UserMetadata um_nico(db);
    um_nico.setUsername(nico);
    s = um_nico.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_nico.getShared_files()->at(0).first == gabo);
    EXPECT_TRUE(um_nico.getShared_files()->at(0).second == "arch1.txt");
}

TEST(FileDataTests, ModifyDarPermisoEscrituraOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> perm;
    perm.push_back(nico);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, perm, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getUsersWithReadPermission()->at(0) == nico);
    EXPECT_TRUE(arch.getUsersWithWritePermission()->at(0) == nico);

    UserMetadata um_nico(db);
    um_nico.setUsername(nico);
    s = um_nico.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_nico.getShared_files()->at(0).first == gabo);
    EXPECT_TRUE(um_nico.getShared_files()->at(0).second == "arch1.txt");
}

TEST(FileDataTests, ModifyBorrarPermisoLecturaOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> perm;
    perm.push_back(nico);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, perm, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, perm, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
}

TEST(FileDataTests, ModifyBorrarPermisoEscrituraOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> perm;
    perm.push_back(nico);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, perm, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, empty_str, perm, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
}

TEST(FileDataTests, ModifyAddTagsOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> tags;
    tags.push_back("test");
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, empty_str, empty_str, tags, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getTags()->at(0) == "test");
}

TEST(FileDataTests, ModifyRemoveTagsOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> tags;
    tags.push_back("test");
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, empty_str, empty_str, tags, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getTags()->at(0) == "test");

    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, empty_str, empty_str, empty_str, tags, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getTags()->size() == 0);
}

TEST(FileDataTests, ModifyDeleteVersionFallaAunConPermisoDeEscritura){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> perm;
    perm.push_back(nico);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, perm, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "lkashda", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getVersions()->size() == 2);

    arch.reSync();
    vector<int> erase_versions;
    erase_versions.push_back(0);
    s = arch.DBmodify(nico, "", "", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, erase_versions);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, ModifyDeleteVersionFallaSiSoloHayUnaVersion){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "lkashda", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "lkashasdda", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getVersions()->size() == 3);

    arch.reSync();
    vector<int> erase_versions;
    erase_versions.push_back(5);
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, erase_versions);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, ModifyDeleteVersionOKDisminuyeCuotaActual){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "lkashda", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "lkashasdda", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());
    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getVersions()->size() == 3);
    UserMetadata um_gabo(db);
    um_gabo.setUsername(gabo);
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    double cuota_con_3 = um_gabo.getCuotaActual();

    arch.reSync();
    vector<int> erase_versions;
    erase_versions.push_back(0);
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, erase_versions);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getVersions()->size() == 2);
    um_gabo.reSync();
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_gabo.getCuotaActual() <= cuota_con_3);
}

TEST(FileDataTests, ModifyDeleteVersionFallaSiElNroIndicadoEsInvalido){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    s = arch.DBget_for_read(gabo);
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(arch.getVersions()->size() == 1);

    arch.reSync();
    vector<int> erase_versions;
    erase_versions.push_back(0);
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, empty_str, empty_str, empty_str, empty_str, erase_versions);
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, BorrarArchivoFallaSiNoEsMiArchivo){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    arch.reSync();
    arch.setFilename("archivin.txt");
    s = arch.DBerase();
    EXPECT_FALSE(s.ok());

    vector<string> perm;
    perm.push_back(edu);
    arch.reSync();
    arch.setFilename("arch1.txt");
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, perm, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    arch.reSync();
    arch.setOwnerUsername(edu);
    arch.setFilename("arch1.txt");
    s = arch.DBerase();
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, BorrarArchivoOKSeBorraDeSharedFiles){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    vector<string> perm;
    perm.push_back(edu);
    arch.reSync();
    s = arch.DBmodify(gabo, "", "", "", empty_str, empty_str, perm, empty_str, empty_str, empty_str, empty_int);
    EXPECT_TRUE(s.ok());

    UserMetadata um_gabo(db);
    um_gabo.setUsername(gabo);
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_gabo.getMy_files()->at(0) == "arch1.txt");

    UserMetadata um_edu(db);
    um_edu.setUsername(edu);
    s = um_edu.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_edu.getShared_files()->at(0).second == "arch1.txt");

    arch.reSync();
    s = arch.DBerase();
    EXPECT_TRUE(s.ok());

    um_gabo.reSync();
    s = um_gabo.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_gabo.getMy_files()->size() == 0);

    um_edu.reSync();
    s = um_edu.DBget();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(um_edu.getShared_files()->size() == 0);
}

TEST(FileDataTests, CrearArchivoOK){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());
}

TEST(FileDataTests, CrearArchivoFallaSiNoExisteElUsuario){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername("ArielAlegre");
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, CrearArchivoFallaSiElUsuarioNoTieneCuotaSuficiente){
    setUp();

    UserMetadata um_gabo(db);
    um_gabo.setUsername(gabo);
    Status s = um_gabo.DBsetCuotaMax(0);
    EXPECT_TRUE(s.ok());

    FileData arch(db);
    arch.setOwnerUsername("arielalegre");
    arch.setFilename("arch1.txt");
    s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, CrearArchivoFallaSiYaExisteOtroConEseNombreEnElMismoUsuario){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    FileData arch2(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    s = arch.DBcreate("contendioasdasdas2", "argentina");
    EXPECT_FALSE(s.ok());
}

TEST(FileDataTests, CrearArchivoOKSiExisteOtroConEseNombreEnOtroUsuario){
    setUp();

    FileData arch(db);
    arch.setOwnerUsername(gabo);
    arch.setFilename("arch1.txt");
    Status s = arch.DBcreate("contendioasdasdas", "argentina");
    EXPECT_TRUE(s.ok());

    FileData arch2(db);
    arch2.setOwnerUsername(nico);
    arch2.setFilename("arch1.txt");
    s = arch2.DBcreate("contendioasdasdas2", "argentina");
    EXPECT_TRUE(s.ok());
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

