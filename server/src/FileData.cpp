#include "FileData.h"
#include <ctime>
#include "Util.h"
#include "JsonSerializer.h"

FileData::FileData()
{
    //ctor
}

FileData::~FileData()
{
    //dtor
}

void FileData::addUserWithReadPermission(std::string user_key){
    users_with_read_permission.push_back(user_key);
}

void FileData::addUserWithWritePermission(std::string user_key){
    users_with_write_permission.push_back(user_key);
    addUserWithReadPermission(user_key);
}

void FileData::_setKey(){
    // armo la clave como owner_username+filename+tiempo (asumiendo que un usuario no puede subir el mismo archivo en el mismo momento dos veces)
    std::string temp = "";
    temp.append(this->owner_username);
    temp.append(this->filename);
    temp.append(std::to_string(std::time(nullptr)));

    get_md5_hash(temp, this->key);
}

void FileData::_setValue(){
    JsonSerializer serializer;

    std::string val_json = "";
    serializer.addValueToObjectList(val_json, "content", content);
    serializer.addValueToObjectList(val_json, "filename", filename);
    serializer.addValueToObjectList(val_json, "extension", extension);
    serializer.addValueToObjectList(val_json, "owner_username", owner_username);
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

    this->value = val_json;
}
