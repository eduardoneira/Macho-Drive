#include "RecyclebinRecoverHandler.h"
#include "UserMetadata.h"
#include "Logger.h"

RecyclebinRecoverHandler::RecyclebinRecoverHandler(Database* db , TokenAuthenticator* a) : EventHandlerChecksAuthentication(db,a)
{
    //ctor
}

bool RecyclebinRecoverHandler::isMyRequest(HttpRequest &hmsg){
    //PUT /files/'username'/recycle_bin/'filename' restaura el archivo de la recycle bin del usuario
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("Verifica que se trate de un Handler tipo RecycleBinRecover",INFO);
    if (hmsg.getUriParsedByIndex(0) == HttpRequest::FILES &&
        hmsg.getUriCantCampos() == 4 &&
        hmsg.getUriParsedByIndex(2) == HttpRequest::RECYCLE_BIN &&
        hmsg.getMethod() == HttpRequest::PUT){
        log->Log("Confirma que es un Handler tipo RecycleBinRecover",INFO);
        return true;
    }
    return false;
}

void RecyclebinRecoverHandler::_handle(HttpRequest &hmsg){
    std::string my_username = hmsg.getUsername();
    Server_Logger* log = Server_Logger::getInstance();
    log->Log("El campo recibido por username es : "+my_username,DEBUG);
    if (my_username == "" ) return;
    std::string filename = hmsg.getUriStringParsedByIndex(3);
    log->Log("El campo recibido por filename es : "+filename,DEBUG);
    if (filename == "") return;

    UserMetadata user_metadata(db);
    user_metadata.setUsername(my_username);

    Status s = user_metadata.DBget();

    if (!s.ok()){
        hmsg.setResponse(s,"No se encontro el usuario");
    } else{
        if(user_metadata.recoverFileRecycleBin(filename)){
            log->Log("El archivo fue recuperado con exito",INFO);
        hmsg.setResponse(Status::OK(), "El archivo ha sido recuperado satisfactoriamente");
        }else{
            log->Log("El archivo no pudo ser recuperado con exito",WARNING);
            hmsg.setResponse(Status::NotFound(), "No se encontro el archivo");
        }
    }
}

RecyclebinRecoverHandler::~RecyclebinRecoverHandler()
{
    //dtor
}

