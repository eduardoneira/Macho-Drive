#include "RecyclebinEmptyHandler.h"
#include "UserMetadata.h"
#include "FileData.h"
#include "Logger.h"

RecyclebinEmptyHandler::RecyclebinEmptyHandler(Database* db , TokenAuthenticator* a) : EventHandlerChecksAuthentication(db,a)
{
    //ctor
}

bool RecyclebinEmptyHandler::isMyRequest(HttpRequest &hmsg){
    // DELETE /files/'username'/recycle_bin/ limpia la recycle bin del usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo RecycleBinHandler",INFO);
    if (hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 3 &&
        hmsg.getUriParsedByIndex(2) == HttpRequest::RECYCLE_BIN &&
        hmsg.getMethod() == HttpRequest::DELETE){
        log->Log("Confirma que es un Handler tipo RecycleBinHandler",INFO);
        return true;
    }
    return false;
}

/*
    Este handler borra todos los archivos en la papelera
*/
void RecyclebinEmptyHandler::_handle(HttpRequest &hmsg){
    Status s;
    std::string username = hmsg.getUsername();
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("El campo recibido por username es : "+username,DEBUG);
    if(username == "") {
        return;
    }
    UserMetadata user_metadata(db);
    user_metadata.setUsername(username);
    log->Log("Corrobora que se encuentre en la base de datos", INFO);
    s = user_metadata.DBget();

    if (!s.ok()) {
        log->Log("No se encontro en la base de datos", WARNING);
        hmsg.setResponse(s);
        return;
    }
    log->Log("Si se encontro en la base de datos", INFO);
    FileData file_data(db);
    file_data.setOwnerUsername(username);
    std::vector<std::string> bin = user_metadata.getAllFilesBin();

    //DB ERASE : hacer uno que deletee solo el personal
    for(std::vector<std::string>::iterator it = bin.begin(); it != bin.end(); ++it){
        file_data.setFilename(*it);
        s = file_data.DBdelete_file();
        if (!s.ok()) break;
    }

    hmsg.setResponse(s);

}

RecyclebinEmptyHandler::~RecyclebinEmptyHandler()
{
    //dtor
}
