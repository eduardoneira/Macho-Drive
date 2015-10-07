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

Status FileData::DBaddUserWithReadPermission(std::string user){
    Status s;

    s = this->db->get(*this);
    // ver status
    addUserWithReadPermission(user);
    s = this->db->put(*this);
    // ver status

    UserMetadata user_metadata(db);
    user_metadata.setUsername(user);
    s = user_metadata.DBadd_shared_file(this->getOwnerUsername(), this->getFilename());
    // ver status

    return s;
}

Status FileData::DBaddUserWithWritePermission(std::string user){
    Status s;

    s = this->db->get(*this);
    // ver status
    addUserWithWritePermission(user);
    s = this->db->put(*this);
    // ver status

    UserMetadata user_metadata(db);
    user_metadata.setUsername(user);
    s = user_metadata.DBadd_shared_file(this->getOwnerUsername(), this->getFilename());
    // ver status

    return s;
}

Status FileData::DBremoveUserWithReadPermission(std::string user){
    Status s;

    s = this->db->get(*this);
    // ver status
    removeUserWithReadPermission(user);
    s = this->db->put(*this);
    // ver status

    UserMetadata user_metadata(db);
    user_metadata.setUsername(user);
    s = user_metadata.DBremove_shared_file(this->getOwnerUsername(), this->getFilename());
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
    setOwnerUsername(JsonSerializer::get(json_value, "username", "", temp_value, temp_str_value));
    setDateLastModified(JsonSerializer::get(json_value, "date_last_modified", "", temp_value, temp_str_value));
    setUserWhoLastModified(JsonSerializer::get(json_value, "user_who_last_modified", "", temp_value, temp_str_value));

    /*setContent(json_value["content"].toStyledString());
    setFilename(json_value["filename"].toStyledString());
    setExtension(json_value["extension"].toStyledString());
    setOwnerUsername(json_value["username"].toStyledString());
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
    serializer.addValueToObjectList(val_json, "username", owner_username);
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

    //s = this->DBsetContent("", "");
    s = this->db->get(*this);
    // ver status
    if(!s.ok()){
        return s;
    }
    s = this->db->erase(*this);
    // ver status

    UserMetadata owner_user_metadata(db);
    owner_user_metadata.setUsername(this->getOwnerUsername());
    owner_user_metadata.DBremove_my_file(this->getFilename(), content.size());

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

    return s;
}

void FileData::setContent(std::string n_content){
    this->content = n_content;
}

Status FileData::DBsetContent(std::string n_content, std::string ubicacion){
    Status s;

    s = this->db->get(*this);
    // ver status

    UserMetadata user_metadata(db);
    user_metadata.setUsername(this->getOwnerUsername());

    double new_size = n_content.size();
    double old_size = this->content.size();
    double dif_add = new_size - old_size;

    if(!user_metadata.DBhas_enough_cuota(dif_add)){
        // error, el usuario no tiene cuota suficiente
        return Status::Aborted();
    } else {
        this->setContent(n_content);
        s = user_metadata.DBmodif_file(dif_add, ubicacion);
        // ver status
    }

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
    s = this->db->put(*this);
    // ver status

    /// actualizar registro de tags
    /*FileTag file_tag(db);
    file_tag.setTag(tag);
    this->db->get(file_tag);
    file_tag.addFileToken(file_data.getKey());
    this->db->put(file_tag);*/

    return s;
}

Status FileData::DBremoveTag(std::string tag){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->removeTag(tag);
    s = this->db->put(*this);
    // ver status

    /// actualizar registro de tags?
    /*FileTag file_tag(db);
    file_tag.setTag(tag);
    this->db->get(file_tag);
    file_tag.removeFileToken(file_data.getKey());
    this->db->put(file_tag);*/

    return s;
}

Status FileData::DBget(){
    Status s;

    s = this->db->get(*this);
    // ver status
    return s;
}

Status FileData::DBsetFilename(std::string new_filename){
    Status s = Status::OK();

    if(new_filename != this->getFilename()){
        s = this->db->get(*this);
        // ver status

        s = this->db->erase(*this);
        // ver status
        this->setFilename(new_filename);
        s = this->db->put(*this);

        /// agregar archivo a su filename?
        /*FileName file_name(db);
        file_name.setName(file_data.getFilename());
        file_name.setUsername(owner_username);
        s = this->db->get(file_name);
        file_name.addFileToken(file_data.getKey());
        s = this->db->put(file_name);*/
        /*if(filename_new != ""){
            this->db->erase(file_data);

            FileName file_name_old(db);
            file_name_old.setName(file_data.getFilename());
            this->db->get(file_name_old);
            file_name_old.removeFileToken(file_data.getKey());
            this->db->put(file_name_old);

            FileName file_name_new(db);
            file_name_new.setName(filename_new);
            this->db->get(file_name_new);
            file_name_new.addFileToken(file_data.getKey());
            this->db->put(file_name_new);

            file_data.setFilename(filename_new);
        }*/
    }

    std::string extension_new = get_longest_extension_from_filename(new_filename);
    if(extension_new != this->getExtension()){
        s = this->DBsetExtension(extension_new);
        // ver status
    }

    return s;
}

Status FileData::DBsetExtension(std::string new_extension){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->setExtension(new_extension);
    /// actualizar registro de extensiones?
    /*
    FileExtension file_extension(db);
    file_extension.setExtension(file_data.getExtension());
    file_extension.setUsername(owner_username);
    s = this->db->get(file_extension);
    file_extension.addFileToken(file_data.getKey());
    s = this->db->put(file_extension);

    FileExtension ext_old(db);
    ext_old.setExtension(file_data.getExtension());
    s = this->db->get(ext_old);
    ext_old.removeFileToken(file_data.getKey());
    s = this->db->put(ext_old);

    FileExtension ext_new(db);
    ext_new.setExtension(extension_new);
    s = this->db->get(ext_new);
    ext_new.addFileToken(file_data.getKey());
    s = this->db->put(ext_new);
    */

    s = this->db->put(*this);

    return s;
}

Status FileData::DBchangeModified(std::string username){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->setDateLastModified(get_date_and_time());
    this->setUserWhoLastModified(username);
    s = this->db->put(*this);
    // ver status
    return s;
}

Status FileData::DBcreate(std::string n_content, std::string ubicacion){
    Status s;

    s = this->db->get(*this);
    // ver status, si ya existe devolver error

    // agregar archivo a base de datos
    s = this->db->put(*this);
    // ver status

    s = this->DBsetContent(n_content, ubicacion);
    // ver status (si no alcanza la cuota terminar aca y borrar el archivo vacio que se agrego con this->db->erase(*this)

    /// seteos iniciales
    s = this->DBsetExtension(get_longest_extension_from_filename(this->getFilename()));
    // ver status

    s = this->DBchangeModified(this->getOwnerUsername());
    // ver status

    s = this->DBsetFilename(this->getFilename());
    // ver status

    // agregar archivo a su usuario
    UserMetadata user_metadata(db);
    user_metadata.setUsername(this->getOwnerUsername());
    s = user_metadata.DBadd_my_file(this->getFilename()/*, content.size(), ubicacion*/);
    // ver status

    return s;
}
