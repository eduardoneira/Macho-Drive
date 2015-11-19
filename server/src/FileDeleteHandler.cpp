#include "FileDeleteHandler.h"
#include "FileData.h"
#include "UserMetadata.h"
#include <iostream>
#include "Logger.h"

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
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo FileDelete",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 3 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::DELETE){
        log->Log("Confirma que es un Handler tipo FileDelete",INFO);
        return true;
    }
    return false;
}

void FileDeleteHandler::_handle(HttpRequest &hmsg){
    Server_Logger* log = Server_Logger::getInstance();
    std::string username = hmsg.getUsername();
    log->Log("El campo recibido por username es : "+username,DEBUG);
    if(username == "") return;
    std::string filename = hmsg.getFilename();
    log->Log("El campo recibido por filename es : "+filename,DEBUG);
    if(filename == "") return;

    FileData file_data(db);
    file_data.setOwnerUsername(username);
    file_data.setFilename(filename);
    Status s = file_data.DBerase();

    // devolver mensaje de error
    hmsg.setResponse(s);
}
