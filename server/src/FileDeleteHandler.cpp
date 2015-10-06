#include "FileDeleteHandler.h"
#include "FileData.h"

FileDeleteHandler::FileDeleteHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

FileDeleteHandler::~FileDeleteHandler()
{
    //dtor
}

void FileDeleteHandler::_handle(HttpRequest &hmsg){
    FileData file_data(db);
    file_data.setOwnerUsername(hmsg.getCampo("owner_username"));
    file_data.setFilename(hmsg.getCampo("filename"));
    Status s = file_data.DBerase();
    // devolver mensaje de error
}
