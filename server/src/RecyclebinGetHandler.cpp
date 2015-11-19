#include "RecyclebinGetHandler.h"
#include "UserMetadata.h"
#include "Logger.h"

RecyclebinGetHandler::RecyclebinGetHandler(Database* db , TokenAuthenticator* a) : EventHandlerChecksAuthentication(db,a)
{
    //ctor
}

bool RecyclebinGetHandler::isMyRequest(HttpRequest &hmsg){
    // GET /files/'username'/recycle_bin/ devuelve los archivos de la recycle bin del usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo RecycleBinGet",INFO);
    if (hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 3 &&
        hmsg.getUriParsedByIndex(2) == HttpRequest::RECYCLE_BIN &&
        hmsg.getMethod() == HttpRequest::GET){
        log->Log("Confirma que es un Handler tipo RecycleBinGet",INFO);
        return true;
    }
    return false;
}

/*
En este handle se devulve los archivos que esten en la papelera de reciclaje del usuario que pidio.
*/

void RecyclebinGetHandler::_handle(HttpRequest &hmsg){

    std::string my_username = hmsg.getUsername();

    Server_Logger* log = Server_Logger::getInstance();
    log->Log("El campo recibido por username es : "+my_username,DEBUG);
    UserMetadata user_metadata(db);
    user_metadata.setUsername(my_username);

    Status s = user_metadata.DBget();

    if (!s.ok()){
        hmsg.setResponse(s,"No se encontro el usuario");
    } else{
        hmsg.setResponse(Status::OK(), user_metadata.getRecycleBin());
    }

}

RecyclebinGetHandler::~RecyclebinGetHandler()
{
    //dtor
}
