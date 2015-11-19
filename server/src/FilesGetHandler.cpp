#include "FilesGetHandler.h"
#include "UserMetadata.h"
#include "Logger.h"

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
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo FilesGet",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::COLLECTION_URI &&
        hmsg.getMethod() == HttpRequest::GET){
        log->Log("Confirma que es un Handler tipo FilesGet",INFO);
        return true;
    }
    return false;
}

void FilesGetHandler::_handle(HttpRequest &hmsg){
    Status s;

    Server_Logger* log = Server_Logger::getInstance();
    std::string username = hmsg.getUsername();
    log->Log("El campo recibido por username es : "+username,DEBUG);

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    s = user_metadata.DBget();

    if(s.ok())
        hmsg.setResponse(Status::OK(), user_metadata.getFileTreeJson());
    else
        hmsg.setResponse(s);
}
