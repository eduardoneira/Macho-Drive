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

bool FileDeleteHandler::isMyRequest(HttpRequest &hmsg){
    // DELETE /files/'username'/'filename' quiere decir borrar archivo de tal usuario
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 3 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::DELETE){
        return true;
    }
    return false;
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
