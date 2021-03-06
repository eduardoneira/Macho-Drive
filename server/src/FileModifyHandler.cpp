#include "FileModifyHandler.h"
#include "JsonSerializer.h"
#include "FileData.h"
#include "Util.h"
#include "UserMetadata.h"
#include <iostream>
#include "Logger.h"

FileModifyHandler::FileModifyHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

FileModifyHandler::~FileModifyHandler()
{
    //dtor
}

bool FileModifyHandler::isMyRequest(HttpRequest &hmsg){
    // PUT /files/'username'/'filename' quiere decir modificar archivo de tal usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo FileModify",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 3 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::PUT){
        log->Log("Confirma que es un Handler tipo FileModify",INFO);
        return true;
    }
    return false;
}

/// FORMATO PARA MODIFICACIONES

/*
    {
        // estos campos son obligatorios, para encontrar el archivo en la base de datos
        "filename" : "test.txt",
        "username" : "gabriel",

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

    Server_Logger* log = Server_Logger::getInstance();
    std::string filename = hmsg.getFilename();
    log->Log("El campo recibido por filename es : "+filename,DEBUG);
    if(filename == ""){
        hmsg.setResponse(Status::InvalidArgument());
        return;
    }
    std::string username = hmsg.getUsername();
    log->Log("El campo recibido por username es : "+username,DEBUG);
    if(username == ""){
        hmsg.setResponse(Status::InvalidArgument());
        return;
    }
    std::string owner_username = "";
    /*std::string owner_username = hmsg.getCampo("owner_username");
    log->Log("El campo recibido por owner username es : "+owner_username,DEBUG);
    if(owner_username == ""){
        hmsg.setResponse(Status::InvalidArgument());
        return;
    }*/

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);

    log->Log("Verifica a quien le pertenece el arcivo buscado",INFO);
    if(user_metadata.DBisMyFile(filename)){
        owner_username = username;
    } else {
        owner_username = user_metadata.DBisSharedFile(filename).first;

        if(owner_username == ""){
            log->Log("No se encontro el archivo buscado",WARNING);
            hmsg.setResponse(Status::NotFound("File not found"));
            return;
        }
    }

    std::string ubicacion = hmsg.getCampo("ubicacion");
    std::string filename_new = hmsg.getCampo("filename_change");
    std::string content_new = hmsg.getCampo("content_change");

    std::vector<int> delete_versions;
    for(int i = 0;; ++i){
        std::string v_str = hmsg.getCampoDeArray("delete_versions", i);
        if(v_str == "")
            break;
        delete_versions.push_back(atoi(v_str.c_str()));
    }

    std::vector<std::string> users_read_add;
    for(int i = 0;; ++i){
        std::string user = hmsg.getCampoDeArray("users_with_read_permission_add", i);
        if(user == "")
            break;
        //file_data.DBaddUserWithReadPermission(user);
        users_read_add.push_back(user);
    }

    std::vector<std::string> users_read_remove;
    for(int i = 0;; ++i){
        std::string user = hmsg.getCampoDeArray("users_with_read_permission_remove", i);
        if(user == "")
            break;
        //file_data.DBremoveUserWithReadPermission(user);
        users_read_remove.push_back(user);
    }

    std::vector<std::string> users_write_add;
    for(int i = 0;; ++i){
        std::string user = hmsg.getCampoDeArray("users_with_write_permission_add", i);
        if(user == "")
            break;
        //file_data.DBaddUserWithWritePermission(user);
        users_write_add.push_back(user);
    }

    std::vector<std::string> users_write_remove;
    for(int i = 0;; ++i){
        std::string user = hmsg.getCampoDeArray("users_with_write_permission_remove", i);
        if(user == "")
            break;
        //file_data.DBremoveUserWithWritePermission(user);
        users_write_remove.push_back(user);
    }

    std::vector<std::string> tags_add;
    for(int i = 0;; ++i){
        std::string tag = hmsg.getCampoDeArray("tags_add", i);
        if(tag == "")
            break;
        //file_data.DBaddTag(tag);
        tags_add.push_back(tag);
    }

    std::vector<std::string> tags_remove;
    for(int i = 0;; ++i){
        std::string tag = hmsg.getCampoDeArray("tags_delete", i);
        if(tag == "")
            break;
        //file_data.DBremoveTag(tag);
        tags_remove.push_back(tag);
    }

    FileData file_data(db);
    file_data.setFilename(filename);
    file_data.setOwnerUsername(owner_username);
    s = file_data.DBmodify(username, filename_new, ubicacion, content_new, users_read_add, users_read_remove, users_write_add, users_write_remove, tags_add, tags_remove, delete_versions);
    hmsg.setResponse(s);
}
