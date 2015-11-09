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
    std::string username = hmsg.getUsername();
    if(username == "") return;
    std::string filename = hmsg.getFilename();
    if(filename == "") return;

    FileData file_data(db);
    file_data.setOwnerUsername(username);
    file_data.setFilename(filename);
    Status s = file_data.DBerase();
    // devolver mensaje de error
    hmsg.setResponse(s);
}
