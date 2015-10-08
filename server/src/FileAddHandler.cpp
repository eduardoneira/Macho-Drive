#include "FileAddHandler.h"
#include "FileData.h"
#include "JsonSerializer.h"
#include "Util.h"
#include "UserMetadata.h"
#include "FileTag.h"
#include "FileExtension.h"
#include "FileName.h"

FileAddHandler::FileAddHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

FileAddHandler::~FileAddHandler()
{
    //dtor
}

void FileAddHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string filename = hmsg.getCampo("filename");
    if(filename == "") return;
    std::string owner_username = hmsg.getCampo("username");
    if(owner_username == "") return;
    std::string ubicacion = hmsg.getCampo("ubicacion");
    std::string content = hmsg.getCampo("content");
    if(content == "") return;

    FileData file_data(db);
    file_data.setFilename(filename);
    file_data.setOwnerUsername(owner_username);
    s = file_data.DBcreate(content, ubicacion);
    if(!s.ok()){
        hmsg.setResponse(s.ToString());
        return;
    }

    /// esto va aca, o directo cuando creas un archivo no tiene nada de esto y se agrega con modificaciones?

    std::string tag = "tags";
    for(int i = 0;; ++i){
        tag = hmsg.getCampoDeArray("tags", i);
        if(tag == "")
            break;
        s = file_data.DBaddTag(tag);
        // ver status
    }

    std::string user_with_read_perm = "user_read_perm";
    for(int i = 0;; ++i){
        user_with_read_perm = hmsg.getCampoDeArray("users_with_read_permission", i);
        if(user_with_read_perm == "")
            break;
        s = file_data.DBaddUserWithReadPermission(user_with_read_perm);
        // ver status
    }

    std::string user_with_write_perm = "user_write_perm";
    for(int i = 0;; ++i){
        user_with_write_perm = hmsg.getCampoDeArray("users_with_write_permission", i);
        if(user_with_write_perm == "")
            break;
        s = file_data.DBaddUserWithWritePermission(user_with_write_perm);
        // ver status
    }

    hmsg.setResponse(s.ToString());
}
