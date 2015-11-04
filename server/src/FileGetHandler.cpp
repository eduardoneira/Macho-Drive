#include "FileGetHandler.h"
#include "FileData.h"
#include "UserMetadata.h"
#include "Util.h"

FileGetHandler::FileGetHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

FileGetHandler::~FileGetHandler()
{
    //dtor
}

// tal vez permitir hacer get de un campo particular del file, por ej solo del contenido o solo de los tags?
void FileGetHandler::_handle(HttpRequest &hmsg){
    Status s;

    //std::string owner_username = hmsg.getCampo("owner_username");
    //if(owner_username == "") return;
    std::string owner_username = "";
    std::string username = hmsg.getUsername();
    if(username == "") return;
    std::string filename = hmsg.getFilename();
    if(filename == "") return;

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);

    if(user_metadata.DBisMyFile(filename)){
        owner_username = username;
    } else {
        owner_username = user_metadata.DBisSharedFile(filename).first;

        if(owner_username == ""){
            hmsg.setResponse(Status::NotFound("No se encontro el archivo indicado"));
        }
    }

    FileData file_data(db);
    file_data.setOwnerUsername(owner_username);
    file_data.setFilename(filename);

    s = file_data.DBget_for_read(username);

    if(s.ok()){
        hmsg.setResponse(Status::OK(), file_data.getValueToString());
    } else {
        hmsg.setResponse(s);
    }
}
