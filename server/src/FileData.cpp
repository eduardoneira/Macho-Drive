#include "FileData.h"
#include <ctime>
#include "Util.h"
#include "JsonSerializer.h"
#include "DatabaseRocksDB.h"
#include "UserMetadata.h"

FileData::FileData(Database* db) : DBElement(db)
{
    //ctor
}

FileData::~FileData()
{
    //dtor
}

void FileData::addUserWithReadPermission(std::string user_key){
    if(std::find(users_with_read_permission.begin(), users_with_read_permission.end(), user_key) == users_with_read_permission.end()){
        users_with_read_permission.push_back(user_key);
    }
}

void FileData::addUserWithWritePermission(std::string user_key){
    if(std::find(users_with_write_permission.begin(), users_with_write_permission.end(), user_key) == users_with_write_permission.end()){
        users_with_write_permission.push_back(user_key);
        addUserWithReadPermission(user_key);
    }
}

void FileData::removeUserWithReadPermission(std::string user_key){
    users_with_read_permission.erase(std::remove(users_with_read_permission.begin(), users_with_read_permission.end(), user_key), users_with_read_permission.end());
}

void FileData::removeUserWithWritePermission(std::string user_key){
    users_with_write_permission.erase(std::remove(users_with_write_permission.begin(), users_with_write_permission.end(), user_key), users_with_write_permission.end());
}

Status FileData::DBaddUserWithReadPermission(std::string user_key){
    Status s;

    s = this->db->get(*this);
    // ver status
    addUserWithReadPermission(user_key);
    s = this->db->put(*this);
    // ver status
    return s;
}

Status FileData::DBaddUserWithWritePermission(std::string user_key){
    Status s;

    s = this->db->get(*this);
    // ver status
    addUserWithWritePermission(user_key);
    s = this->db->put(*this);
    // ver status
    // ver status
    return s;
}

Status FileData::DBremoveUserWithReadPermission(std::string user_key){
    Status s;

    s = this->db->get(*this);
    // ver status
    removeUserWithReadPermission(user_key);
    s = this->db->put(*this);
    // ver status
    return s;
}

Status FileData::DBremoveUserWithWritePermission(std::string user_key){
    Status s;

    s = this->db->get(*this);
    // ver status
    removeUserWithWritePermission(user_key);
    s = this->db->put(*this);
    // ver status
    return s;
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

    setContent(JsonSerializer::get(json_value, "content", "", temp_value, temp_str_value));
    setFilename(JsonSerializer::get(json_value, "filename", "", temp_value, temp_str_value));
    setExtension(JsonSerializer::get(json_value, "extension", "", temp_value, temp_str_value));
    setOwnerUsername(JsonSerializer::get(json_value, "owner_username", "", temp_value, temp_str_value));
    setDateLastModified(JsonSerializer::get(json_value, "date_last_modified", "", temp_value, temp_str_value));
    setUserWhoLastModified(JsonSerializer::get(json_value, "user_who_last_modified", "", temp_value, temp_str_value));

    /*setContent(json_value["content"].toStyledString());
    setFilename(json_value["filename"].toStyledString());
    setExtension(json_value["extension"].toStyledString());
    setOwnerUsername(json_value["owner_username"].toStyledString());
    setDateLastModified(json_value["date_last_modified"].toStyledString());
    setUserWhoLastModified(json_value["user_who_last_modified"].toStyledString());*/

    for(ValueIterator it = json_value["users_with_read_permission"].begin(); it != json_value["users_with_read_permission"].end(); ++it){
        addUserWithReadPermission(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }
    for(ValueIterator it = json_value["users_with_write_permission"].begin(); it != json_value["users_with_write_permission"].end(); ++it){
        addUserWithWritePermission(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }
    for(ValueIterator it = json_value["tags"].begin(); it != json_value["tags"].end(); ++it){
        addTag(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
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

// asumo que tengo filename y username correctos
Status FileData::DBerase(){
    Status s;

    s = this->db->get(*this);
    // ver status

    UserMetadata owner_user_metadata(db);
    owner_user_metadata.setUsername(this->getOwnerUsername());
    owner_user_metadata.DBremove_my_file(this->getFilename());

    // TODO: actualizar registros de extension, filename y tags

    for(std::vector<std::string>::iterator it = users_with_read_permission.begin(); it != users_with_read_permission.end(); ++it){
        UserMetadata user_metadata(db);
        user_metadata.setUsername(*it);
        user_metadata.DBremove_shared_file(this->getOwnerUsername(), this->getFilename());
    }

    // como es ahora los que tienen write permission estan incluidos en los que tienen read
    /*for(std::vector<std::string>::iterator it = users_with_write_permission.begin(); it != users_with_write_permission.end(); ++it){
        UserMetadata user_metadata(db);
        user_metadata.setUsername(*it);
        user_metadata.DBremove_shared_file(this->getFilename());
    }*/

    s = this->db->erase(*this);
    // ver status

    return s;
}

Status FileData::DBsetContent(std::string content){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->setContent(content);
    s = this->db->put(*this);
    // ver status

    return s;
}

void FileData::addTag(std::string tag){
    if(std::find(tags.begin(), tags.end(), tag) == tags.end()){
        tags.push_back(tag);
    }
}

Status FileData::DBaddTag(std::string tag){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->addTag(tag);
    /// actualizar registro de tags
    /*for(std::vector<std::string>::iterator it = file_data.getTags()->begin(); it != file_data.getTags()->end(); ++it){
        FileTag file_tag(db);
        file_tag.setTag(*it);
        file_tag.setUsername(owner_username);
        s = this->db->get(file_tag);
        file_tag.addFileToken(file_data.getKey());
        s = this->db->put(file_tag);
    }*/
    s = this->db->put(*this);
    // ver status
    return s;
}

Status FileData::DBget(){
    Status s;

    s = this->db->get(*this);
    // ver status
    return s;
}

Status FileData::DBcreate(){
    Status s;

    s = this->db->get(*this);
    // ver status, si ya existe devolver error

    this->setDateLastModified(get_date_and_time());
    this->setUserWhoLastModified(this->getOwnerUsername());
    // probablemente llamar a una funcion que haga esto
    this->setExtension(get_longest_extension_from_filename(this->getFilename()));
    // crear registro de usuario + extension?
    /*FileExtension file_extension(db);
    file_extension.setExtension(file_data.getExtension());
    file_extension.setUsername(owner_username);
    s = this->db->get(file_extension);
    file_extension.addFileToken(file_data.getKey());
    s = this->db->put(file_extension);*/

    // agregar archivo a su usuario
    UserMetadata user_metadata(db);
    user_metadata.setUsername(this->getOwnerUsername());
    s = user_metadata.DBadd_my_file(this->getFilename());
    // ver status

    /// agregar archivo a su filename?
    /*FileName file_name(db);
    file_name.setName(file_data.getFilename());
    file_name.setUsername(owner_username);
    s = this->db->get(file_name);
    file_name.addFileToken(file_data.getKey());
    s = this->db->put(file_name);*/

    // agregar archivo a base de datos
    s = this->db->put(*this);
    // ver status



    return s;
}
