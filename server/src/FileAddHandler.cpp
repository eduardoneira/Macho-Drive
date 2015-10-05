#include "FileAddHandler.h"
#include "FileData.h"
#include "JsonSerializer.h"
#include "Util.h"
#include "UserMetadata.h"
#include "FileTag.h"
#include "FileExtension.h"
#include "FileName.h"

FileAddHandler::FileAddHandler(Database *db) : EventHandler(db)
{
    //ctor
}

FileAddHandler::~FileAddHandler()
{
    //dtor
}

void FileAddHandler::handle(HttpRequest &hmsg){
    JsonSerializer serialzier;

    std::string content = hmsg.getCampo("content");
    std::string filename = hmsg.getCampo("filename");
    std::string extension = get_longest_extension_from_filename(hmsg.getCampo("filename")); //hmsg.getCampo("extension");
    std::string owner_username = hmsg.getCampo("owner_username");
    std::string date_last_mod = "hoy"; //hmsg.getCampo("date_last_modified");
    std::string user_who_last_mod = owner_username; //hmsg.getCampo("user_who_last_modified");

    FileData file_data(db);
    file_data.setContent(content);
    file_data.setFilename(filename);
    file_data.setExtension(extension);
    //file_data.setOwnerKey();
    file_data.setOwnerUsername(owner_username);
    file_data.setDateLastModified(date_last_mod);
    file_data.setUserWhoLastModified(user_who_last_mod);

    std::string tag = "tags";
    for(int i = 0;; ++i){
        tag = hmsg.getCampoDeArray("tags", i);
        if(tag == "")
            break;
        file_data.addTag(tag);
    }

    std::string user_with_read_perm = "user_read_perm";
    for(int i = 0;; ++i){
        user_with_read_perm = hmsg.getCampoDeArray("users_with_read_permission", i);
        if(user_with_read_perm == "")
            break;
        file_data.addUserWithReadPermission(user_with_read_perm);
    }

    std::string user_with_write_perm = "user_write_perm";
    for(int i = 0;; ++i){
        user_with_write_perm = hmsg.getCampoDeArray("users_with_write_permission", i);
        if(user_with_write_perm == "")
            break;
        file_data.addUserWithWritePermission(user_with_write_perm);
    }

    /// hacer esto en un batch asi nos aseguramos q no hay info desactualizada
    // agregar archivo
    Status s = this->db->put(file_data);

    // agregar archivo a su usuario
    UserMetadata user_metadata(db);
    user_metadata.setUsername(owner_username);
    s = this->db->get(user_metadata);
    user_metadata.addMyFile(file_data.getFilename());
    s = this->db->put(user_metadata);

    // agregar archivo a sus tags (si tiene)
    for(std::vector<std::string>::iterator it = file_data.getTags()->begin(); it != file_data.getTags()->end(); ++it){
        FileTag file_tag(db);
        file_tag.setTag(*it);
        file_tag.setUsername(owner_username);
        s = this->db->get(file_tag);
        file_tag.addFileToken(file_data.getKey());
        s = this->db->put(file_tag);
    }

    // agregar archivo a su filename
    FileName file_name(db);
    file_name.setName(file_data.getFilename());
    file_name.setUsername(owner_username);
    s = this->db->get(file_name);
    file_name.addFileToken(file_data.getKey());
    s = this->db->put(file_name);

    // agregar archivo a su extension
    FileExtension file_extension(db);
    file_extension.setExtension(file_data.getExtension());
    file_extension.setUsername(owner_username);
    s = this->db->get(file_extension);
    file_extension.addFileToken(file_data.getKey());
    s = this->db->put(file_extension);
}
