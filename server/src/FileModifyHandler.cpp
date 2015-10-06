#include "FileModifyHandler.h"
#include "JsonSerializer.h"
#include "FileData.h"
#include "Util.h"
#include "FileExtension.h"
#include "FileTag.h"
#include "FileName.h"
#include "UserMetadata.h"
#include <iostream>

FileModifyHandler::FileModifyHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
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

void FileModifyHandler::_handle(HttpRequest &hmsg){
    Status s;

    JsonSerializer serializer;

    std::string filename = hmsg.getCampo("filename");
    std::string owner_username = hmsg.getCampo("owner_username");

    FileData file_data(db);
    file_data.setFilename(filename);
    file_data.setOwnerUsername(owner_username);
    //s = file_data.DBget();
    // ver status

    // cambia la clave
    std::string filename_new = hmsg.getCampo("filename_change");
    if(filename_new != ""){
        file_data.DBsetFilename(filename_new);
    }

    std::string content_new = hmsg.getCampo("content_change");
    if(content_new != ""){
        file_data.DBsetContent(content_new);
    }

    for(int i = 0;; ++i){
        std::string user = hmsg.getCampoDeArray("users_with_read_permission_add", i);
        if(user == "")
            break;
        file_data.DBaddUserWithReadPermission(user);
    }

    for(int i = 0;; ++i){
        std::string user = hmsg.getCampoDeArray("users_with_read_permission_remove", i);
        if(user == "")
            break;
        file_data.DBremoveUserWithReadPermission(user);
    }

    for(int i = 0;; ++i){
        std::string user = hmsg.getCampoDeArray("users_with_write_permission_add", i);
        if(user == "")
            break;
        file_data.DBaddUserWithWritePermission(user);
    }

    for(int i = 0;; ++i){
        std::string user = hmsg.getCampoDeArray("users_with_write_permission_remove", i);
        if(user == "")
            break;
        file_data.DBremoveUserWithWritePermission(user);
    }

    for(int i = 0;; ++i){
        std::string tag = hmsg.getCampoDeArray("tags_add", i);
        if(tag == "")
            break;
        file_data.DBaddTag(tag);
    }

    for(int i = 0;; ++i){
        std::string tag = hmsg.getCampoDeArray("tags_delete", i);
        if(tag == "")
            break;
        file_data.DBremoveTag(tag);
    }
}
