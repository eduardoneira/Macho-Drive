#include "FilesGetHandler.h"
#include "UserMetadata.h"

FilesGetHandler::FilesGetHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

FilesGetHandler::~FilesGetHandler()
{
    //dtor
}

bool FilesGetHandler::isMyRequest(HttpRequest &hmsg){
    // GET /files/'username'/ devuelve un arbol de archivos
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::COLLECTION_URI &&
        hmsg.getMethod() == HttpRequest::GET){
        return true;
    }
    return false;
}

void FilesGetHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string username = hmsg.getUsername();

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    s = user_metadata.DBget();

    if(s.ok())
        hmsg.setResponse(Status::OK(), user_metadata.getFileTreeJson());
    else
        hmsg.setResponse(s);
}
