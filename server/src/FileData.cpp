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
    //temp.append(std::to_string(std::time(nullptr))); - por ahora, porque no se actualiza la metadata del usuario

    get_md5_hash(temp, this->key);
}

void FileData::_setValueVars(){
    Reader reader;
    Value json_value;
    reader.parse(this->value, json_value);

    Value temp_value;
    std::string temp_str_value;

    /*setContent(JsonSerializer::get(json_value, "content", "", temp_value, temp_str_value));
    setFilename(JsonSerializer::get(json_value, "filename", "", temp_value, temp_str_value));
    setExtension(JsonSerializer::get(json_value, "extension", "", temp_value, temp_str_value));
    setOwnerUsername(JsonSerializer::get(json_value, "owner_username", "", temp_value, temp_str_value));
    setDateLastModified(JsonSerializer::get(json_value, "date_last_modified", "", temp_value, temp_str_value));
    setUserWhoLastModified(JsonSerializer::get(json_value, "user_who_last_modified", "", temp_value, temp_str_value));
*/
    setContent(json_value["content"].toStyledString());
    setFilename(json_value["filename"].toStyledString());
    setExtension(json_value["extension"].toStyledString());
    setOwnerUsername(json_value["owner_username"].toStyledString());
    setDateLastModified(json_value["date_last_modified"].toStyledString());
    setUserWhoLastModified(json_value["user_who_last_modified"].toStyledString());

    for(ValueIterator it = json_value["users_with_read_permission"].begin(); it != json_value["users_with_read_permission"].end(); ++it){
        addUserWithReadPermission((*it).asString());
    }
    for(ValueIterator it = json_value["users_with_write_permission"].begin(); it != json_value["users_with_write_permission"].end(); ++it){
        addUserWithWritePermission((*it).asString());
    }
    for(ValueIterator it = json_value["tags"].begin(); it != json_value["tags"].end(); ++it){
        addTag((*it).asString());
    }
}

void FileData::_setValue(){
    JsonSerializer serializer;

    std::string val_json = "";
    serializer.addValueToObjectList(val_json, "content", content);
    serializer.addValueToObjectList(val_json, "filename", filename);
    serializer.addValueToObjectList(val_json, "extension", extension);
    serializer.addValueToObjectList(val_json, "owner_username", owner_username);
    //serializer.addValueToObjectList(val_json, "owner_key", owner_key);
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

    this->value = val_json;
}
