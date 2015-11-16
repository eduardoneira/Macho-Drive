#include "FileDeleteHandler.h"
#include "FileData.h"
#include "UserMetadata.h"
#include <iostream>

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

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    user_metadata.DBget();
    std::cout << user_metadata.getAllFilesBin().size() << std::endl;

    // devolver mensaje de error
    hmsg.setResponse(s);
}
