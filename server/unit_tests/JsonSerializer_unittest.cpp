
#include "gtest/gtest.h"
#include "JsonSerializer.h"
#include "json/json.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define ERROR "error"

using namespace Json;

TEST(JsonSerializerTest, crearValue_nom_valor){
    std::string name = "nombre";
    std::string val = "pepe";

    std::string val_json = "";

    JsonSerializer serializer;
    serializer.addValueToObjectList(val_json, name, val);
    serializer.turnObjectListToObject(val_json);

    Reader reader;
    Value value;
    reader.parse(val_json, value);
    //std::cout << val_json << std::endl;
    //std::cout << value.toStyledString() << std::endl;
    //std::cout << value.get("as", "error").toStyledString() << std::endl;
    //std::cout << value[name].toStyledString() /*<< std::endl*/;
    std::string str_res = "";
    Value val_res;
    serializer.get(value, name, ERROR, val_res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare(val) == 0);
}

TEST(JsonSerializerTest, crearListaValues_nom_valor){

    std::string val_json = "";

    JsonSerializer serializer;
    serializer.addValueToObjectList(val_json, "gabriel", "gayoso");
    serializer.addValueToObjectList(val_json, "nicolas", "keklikian");
    serializer.addValueToObjectList(val_json, "cristian", "salas");
    serializer.addValueToObjectList(val_json, "eduardo", "neira");
    serializer.turnObjectListToObject(val_json);

    Reader reader;
    Value value;
    reader.parse(val_json, value);
    //std::cout << val_json << std::endl;
    //std::cout << value.toStyledString() << std::endl;

    Value res;
    std::string str_res;

    serializer.get(value, "gabriel", ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare("gayoso") == 0);
    serializer.get(value, "nicolas", ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare("keklikian") == 0);
    serializer.get(value, "eduardo", ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare("neira") == 0);
    serializer.get(value, "cristian", ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare("salas") == 0);
}

TEST(JsonSerializerTest, crearArrayValues_nom_valor){

    std::string val_json = "";

    JsonSerializer serializer;
    serializer.addValueToArrayList(val_json, "gabriel");
    //std::cout << val_json << std::endl;
    serializer.addValueToArrayList(val_json, "nicolas");
    //std::cout << val_json << std::endl;
    serializer.addValueToArrayList(val_json, "cristian");
    //std::cout << val_json << std::endl;
    serializer.addValueToArrayList(val_json, "eduardo");
    //std::cout << val_json << std::endl;
    serializer.turnArrayListToArray(val_json, "nombres");
    //std::cout << val_json << std::endl;
    serializer.turnObjectListToObject(val_json);
    //std::cout << val_json << std::endl;

    Reader reader;
    Value value;
    reader.parse(val_json, value);
    //std::cout << val_json << std::endl;
    //std::cout << value.toStyledString() << std::endl;

    Value res_array, res;
    std::string str_res;

    serializer.get(value, "nombres", ERROR, res_array, str_res);
    //std::cout << str_res << std::endl;
    serializer.get(res_array, 0, ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare(ERROR) != 0);

    serializer.get(res_array, 1, ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare(ERROR) != 0);

    serializer.get(res_array, 2, ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare(ERROR) != 0);

    serializer.get(res_array, 3, ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare(ERROR) != 0);
}

TEST(JsonSerializerTest, crearArrayValuesDeVector_nom_valor){

    std::string val_json = "";

    JsonSerializer serializer;

    std::vector<std::string> nombres;
    nombres.push_back("gabriel");
    nombres.push_back("nicolas");
    nombres.push_back("cristian");
    nombres.push_back("eduardo");

    serializer.turnVectorToArray(nombres, "nombres", val_json);
    serializer.turnObjectListToObject(val_json);

    Reader reader;
    Value value;
    reader.parse(val_json, value);
    //std::cout << val_json << std::endl;
    //std::cout << value.toStyledString() << std::endl;

    Value res_array, res;
    std::string str_res;

    serializer.get(value, "nombres", ERROR, res_array, str_res);
    //std::cout << str_res << std::endl;
    serializer.get(res_array, 0, ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare(ERROR) != 0);

    serializer.get(res_array, 1, ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare(ERROR) != 0);

    serializer.get(res_array, 2, ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare(ERROR) != 0);

    serializer.get(res_array, 3, ERROR, res, str_res);
    //std::cout << str_res << std::endl;
    EXPECT_TRUE(str_res.compare(ERROR) != 0);
}

TEST(JsonSerializerTest, userMetadataSerialization){
    std::vector<std::string> my_file_tokens;
    my_file_tokens.push_back("135253");
    my_file_tokens.push_back("073621");
    my_file_tokens.push_back("680235");

    std::vector<std::string> shared_file_tokens;
    shared_file_tokens.push_back("098748");

    std::string join_date = "26/09/2015";

    JsonSerializer serializer;

    std::string array_my_file_tokens = "";
    serializer.turnVectorToArray(my_file_tokens, "my_file_tokens", array_my_file_tokens);
    //std::cout << array_my_file_tokens << std::endl;
    std::string array_shared_file_tokens = "";
    serializer.turnVectorToArray(shared_file_tokens, "shared_file_tokens", array_shared_file_tokens);
    //std::cout << array_shared_file_tokens << std::endl;
    std::string val_date_joined = "";
    serializer.addValueToObjectList(val_date_joined, "join_date", join_date);
    //std::cout << val_date_joined << std::endl;

    std::string val_json = "";
    serializer.joinValueIntoList(val_json, array_my_file_tokens);
    serializer.joinValueIntoList(val_json, array_shared_file_tokens);
    serializer.joinValueIntoList(val_json, val_date_joined);
    serializer.turnObjectListToObject(val_json);

    std::ofstream log;
    log.open("JsonSerializer_unittest_LOG.txt", std::ios::app);
    if(!log){
        log.open("JsonSerializer_unittest_LOG.txt", std::ios::trunc);
    }
    log << "Json armado: " << std::endl;
    log << val_json << std::endl;

    Reader reader;
    Value value;
    reader.parse(val_json, value);
    log << "Json parseado: " << std::endl;
    log << value.toStyledString() << std::endl;
    log.close();

    // hardcodeo max, pero bueno es para una prueba
    //EXPECT_TRUE(val_json.compare("{ \"my_file_tokens\" : [ \"135253\", \"073621\", \"680235\" ], \"shared_file_tokens\" : [ \"098748\" ], \"join_date\" : \"26/09/2015\" }") == 0);
}

TEST(JsonSerializerTest, fileDataSerialization){
    JsonSerializer serializer;

    std::string content = "21349087140";
    std::string filename = "mi_arch";
    std::string extension = ".txt";
    std::string owner_username = "gabriel";
    std::string owner_key = "gabriel"; // si es el MD5 del username no hace falta tenerlo

    std::vector<std::string> users_with_read_permission; // tal vez deberian ser sets para evitar duplicados
    users_with_read_permission.push_back("eduardo");
    users_with_read_permission.push_back("cristian");
    users_with_read_permission.push_back("nicolas");
    std::vector<std::string> users_with_write_permission;
    users_with_write_permission.push_back("eduardo");
    users_with_write_permission.push_back("cristian");
    users_with_write_permission.push_back("nicolas");

    std::string date_last_modified = "27/09/2015";
    std::string user_who_modified = "gabriel";

    std::vector<std::string> tags; // tal vez deberia ser set
    tags.push_back("taller 2");
    tags.push_back("server");

    std::string val_json = "";
    serializer.addValueToObjectList(val_json, "content", content);
    serializer.addValueToObjectList(val_json, "filename", filename);
    serializer.addValueToObjectList(val_json, "extension", extension);
    serializer.addValueToObjectList(val_json, "username", owner_username);
    serializer.addValueToObjectList(val_json, "owner_key", owner_key);
    serializer.addValueToObjectList(val_json, "date_last_modified", date_last_modified);
    serializer.addValueToObjectList(val_json, "user_who_last_modified", user_who_modified);

    std::string array_users_with_read_permission = "";
    serializer.turnVectorToArray(users_with_read_permission, "users_with_read_permission", array_users_with_read_permission);
    std::string array_users_with_write_permission = "";
    serializer.turnVectorToArray(users_with_write_permission, "users_with_write_permission", array_users_with_write_permission);
    std::string array_tags = "";
    serializer.turnVectorToArray(tags, "tags", array_tags);

    serializer.joinValueIntoList(val_json, array_users_with_read_permission);
    serializer.joinValueIntoList(val_json, array_users_with_write_permission);
    serializer.joinValueIntoList(val_json, array_tags);
    serializer.turnObjectListToObject(val_json);

    std::ofstream log;
    log.open("JsonSerializer_unittest_LOG.txt", std::ios::app);
    if(!log){
        log.open("JsonSerializer_unittest_LOG.txt", std::ios::trunc);
    }
    log << "fileDataSerializationTest" << std::endl;
    log << "Json armado: " << std::endl;
    log << val_json << std::endl;

    Reader reader;
    Value value;
    reader.parse(val_json, value);
    log << "Json parseado: " << std::endl;
    log << value.toStyledString() << std::endl;

    log.close();
}
/*
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}*/
