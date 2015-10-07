#include "FileGetHandler.h"
#include "FileData.h"
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

    std::string owner_username = hmsg.getCampo("owner_username");
    std::string username = hmsg.getCampo("username");
    std::string filename = hmsg.getCampo("filename");

    FileData file_data(db);
    file_data.setOwnerUsername(owner_username);
    file_data.setFilename(filename);

    s = file_data.DBget_for_read(username);
    // ver status, si no existe mandar error
    if(s.ok()){
        hmsg.setResponse(file_data.getValueToString());
    } else {
        hmsg.setResponse(s.ToString());
    }
}
