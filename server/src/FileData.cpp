#include "FileData.h"
#include <ctime>
#include "Util.h"
#include "JsonSerializer.h"
#include "DatabaseRocksDB.h"
#include "UserMetadata.h"
#include "Logger.h"

FileData::FileData(Database* db, DatabaseWriteBatch* dbbatch) : DBElement(db, dbbatch)
{
    //ctor
}

FileData::~FileData()
{
    //dtor
}

void FileData::addUserWithReadPermission(std::string user_key){
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Agregando user con permiso de lectura",INFO);
    if(std::find(users_with_read_permission.begin(), users_with_read_permission.end(), user_key) == users_with_read_permission.end()){
        users_with_read_permission.push_back(user_key);
        log->Log("Agregando user con permiso de lectura: "+user_key,INFO);
    }
}

void FileData::addUserWithWritePermission(std::string user_key){
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Agregando user con permiso de escritura",INFO);
    if(std::find(users_with_write_permission.begin(), users_with_write_permission.end(), user_key) == users_with_write_permission.end()){
        log->Log("Agregando user con permiso de escritura: "+user_key,INFO);
        users_with_write_permission.push_back(user_key);
        addUserWithReadPermission(user_key);
    }
}

void FileData::removeUserWithReadPermission(std::string user_key){
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Eliminando user con permiso de escritura",INFO);
    users_with_read_permission.erase(std::remove(users_with_read_permission.begin(), users_with_read_permission.end(), user_key), users_with_read_permission.end());
}

void FileData::removeUserWithWritePermission(std::string user_key){
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Eliminando user con permiso de escritura",INFO);
    users_with_write_permission.erase(std::remove(users_with_write_permission.begin(), users_with_write_permission.end(), user_key), users_with_write_permission.end());
}

Status FileData::_DBaddUserWithReadPermission(std::string user){
    Status s = Status::OK();

    s = this->get();
    // ver status

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Agregando user con permiso de lectura",INFO);

    if(user == this->owner_username){
        return Status::Corruption("el usuario con quien se queria compartir el archivo ya es su duenio");
        log->Log("No se puede agregar user, ya es dueño",WARNING);
    }

    addUserWithReadPermission(user);
    s = this->put();
    // ver status

    log->Log("Agregando archivos a user con read permission",INFO);
    UserMetadata user_metadata(db, this->batch);
    user_metadata.setUsername(user);
    s = user_metadata.DBadd_shared_file(this->getOwnerUsername(), this->getFilename());
    // ver status

    return s;
}

Status FileData::_DBaddUserWithWritePermission(std::string user){
    Status s;

    s = this->get();
    // ver status

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Agregando user con permiso de escritura",INFO);


    if(user == this->owner_username){
        return Status::Corruption("el usuario con quien se queria compartir el archivo ya es su duenio");
        log->Log("No se puede agregar user, ya es dueño",WARNING);
    }

    addUserWithWritePermission(user);
    s = this->put();
    // ver status

    log->Log("Agregando archivos a user con write permission",INFO);

    UserMetadata user_metadata(db, this->batch);
    user_metadata.setUsername(user);
    s = user_metadata.DBadd_shared_file(this->getOwnerUsername(), this->getFilename());
    // ver status

    return s;
}

Status FileData::_DBremoveUserWithReadPermission(std::string user){
    Status s;

    s = this->get();

    removeUserWithReadPermission(user);
    s = this->put();
    // ver status

    UserMetadata user_metadata(db, this->batch);
    user_metadata.setUsername(user);
    s = user_metadata.DBremove_shared_file(this->getOwnerUsername(), this->getFilename());
    // ver status

    return s;
}

Status FileData::_DBremoveUserWithWritePermission(std::string user_key){
    Status s;

    s = this->get();

    removeUserWithWritePermission(user_key);
    s = this->put();
    // ver status
    return s;
}

void FileData::_setKey(){
    // armo la clave como owner_username+filename+tiempo (asumiendo que un usuario no puede subir el mismo archivo en el mismo momento dos veces)
    std::string temp = "";
    temp.append(this->owner_username);
    temp.append(this->filename);
    //temp.append(std::to_string(std::time(nullptr))); - por ahora, porque no se actualiza la metadata del usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Seteando key para archivo de db",INFO);
    get_md5_hash(temp, this->key);
    log->Log("Filename :"+this->filename+" key: "+this->key,TRACE);
}

void FileData::_setValueVars(){
    Reader reader;
    Value json_value;
    reader.parse(this->value, json_value);

    Value temp_value;
    std::string temp_str_value;

    //setContent(JsonSerializer::get(json_value, "content", "", temp_value, temp_str_value));
    setFilename(JsonSerializer::get(json_value, "filename", "", temp_value, temp_str_value));
    setExtension(JsonSerializer::get(json_value, "extension", "", temp_value, temp_str_value));
    setOwnerUsername(JsonSerializer::get(json_value, "username", "", temp_value, temp_str_value));
    setDateLastModified(JsonSerializer::get(json_value, "date_last_modified", "", temp_value, temp_str_value));
    setUserWhoLastModified(JsonSerializer::get(json_value, "user_who_last_modified", "", temp_value, temp_str_value));

    this->content.clear();
    for(ValueIterator it = json_value["content"].begin(); it != json_value["content"].end(); ++it){
        setContent(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }
    this->users_with_read_permission.clear();
    for(ValueIterator it = json_value["users_with_read_permission"].begin(); it != json_value["users_with_read_permission"].end(); ++it){
        addUserWithReadPermission(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }
    this->users_with_write_permission.clear();
    for(ValueIterator it = json_value["users_with_write_permission"].begin(); it != json_value["users_with_write_permission"].end(); ++it){
        addUserWithWritePermission(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }
    this->tags.clear();
    for(ValueIterator it = json_value["tags"].begin(); it != json_value["tags"].end(); ++it){
        addTag(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Seteando value para archivos de la db",INFO);
    log->Log("Filename values: "+this->value,TRACE);
}

void FileData::_setValue(){
    JsonSerializer serializer;

    std::string val_json = "";
    //serializer.addValueToObjectList(val_json, "content", content);
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
    std::string array_content = "";
    serializer.turnVectorToArray(content, "content", array_content);

    serializer.joinValueIntoList(val_json, array_users_with_read_permission);
    serializer.joinValueIntoList(val_json, array_users_with_write_permission);
    serializer.joinValueIntoList(val_json, array_tags);
    serializer.joinValueIntoList(val_json, array_content);
    serializer.turnObjectListToObject(val_json);

    this->value = val_json;
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Guardando value para archivos en db",INFO);
    log->Log("Filename values: "+this->value,TRACE);
}

std::string FileData::getMetadataToString(){
    JsonSerializer serializer;

    std::string val_json = "";
    //serializer.addValueToObjectList(val_json, "content", content);
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

    return val_json;
}

// asumo que tengo filename y username correctos
Status FileData::DBerase(){
    Status s;
    this->startBatch();
    //s = this->DBsetContent("", "");
    s = this->_DBget_for_modify(this->getOwnerUsername());
    if(!s.ok()) return s;

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Eliminando archivo del user y pasandolo a la papelera",INFO);

    UserMetadata owner_user_metadata(db, this->batch);
    owner_user_metadata.setUsername(this->getOwnerUsername());
    owner_user_metadata.DB_move_to_bin(this->getFilename());

    for(std::vector<std::string>::iterator it = users_with_read_permission.begin(); it != users_with_read_permission.end(); ++it){
        UserMetadata user_metadata(db, this->batch);
        user_metadata.setUsername(*it);
        s = user_metadata.DBremove_shared_file(this->getOwnerUsername(), this->getFilename());
        if(!s.ok()) return s;
    }

    log->Log("Eliminando todos los users con read y write permissions",INFO);

    this->users_with_read_permission.clear();
    this->users_with_write_permission.clear();

    s = this->put();

    this->endBatch();
    return s;
}

Status FileData::DBdelete_file(){
   Status s;

    //s = this->DBsetContent("", "");
    s = this->_DBget_for_modify(this->getOwnerUsername());
    if(!s.ok()) return s;

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Eliminando archivo de la base de datos",INFO);

    UserMetadata owner_user_metadata(db, this->batch);
    owner_user_metadata.setUsername(this->getOwnerUsername());
    int tam = this->_contentSize();
    s = owner_user_metadata.DBremove_my_file(this->getFilename(), tam);
    if(!s.ok()) return s;

    for(std::vector<std::string>::iterator it = users_with_read_permission.begin(); it != users_with_read_permission.end(); ++it){
        UserMetadata user_metadata(db, this->batch);
        user_metadata.setUsername(*it);
        s = user_metadata.DBremove_shared_file(this->getOwnerUsername(), this->getFilename());
        if(!s.ok()) return s;
    }

    s = this->erase();
    if(!s.ok()) return s;

    return s;
}

void FileData::setContent(std::string n_content){
    this->content.push_back(n_content);
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Cambiando contenido a : "+n_content,TRACE);
}

int FileData::_contentSize(){
    int total_size = 0;
    for(std::vector<std::string>::iterator it = this->content.begin(); it != this->content.end(); ++it){
        total_size += (*it).size();
    }
    return total_size;
}

Status FileData::_DBsetContent(std::string n_content, UserMetadata* user_metadata){
    Status s;

    s = this->get();
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Cambiando contenido en la db ",INFO);

    if(!s.ok()){
        log->Log("No se encontro el archivo",ERROR);
        return Status::NotFound("File not found");
    }

    //UserMetadata user_metadata(db, this->batch);
    user_metadata->setUsername(this->getOwnerUsername());

    double dif_add = n_content.size();
    bool has_space = false;

    log->Log("Actualizando tamñano cuota ",INFO);
    s = user_metadata->DBhas_enough_cuota(dif_add, has_space);
    if(!s.ok()){
        return s;
    }

    if(!has_space){
        log->Log("No hay suficiente espacio ",WARNING);
        return Status::Corruption("Not enough cuota");
    } else {
        this->setContent(n_content);
        s = user_metadata->DBmodif_file(dif_add);
        // ver status
    }

    s = this->put();
    // ver status

    return s;
}

void FileData::addTag(std::string tag){
    if(std::find(tags.begin(), tags.end(), tag) == tags.end()){
        tags.push_back(tag);
    }
}

Status FileData::_DBaddTag(std::string tag){
    Status s;

    s = this->get();
    // ver status
    this->addTag(tag);
    s = this->put();
    // ver status

    return s;
}

Status FileData::_DBremoveTag(std::string tag){
    Status s;

    s = this->get();
    // ver status
    this->removeTag(tag);
    s = this->put();
    // ver status

    return s;
}

Status FileData::DBget_for_read(std::string username){
    Status s = Status::OK();

    s = this->get();

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Consiguiendo archivo para lectura ",INFO);

    if(!s.ok()){
        log->Log("No se encontro el archivo para lectura ",ERROR);
        return Status::NotFound("File not found");
    }


    if(!this->check_read_permission(username)){
        log->Log("Usuario no tiene permiso para ver archivo ",WARNING);
        return Status::Corruption("Permission denied");
    }
    // ver status
    return s;
}

 Status FileData::_DBget_for_modify(std::string username){
    Status s = Status::OK();

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Modificando archivo con usuario : "+username,INFO);

    s = this->get();
    if(!s.ok()){
        return Status::NotFound("File not found");
    }

    if(!this->check_write_permission(username)){
        log->Log("Usuario no tiene permiso para modificar"+username,WARNING);
        return Status::Corruption("Permission denied");
    }

    return s;
 }

bool FileData::check_read_permission(std::string username){
    if(this->owner_username.compare(username) == 0){
        return true;
    } else if(std::find(this->users_with_read_permission.begin(), this->users_with_read_permission.end(), username) != this->users_with_read_permission.end()){
        return true;
    } else {
        return false;
    }
}

bool FileData::check_write_permission(std::string username){
    if(this->owner_username.compare(username) == 0){
        return true;
    } else if(std::find(this->users_with_write_permission.begin(), this->users_with_write_permission.end(), username) != this->users_with_write_permission.end()){
        return true;
    } else {
        return false;
    }
}

Status FileData::_DBsetFilename(std::string new_filename, UserMetadata* owner_user_metadata){
    Status s = Status::OK();



    FileData tmp(db);
    tmp.setOwnerUsername(this->getOwnerUsername());
    tmp.setFilename(new_filename);
    s = tmp.get();
    if(!s.IsNotFound()){
        return Status::Corruption("Filename already exists");
    }

    this->setFilename(new_filename);
    s = this->put();

    std::string extension_new = get_longest_extension_from_filename(new_filename);
    s = this->_DBsetExtension(extension_new);

    return s;
}

Status FileData::_DBchangeFilename(std::string new_filename, UserMetadata* owner_user_metadata){
    Status s = Status::OK();
    std::string old_filename = this->getFilename();

    FileData tmp(db);
    tmp.setOwnerUsername(this->getOwnerUsername());
    tmp.setFilename(new_filename);
    s = tmp.get();
    if(!s.IsNotFound()){
        return Status::Corruption("Filename already exists");
    }

    if(new_filename != old_filename){
        s = this->get();
        // ver status

        s = this->erase();
        // ver status
        this->setFilename(new_filename);
        s = this->put();

        /// avisar a los usuarios
        //UserMetadata owner_user_metadata(db, this->batch);
        owner_user_metadata->setUsername(this->getOwnerUsername());
        s = owner_user_metadata->DBchange_my_filename(old_filename, new_filename);
        // ver status

        for(std::vector<std::string>::iterator it = users_with_read_permission.begin(); it != users_with_read_permission.end(); ++it){
            UserMetadata user_metadata(db, this->batch);
            user_metadata.setUsername(*it);
            s = user_metadata.DBchange_shared_filename(old_filename, new_filename);
            // ver status
        }
    }

    std::string extension_new = get_longest_extension_from_filename(new_filename);
    if(extension_new != this->getExtension()){
        s = this->_DBsetExtension(extension_new);
        // ver status
    }

    return s;
}

Status FileData::_DBsetExtension(std::string new_extension){
    Status s;

    s = this->get();
    // ver status
    this->setExtension(new_extension);

    s = this->put();

    return s;
}

Status FileData::_DBchangeModified(std::string username){
    Status s;

    s = this->get();
    // ver status
    this->setDateLastModified(get_date_and_time());
    this->setUserWhoLastModified(username);
    s = this->put();
    // ver status
    return s;
}

Status FileData::DBcreate(std::string n_content, std::string ubicacion){
    Status s;

    s = this->get();
    /*if(!s.IsNotFound()){
        return Status::Aborted("el archivo ya existe");
    }*/
    this->startBatch();

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Creando archivo en db",INFO);

    UserMetadata user_metadata(db, this->batch);
    user_metadata.setUsername(this->getOwnerUsername());

    s = this->_DBsetContent(n_content, &user_metadata);
    if(!s.ok()) return s;

    s = user_metadata.DBchange_ultima_ubicacion(ubicacion);
    if(!s.ok()) return s;

    /// seteos iniciales
    s = this->_DBsetExtension(get_longest_extension_from_filename(this->getFilename()));
    if(!s.ok()) return s;

    s = this->_DBchangeModified(this->getOwnerUsername());
    if(!s.ok()) return s;

    s = this->_DBsetFilename(this->getFilename(), &user_metadata);
    if(!s.ok()) return s;

    // agregar archivo a su usuario
    s = user_metadata.DBadd_my_file(this->getFilename()/*, content.size(), ubicacion*/);
    if(!s.ok()) return s;

    return this->endBatch();
}

Status FileData::DBmodify(std::string username, std::string n_filename, std::string ubicacion, std::string n_content, std::vector<std::string> &users_read_add,
                        std::vector<std::string> &users_read_remove, std::vector<std::string> &users_write_add, std::vector<std::string> &users_write_remove,
                        std::vector<std::string> &tags_add, std::vector<std::string> &tags_remove, std::vector<int> delete_versions){
    Status s = Status::OK();

    this->startBatch();
    UserMetadata user_metadata(db, this->batch);
    user_metadata.setUsername(username);

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Modificando archivo en db : "+this->filename,INFO);
    if(ubicacion != ""){
        s = user_metadata.DBchange_ultima_ubicacion(ubicacion);
        if(!s.ok()) return s;
    }

    // veo si tengo permiso para modificar
    s = this->_DBget_for_modify(username);
    if(!s.ok()) return s;

    s = this->_DBchangeModified(username);
    if(!s.ok()) return s;
    // ver status

    for(std::vector<std::string>::iterator it = tags_add.begin(); it != tags_add.end(); ++it){
        s = this->_DBaddTag(*it);
        if(!s.ok()) return s;
    }

    for(std::vector<std::string>::iterator it = tags_remove.begin(); it != tags_remove.end(); ++it){
        s = this->_DBremoveTag(*it);
        if(!s.ok()) return s;
    }

    // NOTA: si cambio filename y borro permisos al mismo tiempo se rompe. O no lo permitimos aca, o no lo permitimos en el cliente
    if(n_filename != ""){
        //if(username != this->getOwnerUsername()) return Status::Aborted("el usuario no tiene permiso para cambiar el nombre del archvo");
        s = this->_DBchangeFilename(n_filename, &user_metadata);
        if(!s.ok()) return s;
    }

    for(std::vector<std::string>::iterator it = users_read_add.begin(); it != users_read_add.end(); ++it){
        if(username != this->getOwnerUsername()) return Status::Corruption("Permission denied");
        s = this->_DBaddUserWithReadPermission(*it);
        if(!s.ok()) return s;
    }

    for(std::vector<std::string>::iterator it = users_read_remove.begin(); it != users_read_remove.end(); ++it){
        if(username != this->getOwnerUsername()) return Status::Corruption("Permission denied");
        s = this->_DBremoveUserWithReadPermission(*it);
        if(!s.ok()) return s;
    }

    for(std::vector<std::string>::iterator it = users_write_add.begin(); it != users_write_add.end(); ++it){
        if(username != this->getOwnerUsername()) return Status::Corruption("Permission denied");
        s = this->_DBaddUserWithWritePermission(*it);
        if(!s.ok()) return s;
    }

    for(std::vector<std::string>::iterator it = users_write_remove.begin(); it != users_write_remove.end(); ++it){
        if(username != this->getOwnerUsername()) return Status::Corruption("Permission denied");
        s = this->_DBremoveUserWithWritePermission(*it);
        if(!s.ok()) return s;
    }

    // en realidad deberia haber distintos handlers para las distintas modificaciones, entonces sus interfaces directamente no te dejan modificar las dos cosas de una
    // pero bueno, paja
    if(n_content != "" && delete_versions.size() > 0){
        return Status::InvalidArgument("Can't modify content and versions at the same time");
    } else if(n_content != ""){
        s = this->_DBsetContent(n_content, &user_metadata);
        if(!s.ok()) return s;
    } else if(delete_versions.size() > 0){
        if(username != this->getOwnerUsername()) return Status::Corruption("Permission denied");
        for(int i = 0; i < delete_versions.size(); ++i){
            s = this->_DBeraseVersion(delete_versions[i], &user_metadata);
            if(!s.ok()) return s;
        }
    }

    return this->endBatch();
}

Status FileData::_DBeraseVersion(int v, UserMetadata* user_metadata){
    Status s;

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Borrando version de "+this->filename+" : "+std::to_string(v),INFO);

    if(this->content.size() < 2 || v >= this->content.size()){
        return Status::Corruption("Can't delete chosen version");
    }

    s = this->get();
    // ver status
    //UserMetadata user_metadata(db, this->batch);
    user_metadata->setUsername(this->getOwnerUsername());
    double add_size = (this->content[v]).size();
    add_size = (-1) * add_size;
    s = user_metadata->DBmodif_file(add_size);
    // ver status

    this->content.erase(this->content.begin()+v);
    s = this->put();

    return s;
}
