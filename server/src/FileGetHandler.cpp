#include "FileGetHandler.h"
#include "FileData.h"
#include "UserMetadata.h"
#include "Util.h"
#include "Logger.h"

FileGetHandler::FileGetHandler(Database *db, TokenAuthenticator *a) : EventHandlerChecksAuthentication(db, a)
{
    //ctor
}

FileGetHandler::~FileGetHandler()
{
    //dtor
}

bool FileGetHandler::isMyRequest(HttpRequest &hmsg){
    // GET /files/'username'/'filename' quiere decir pedir archivo de tal usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo FileGet",INFO);
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 3 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::GET){
        log->Log("Confirma que es un Handler tipo FileGet",INFO);
        return true;
    }
    return false;
}

// tal vez permitir hacer get de un campo particular del file, por ej solo del contenido o solo de los tags?
void FileGetHandler::_handle(HttpRequest &hmsg){
    Status s;

    //std::string owner_username = hmsg.getCampo("owner_username");
    //if(owner_username == "") return;
    Server_Logger* log = Server_Logger::getInstance();
    std::string owner_username = "";
    std::string username = hmsg.getUsername();
    log->Log("El campo recibido por username es : "+username,DEBUG);
    if(username == ""){
        hmsg.setResponse(Status::InvalidArgument());
        return;
    }
    std::string filename = hmsg.getFilename();
    log->Log("El campo recibido por filename es : "+filename,DEBUG);
    if(filename == ""){
        hmsg.setResponse(Status::InvalidArgument());
        return;
    }

    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);

    log->Log("Verifica a quien le pertenece el arcivo buscado",INFO);
    if(user_metadata.DBisMyFile(filename)){
        owner_username = username;
    } else {
        owner_username = user_metadata.DBisSharedFile(filename).first;

        if(owner_username == ""){
            log->Log("No se encontro el archivo buscado",WARNING);
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
