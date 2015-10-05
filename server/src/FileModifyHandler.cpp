#include "FileModifyHandler.h"
#include "JsonSerializer.h"
#include "FileData.h"
#include "Util.h"
#include "FileExtension.h"
#include "FileTag.h"
#include "FileName.h"
#include "UserMetadata.h"
#include <iostream>

FileModifyHandler::FileModifyHandler(Database *db) : EventHandler(db)
{
    //ctor
}

FileModifyHandler::~FileModifyHandler()
{
    //dtor
}

/// FORMATO PARA MODIFICACIONES

/*
    {
        // estos campos son obligatorios, para encontrar el archivo en la base de datos
        "filename" : "test.txt",
        "owner_username" : "gabriel",

        // estos campos refieren a los cambios y son opcionales, si alguno no existe no se hace ningun cambio a la variable que representan
        "filename_change" : "test_update.txt",
        // por ahora es todo el nuevo contenido del archivo. tal vez se podria mejorar y hacer que trabaje con diferencias, pero si probamos esto y no es ineficiente esta bien para mi
        "content_change" : "nuevo contenido",
        // aca no hay ningun chequeo por ahora, si me pasan que borre uno que no existe no pasa nada. Pero si me dicen que agregue a alguien y despues lo borre, es medio turbio
        // para mi el chequeo de 'borrar solo los que ya estan en la lista' o por lo menos 'no pedir agregar y borrar a uno en la misma modificacion' deberian ir en el cliente, pero es debatible
        "users_with_read_permission_add" : [ "edu" ],
        "users_with_read_permission_remove" : [ "nico" ],
        // lo mismo, y ademas si le sacan privilegio de leer a alguien tambien le deberian sacar el de escribir
        // ese chequeo ahora no esta, yo lo pondria en el cliente tambien (va, tal vez lo mejor es que este en los dos)
        "users_with_write_permission_add" : [ "" ],
        "users_with_write_permission_remove" : [ "" ],
        "tags_add" : [ "" ],
        "tags_remove" : [ "" ],
        // todos los campos anteriores deberian basarse en el input del usuario, este lo pone el cliente. seria la cuenta desde la que estoy modificando
        "user_who_modified" : ""
    }
*/

void FileModifyHandler::handle(HttpRequest &hmsg){
    JsonSerializer serializer;

    std::string filename = hmsg.getCampo("filename");
    std::string owner_username = hmsg.getCampo("owner_username");
    FileData file_data(db);
    file_data.setFilename(filename);
    file_data.setOwnerUsername(owner_username);
    //std::cout << file_data.getKey() << std::endl;
    Status s = this->db->get(file_data);
    file_data.setFilename(filename); // despues de _setValueVars, las variables quedan con "" del json, tal vez hay que sacar eso
    file_data.setOwnerUsername(owner_username);
    //std::cout << file_data.getKey() << std::endl;

    // cambia la clave
    std::string filename_new = hmsg.getCampo("filename_change");
    if(filename_new != ""){
        std::string extension_new = get_longest_extension_from_filename(hmsg.getCampo("filename_change"));
        if(extension_new != file_data.getExtension()){

            file_data.setExtension(extension_new);

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
        }
    }

    std::string content_new = hmsg.getCampo("content_change");
    if(content_new != ""){
        file_data.setContent(content_new);
    }

    // fijarse bien que combinaciones de permisos tienen sentido
    for(int i = 0;; ++i){
        std::string temp = hmsg.getCampoDeArray("users_with_read_permission_add", i);
        if(temp == "")
            break;
        file_data.addUserWithReadPermission(temp);

        UserMetadata user_metadata(db);
        user_metadata.setUsername(temp);
        s = this->db->get(user_metadata);
        user_metadata.addSharedFile(file_data.getFilename(), file_data.getOwnerUsername());
        s = this->db->put(user_metadata);
    }

    for(int i = 0;; ++i){
        std::string temp = hmsg.getCampoDeArray("users_with_read_permission_remove", i);
        if(temp == "")
            break;
        file_data.removeUserWithReadPermission(temp);

        UserMetadata user_metadata(db);
        user_metadata.setUsername(temp);
        s = this->db->get(user_metadata);
        user_metadata.removeSharedFile(file_data.getFilename(), file_data.getOwnerUsername());
        s = this->db->put(user_metadata);
    }

    for(int i = 0;; ++i){
        std::string temp = hmsg.getCampoDeArray("users_with_write_permission_add", i);
        if(temp == "")
            break;
        file_data.addUserWithWritePermission(temp);

        UserMetadata user_metadata(db);
        user_metadata.setUsername(temp);
        s = this->db->get(user_metadata);
        user_metadata.addSharedFile(file_data.getFilename(), file_data.getOwnerUsername());
        s = this->db->put(user_metadata);
    }

    for(int i = 0;; ++i){
        std::string temp = hmsg.getCampoDeArray("users_with_write_permission_remove", i);
        if(temp == "")
            break;
        file_data.removeUserWithWritePermission(temp);
    }

    for(int i = 0;; ++i){
        std::string temp = hmsg.getCampoDeArray("tags_add", i);
        if(temp == "")
            break;
        file_data.addTag(temp);

        FileTag file_tag(db);
        file_tag.setTag(temp);
        this->db->get(file_tag);
        file_tag.addFileToken(file_data.getKey());
        this->db->put(file_tag);
    }

    for(int i = 0;; ++i){
        std::string temp = hmsg.getCampoDeArray("tags_delete", i);
        if(temp == "")
            break;
        file_data.removeTag(temp);

        FileTag file_tag(db);
        file_tag.setTag(temp);
        this->db->get(file_tag);
        file_tag.removeFileToken(file_data.getKey());
        this->db->put(file_tag);
    }

    file_data.setUserWhoLastModified(hmsg.getCampo("user_who_modified"));
    file_data.setDateLastModified("hoy"); // cambiar

    if(filename_new != ""){
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
    }
    //std::cout << file_data.getKey() << std::endl;
    this->db->put(file_data);
}
