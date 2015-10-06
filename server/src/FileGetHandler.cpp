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

    FileData file_data(db);
    file_data.setOwnerUsername(hmsg.getCampo("owner_username"));
    file_data.setFilename(hmsg.getCampo("filename"));
    s = file_data.DBget();
    // ver status, si no existe mandar error

    hmsg.setResponse(file_data.getValueToString());
}
